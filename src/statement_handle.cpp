
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

#include <statement_handle.h>
#include <connection_handle.h>

#include <boost/variant/get.hpp>

#include <string.h>

namespace mongoodbc {

StatementHandle::StatementHandle(ConnectionHandle *connHandle)
    : _connHandle(connHandle)
{
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
    
SQLRETURN StatementHandle::sqlNumResultCols(SQLSMALLINT *numColumns)
{
    *numColumns = 0;
    if (0 == _resultSet.size()) {
        return SQL_ERROR;
    }
    *numColumns = (SQLSMALLINT)_resultSet[0].size();
    return SQL_SUCCESS;
}

SQLRETURN StatementHandle::sqlFetch()
{
    ++_rowIdx;
    if (_rowIdx >= _resultSet.size()) {
        return SQL_NO_DATA;
    }

    return SQL_SUCCESS;
}

SQLRETURN StatementHandle::sqlGetData(SQLUSMALLINT columnNum,
                     SQLSMALLINT type,
                     SQLPOINTER valuePtr,
                     SQLLEN len,
                     SQLLEN *lenPtr)
{
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

    return SQL_SUCCESS;
}

} // close mongoodbc namespace

