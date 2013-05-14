//  Copyright [2013] Kyle Galloway (kyle.s.galloway@gmail.com)
//                   Pravish Sood (pravish.sood@gmail.com)
//                   Dylan Kelemen (dckelemen@gmail.com)

//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at

//      http://www.apache.org/licenses/LICENSE-2.0

//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "odbcintf.h"
#include "sql_parser.h"
#include "sql_select_statement.h"

#include <mongo/client/dbclient.h>
#include <mongo/bson/bsonobj.h>

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/variant.hpp>

#include <iostream>
#include <string>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sql.h>
#include <sqlext.h>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace spirit = boost::spirit;
namespace phoenix = boost::phoenix;

namespace {

SQLHENV envHandle;
    
// connection handle
SQLHDBC dbHandle;

struct DumpTables {
    template<typename Arg>
    struct result {
        typedef bool type;
    };

    template<typename Arg>
    bool operator()(const Arg& db) const
    {
        if (db.size()) {
        std::cout << "Getting tables for db: "
                  << db
                  << " using SQLTables"
                  << std::endl;
        } else {
            std::cout << "Getting all tables for all dbs using SQLTables" << std::endl;
        }
        
        SQLRETURN ret;
        SQLHSTMT stmtHandle;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, dbHandle, &stmtHandle);
        if (!SQL_SUCCEEDED(ret)) {
            std::cerr << "Failed to allocate session handle" << std::endl;
            return false;
        }

        ret = SQLTables(stmtHandle, NULL, 0, NULL, 0, NULL, 0, (SQLCHAR*)"TABLE", SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            std::cerr << "SQLTablesFailed" << std::endl;
            return false;
        }

        char schemaName[256];
        char tableName[256];
        while(SQL_SUCCEEDED(ret = SQLFetch(stmtHandle))) {
            SQLLEN len;
            ret = SQLGetData(stmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)schemaName, 256, &len);
            if (db.size() && db != schemaName) {
                continue;
            }
            ret = SQLGetData(stmtHandle, 3, SQL_C_CHAR, (SQLPOINTER)tableName, 256, &len);
            std::cout << schemaName << "." << tableName << std::endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

        return false;
    }
};

struct DumpColumns {
    template<typename Arg1, typename Arg2>
    struct result {
        typedef bool type;
    };

    template<typename Arg1, typename Arg2>
    bool operator()(const Arg1& db, const Arg2& collection) const
    {
        std::cout << "Getting columns for collection: "
                  << db << "." << collection
                  << " using SQLColumns"
                  << std::endl;
        SQLRETURN ret;
        SQLHSTMT stmtHandle;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, dbHandle, &stmtHandle);
        if (!SQL_SUCCEEDED(ret)) {
            std::cerr << "Failed to allocate session handle" << std::endl;
            return false;
        }

        ret = SQLColumns(stmtHandle,
                         NULL,
                         0,
                         (SQLCHAR*)db.c_str(),
                          SQL_NTS,
                         (SQLCHAR*)collection.c_str(),
                         SQL_NTS,
                         NULL,
                         0);
        if (!SQL_SUCCEEDED(ret)) {
            std::cerr << "SQLColumns Failed" << std::endl;
            return false;
        }
        char schemaName[256];
        char tableName[256];
        char columnName[256];
        while(SQL_SUCCEEDED(ret = SQLFetch(stmtHandle))) {
            SQLLEN len;
            ret = SQLGetData(stmtHandle, 4, SQL_C_CHAR, (SQLPOINTER)columnName, 256, &len);
            std::cout << columnName << std::endl;
        }
        
        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

        return false;
    }
};

template <typename It>
struct CommandParser : qi::grammar<It, bool, ascii::space_type> {
    qi::rule<It, std::string(), ascii::space_type> _db;
    qi::rule<It, bool, ascii::space_type> _rule;
    mongoodbc::SQLParser<It> _sql;

    CommandParser();
};

template <typename It>
CommandParser<It>::CommandParser()
    : CommandParser::base_type(_rule)
{
    phoenix::function<DumpTables> dumpTables;
    phoenix::function<DumpColumns> dumpColumns;

    _db %= +ascii::alnum;

    _rule = ascii::no_case["quit"] [qi::_val = true] |
            ascii::no_case[".dbs"] [qi::_val = dumpTables(std::string(""))] |
            (ascii::no_case[".columns"] >> _db >> '.' >> _db) [qi::_val = dumpColumns(qi::_1, qi::_2)] |
            (ascii::no_case[".tables"] >> _db) [qi::_val = dumpTables(qi::_1)];
}

} // close unnamed namespace

