#pragma once
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
#ifndef MONGOODBC_STATEMENT_HANDLE_H_
#define MONGOODBC_STATEMENT_HANDLE_H_

#include "sql_parser.h"

#include <sql.h>
#include <sqlext.h>

#include <mongo/client/dbclient.h>
#include <mongo/bson/bsonobj.h>

#include <boost/variant/variant.hpp>

#include <list>
#include <string>
#include <vector>

namespace mongoodbc {

class ConnectionHandle;

/*
* Class implementing an ODBC statement handle.
*/
class StatementHandle {
    // TYPES
    // Result type for an column entry
    typedef boost::variant<std::string, SQLSMALLINT, SQLINTEGER> Result;

    // INSTANCE DATA
    // The connection handle from which this handle was created,
    // held, not owned
    ConnectionHandle *_connHandle;

    // '_resultSet' is used to store results for operations that do not retrieve data by
    // mongoDB cursor (i.e. SQLTables, SQLColumns).
    std::vector<std::list<Result> > _resultSet;
    int _rowIdx;

    // cursor used to store the result of a datbase query that is retrurned incrementally
    std::auto_ptr<mongo::DBClientCursor> _cursor;

    // parser for SQL statements
    SQLParser<std::string::const_iterator> _parser;

    SQLSMALLINT mapMongoToODBCDataType(mongo::BSONType type);
    const char *dataTypeName(SQLSMALLINT type);
    SQLINTEGER columnSize(SQLSMALLINT type);
    SQLINTEGER bufferLength(SQLSMALLINT type);
    SQLSMALLINT decimalDigits(SQLSMALLINT type);
    SQLSMALLINT numPercRadix(SQLSMALLINT type);
    SQLSMALLINT mapODBCDataTypeToSQLDataType(SQLSMALLINT type);
    SQLSMALLINT getDatetimeSubcode(SQLSMALLINT type);
    SQLINTEGER maxCharLen(SQLSMALLINT type);

  public:
    StatementHandle(ConnectionHandle *connHandle);

    SQLRETURN sqlTables(SQLCHAR *catalogName,
                        SQLSMALLINT catalogNameLen,
                        SQLCHAR *schemaName,
                        SQLSMALLINT schemaNameLen,
                        SQLCHAR *tableName,
                        SQLSMALLINT tableNameLen,
                        SQLCHAR *tableType,
                        SQLSMALLINT tableTypeLen);

    SQLRETURN sqlColumns(SQLCHAR *catalogName,
                         SQLSMALLINT catalogNameLen,
                         SQLCHAR *schemaName,
                         SQLSMALLINT schemaNameLen,
                         SQLCHAR *tableName,
                         SQLSMALLINT tableNameLen,
                         SQLCHAR *columnName,
                         SQLSMALLINT columnNameLen);

    SQLRETURN sqlExec(SQLCHAR *query,
                      SQLINTEGER queryLen);

    SQLRETURN sqlNumResultCols(SQLSMALLINT *numColumns);

    SQLRETURN sqlFetch();

    SQLRETURN sqlGetData(SQLUSMALLINT columnNum,
                         SQLSMALLINT type,
	                     SQLPOINTER valuePtr,
                         SQLLEN len,
                         SQLLEN *lenPtr);
};

} // close mongoodbc namespace

#endif

