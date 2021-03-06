
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

#include "statement_handle.h"
#include "connection_handle.h"

#include <boost/variant/get.hpp>
#include <boost/spirit/include/qi.hpp>

#include <string.h>

namespace mongoodbc {

SQLSMALLINT StatementHandle::mapMongoToODBCDataType(mongo::BSONType type)
{
    switch(type) {
        case mongo::NumberInt: return SQL_INTEGER;
    }

    return 0;
}

const char * StatementHandle::dataTypeName(SQLSMALLINT type)
{
    switch(type) {
        case SQL_INTEGER: return "INTEGER";
    }

    return "";
}

SQLINTEGER StatementHandle::columnSize(SQLSMALLINT type)
{
    switch(type) {
        case SQL_INTEGER: return 10;
    }

    return 0;
}

SQLINTEGER StatementHandle::bufferLength(SQLSMALLINT type)
{
    return columnSize(type);
}

SQLSMALLINT StatementHandle::decimalDigits(SQLSMALLINT type)
{
    switch(type) {
        case SQL_INTEGER: return 0;
    }

    return 0;
}

SQLSMALLINT StatementHandle::numPercRadix(SQLSMALLINT type)
{
    return 10;
}

StatementHandle::StatementHandle(ConnectionHandle *connHandle)
    : _connHandle(connHandle)
{
}

SQLSMALLINT StatementHandle::mapODBCDataTypeToSQLDataType(SQLSMALLINT type)
{
    switch(type) {
    }

    return type;
}

SQLSMALLINT StatementHandle::getDatetimeSubcode(SQLSMALLINT type)
{
    return (SQLSMALLINT)NULL;
}

SQLINTEGER StatementHandle::maxCharLen(SQLSMALLINT type)
{
    return (SQLINTEGER)NULL;
}

SQLRETURN StatementHandle::sqlTables(SQLCHAR *catalogName,
                                     SQLSMALLINT catalogNameLen,
                                     SQLCHAR *schemaName,
                                     SQLSMALLINT schemaNameLen,
                                     SQLCHAR *tableName,
                                     SQLSMALLINT tableNameLen,
                                     SQLCHAR *tableType,
                                     SQLSMALLINT tableTypeLen)
{
    _cursorColumns.clear();
    _cursor.reset();
    _resultSet.clear();
    _rowIdx = -1;
    if (NULL != tableType) {
        std::string tableTypeStr;
        if (tableTypeLen == SQL_NTS) {
            tableTypeStr.assign((char *)tableType);
        } else {
            tableTypeStr.assign((char *)tableType, (int)tableTypeLen);
        }
        if ("TABLE" != tableTypeStr && "'TABLE'" != tableTypeStr) {
            // No other table tyoes are supported
            return SQL_SUCCESS;
        }
    }

    std::list<std::string> schemas;
    int rc = _connHandle->getDbNames(&schemas);
    if (0 != rc) {
        return SQL_ERROR;
    }
    if (NULL != schemaName) {
        std::string schemaNameStr;
        if (schemaNameLen == SQL_NTS) {
            schemaNameStr.assign((char *)schemaName);
        } else {
            schemaNameStr.assign((char *)schemaName, (int)schemaNameLen);
        }

        // TODO: filter 'schemas'
    }
    
    // map from schema name to table names
    for (std::list<std::string>::const_iterator it = schemas.begin();
         it != schemas.end();
         ++it) {
        std::list<std::string> tables;
        int rc = _connHandle->getCollectionNames(*it, &tables);
        if (0 != rc) {
            return SQL_ERROR;
        }
        for (std::list<std::string>::const_iterator tableIt = tables.begin();
             tableIt != tables.end();
             ++tableIt) {
            size_t periodIdx = tableIt->find('.');
            size_t periodIdx2 = tableIt->find('.', periodIdx + 1);
            if (periodIdx2 != std::string::npos) {
                periodIdx2 = periodIdx2 - periodIdx - 1;
            }
            std::string tableName(*tableIt, periodIdx + 1, periodIdx2);
            if ("system" == tableName) {
                // skip mongodb internal tables
                continue;
            }
            _resultSet.push_back(std::list<Result>());
            std::list<Result>& results = _resultSet.back();
            results.push_back("NULL");
            results.push_back(*it);
            results.push_back(tableName);
            results.push_back("TABLE");
            results.push_back("NULL");
        }
    }
    if (NULL != tableName) {
        std::string tableNameStr;
        if (tableNameLen == SQL_NTS) {
            tableNameStr.assign((char *)tableName);
        } else {
            tableNameStr.assign((char *)tableName, (int)tableNameLen);
        }

        //TODO: filter 'tables'
    }

    return SQL_SUCCESS;
}
    
SQLRETURN StatementHandle::sqlColumns(SQLCHAR *catalogName,
                                      SQLSMALLINT catalogNameLen,
                                      SQLCHAR *schemaName,
                                      SQLSMALLINT schemaNameLen,
                                      SQLCHAR *tableName,
                                      SQLSMALLINT tableNameLen,
                                      SQLCHAR *columnName,
                                      SQLSMALLINT columnNameLen)
{
    _cursorColumns.clear();
    _cursor.reset();
    _resultSet.clear();
    _rowIdx = -1;

    std::list<std::string> schemas;
    int rc = _connHandle->getDbNames(&schemas);
    if (0 != rc) {
        return SQL_ERROR;
    }
    std::string schemaNameStr;
    if (NULL != schemaName) {
        if (schemaNameLen == SQL_NTS) {
            schemaNameStr.assign((char *)schemaName);
        } else {
            schemaNameStr.assign((char *)schemaName, (int)schemaNameLen);
        }
    }
    std::string tableNameStr;
    if (NULL != tableName) {
        if (tableNameLen == SQL_NTS) {
            tableNameStr.assign((char *)tableName);
        } else {
            tableNameStr.assign((char *)tableName, (int)tableNameLen);
        }
    }
    
    // map from schema name to table names
    for (std::list<std::string>::const_iterator it = schemas.begin();
         it != schemas.end();
         ++it) {
        if (schemaNameStr.size() && *it != schemaNameStr) {
            continue;
        }
        std::list<std::string> tables;
        int rc = _connHandle->getCollectionNames(*it, &tables);
        if (0 != rc) {
            return SQL_ERROR;
        }
        for (std::list<std::string>::const_iterator tableIt = tables.begin();
             tableIt != tables.end();
             ++tableIt) {
            size_t periodIdx = tableIt->find('.');
            size_t periodIdx2 = tableIt->find('.', periodIdx + 1);
            if (periodIdx2 != std::string::npos) {
                periodIdx2 = periodIdx2 - periodIdx - 1;
            }
            std::string tableName(*tableIt, periodIdx + 1, periodIdx2);
            if ("system" == tableName) {
                // skip mongodb internal tables
                continue;
            }
            if (tableNameStr.size() && tableName != tableNameStr) {
                continue;
            }

            std::auto_ptr<mongo::DBClientCursor> cursor =
                _connHandle->query(*tableIt);

            int columnNum = 1;
            while (cursor->more()) {
                mongo::BSONObj::iterator fieldIt = cursor->next().begin();
                while(fieldIt.more()) {
                    mongo::BSONElement elem = fieldIt.next();
                    SQLSMALLINT dataType = mapMongoToODBCDataType(elem.type());
                    _resultSet.push_back(std::list<Result>());
                    std::list<Result>& results = _resultSet.back();
                    results.push_back("NULL");
                    results.push_back(*it);
                    results.push_back(tableName);
                    results.push_back(elem.fieldName());
                    results.push_back(dataType);
                    results.push_back(dataTypeName(dataType));
                    results.push_back(columnSize(dataType));
                    results.push_back(bufferLength(dataType));
                    results.push_back(decimalDigits(dataType));
                    results.push_back(numPercRadix(dataType));
                    results.push_back((SQLSMALLINT)SQL_NULLABLE);
                    results.push_back("");
                    results.push_back("NULL");
                    results.push_back(mapODBCDataTypeToSQLDataType(dataType));
                    results.push_back(getDatetimeSubcode(dataType));
                    results.push_back(maxCharLen(dataType));
                    results.push_back(columnNum++);
                    results.push_back("\"YES\"");
                }
                break;
            }
        }
    }
    return SQL_SUCCESS;
}

SQLRETURN StatementHandle::sqlExec(SQLCHAR *query,
                                   SQLINTEGER queryLen)
{
    _resultSet.clear();
    _rowIdx = -1;
    SQLStatement stmt;
    std::string queryStr;
    if (queryLen == SQL_NTS) {
        queryStr.assign((char *)query);
    } else {
        queryStr.assign((char *)query, (int)queryLen);
    }
    std::string::const_iterator queryBegin = queryStr.begin();
    std::string::const_iterator queryEnd = queryStr.end();
    bool parseRc = boost::spirit::qi::phrase_parse(queryBegin,
                                                   queryEnd,
                                                   _parser,
                                                   boost::spirit::ascii::space,
                                                   stmt);
    if (!parseRc) {
        return SQL_ERROR;
    }

    SQLSelectStatement& selectStmt = stmt;

    if (!selectStmt._whereClause) {
        // set to a blank query
        selectStmt._whereClause = mongo::Query();
    }

    try {
    _cursor = _connHandle->query(selectStmt._tableRefList[0],
                                 *selectStmt._whereClause);
    } catch (mongo::AssertionException& ex) {
        return SQL_ERROR;
    }

    if (!_cursor.get()) {
        return SQL_ERROR;
    }

    std::vector<mongo::BSONObj> firstRows;
    _cursor->peek(firstRows, 1);
    if (!firstRows.size()) {
        // 0 results
        return SQL_SUCCESS;
    }
    mongo::BSONObj::iterator fieldIt = firstRows[0].begin();
    while(fieldIt.more()) {
        mongo::BSONElement elem = fieldIt.next();
        mongo::BSONType dataType = elem.type();
        _cursorColumns.push_back(std::make_pair(elem.fieldName(), dataType));
    }

    return SQL_SUCCESS;
}
    
SQLRETURN StatementHandle::sqlNumResultCols(SQLSMALLINT *numColumns)
{
    if (_cursor.get()) {
        *numColumns = _cursorColumns.size();
    } else {
        if (0 == _resultSet.size()) {
            return SQL_ERROR;
        }
        *numColumns = (SQLSMALLINT)_resultSet[0].size();
    }
    return SQL_SUCCESS;
}

SQLRETURN StatementHandle::sqlFetch()
{
    if (_cursor.get()) {
        if (!_cursor->more()) {
            return SQL_NO_DATA;
        }
        _row = _cursor->next();
    } else {
        ++_rowIdx;
        if (_rowIdx >= _resultSet.size()) {
            return SQL_NO_DATA;
        }
    }

    return SQL_SUCCESS;
}

SQLRETURN StatementHandle::sqlGetData(SQLUSMALLINT columnNum,
                     SQLSMALLINT type,
                     SQLPOINTER valuePtr,
                     SQLLEN len,
                     SQLLEN *lenPtr)
{
    if (_cursor.get()) {
        if (columnNum > _cursorColumns.size()) {
            return SQL_ERROR;
        }
        std::pair<std::string, mongo::BSONType>& field =
            _cursorColumns[columnNum - 1];
        switch(type) {
          case SQL_C_CHAR: {
          } break;
          case SQL_C_ULONG: {
              *((unsigned long int *)valuePtr) = _row.getIntField(field.first.c_str());
          } break;
          default: {
            return SQL_ERROR;
          } break;
        }
    } else {
        std::list<Result>::const_iterator it =
            _resultSet[_rowIdx].begin();
        for (int i = 1; i < columnNum; ++i) {
            ++it;
        }

        switch(type) {
          case SQL_C_CHAR: {
            const std::string& str = boost::get<std::string>(*it);
            int copyLen = (len > (str.size() - 1) ? str.size() : len - 1);
            strncpy((char *)valuePtr, str.c_str(), copyLen);
            ((char *)valuePtr)[copyLen] = '\0';
            *lenPtr = copyLen + 1;
          } break;
          default: {
            return SQL_ERROR;
          } break;
        }
    }

    return SQL_SUCCESS;
}

} // close mongoodbc namespace