int main(int argc, char **argv)
{
    std::cout << "mongoDB ODBC Demo Application" << std::endl;

    // allocate environment handle
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle);

    // ask for ODBC 3.0
    SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

    // allocate a connection handle
    SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &dbHandle);

    SQLCHAR outstr[1024];
    SQLSMALLINT outstrlen;
    SQLCHAR *dsn = (SQLCHAR *)"DSN=MongoDB";
    // connect to the mongodb driver
    SQLRETURN ret = SQLDriverConnect(dbHandle, NULL, dsn ,SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_COMPLETE);

    if (SQL_SUCCEEDED(ret)) {
        std::cout << "Successfully connected through ODBC: "
                  << std::endl;
    } else {
        std::cerr << "Failed to connect"
                  << std::endl;
        std::exit(-1);
    }
    CommandParser<std::string::const_iterator> parser;
    mongoodbc::SQLParser<std::string::const_iterator> sqlParser;
    while(true) {
        char *line = readline("mongoodbc> ");
        if (!line || !*line) {
            std::cout << std::endl;
            continue;
        }
        add_history(line);
        std::string cmd(line);
        if (cmd == "quit") {
            break;
        }
        std::string::const_iterator begin = cmd.begin();
        std::string::const_iterator end = cmd.end();
        free(line);

        bool quit = false;
        bool result = qi::phrase_parse(begin, end, parser, ascii::space, quit);
        if (!result) {
            mongoodbc::SQLStatement stmt;
            result = qi::phrase_parse(begin, end, sqlParser, ascii::space, stmt);
            if (!result) {
                std::cerr << "Cannot parse command: '"
                          << cmd
                          << "'"
                          << std::endl;
                continue;
            }
            std::cout << "Executing as a query: '"
                      << cmd
                      << "' - parsed form '"
                      << stmt
                      << "'"
                      << std::endl;
            SQLRETURN ret;
            SQLHSTMT stmtHandle;
            ret = SQLAllocHandle(SQL_HANDLE_STMT, dbHandle, &stmtHandle);
            if (!SQL_SUCCEEDED(ret)) {
                std::cerr << "Failed to allocate session handle" << std::endl;
                continue;
            }

            std::string& table = stmt._tableRefList[0];
            size_t idx = table.find('.');
            std::string db(table, 0, idx);
            std::string collection(table, idx + 1);

            ret = SQLColumns(stmtHandle,
                             NULL,
                             0,
                             (SQLCHAR*)db.c_str(),
                              SQL_NTS,
                             (SQLCHAR*)collection.c_str(),
                             SQL_NTS,
                             NULL,
                             0);
            if (!SQL_SUCCEEDED(ret)) {
                std::cerr << "SQLColumns Failed" << std::endl;
                return false;
            }
            char columnName[256];
            std::vector<std::string> columnNames;
            while(SQL_SUCCEEDED(ret = SQLFetch(stmtHandle))) {
                SQLLEN len;
                ret = SQLGetData(stmtHandle, 4, SQL_C_CHAR, (SQLPOINTER)columnName, 256, &len);
                columnNames.push_back(columnName);
            }

            ret = SQLExecDirect(stmtHandle, (SQLCHAR *)cmd.c_str(), SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                std::cerr << "query: '"
                          << cmd
                          << "' failed"
                          << std::endl;
                SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
                continue;
            }

            SQLSMALLINT numResultColumns;
            ret = SQLNumResultCols(stmtHandle, &numResultColumns);
            if (!SQL_SUCCEEDED(ret)) {
                std::cerr << "query: '"
                          << cmd
                          << "' failed to get num result columns"
                          << std::endl;
                SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
                continue;  
            }
            std::cout << "Result has: " << numResultColumns << " columns" << std::endl;

            int row = 1;
            while(SQL_SUCCEEDED(ret = SQLFetch(stmtHandle))) {
                std::cout << "Row " << row << std::endl;
                for (int i = 1; i <= numResultColumns; ++i) {
                    unsigned long int value;
                    SQLLEN len;
                    SQLGetData(stmtHandle, i, SQL_C_ULONG, (SQLPOINTER)&value, sizeof(unsigned long int), &len);
                    std::cout << "    " << columnNames[i-1] << ": " << value << std::endl;
                }
                ++row;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
        }
    }

    SQLFreeHandle(SQL_HANDLE_DBC, dbHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, envHandle);
}
