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

#ifndef MONGOODBC_ODBCINTF_H_
#define MONGOODBC_ODBCINTF_H_

#include <sql.h>
#include <sqlext.h>

extern "C" {

SQLRETURN SQL_API
SQLGetEnvAttr(SQLHENV environmentHandle,
              SQLINTEGER attribute,
              SQLPOINTER valuePtr,
              SQLINTEGER bufferLength,
              SQLINTEGER *stringLenPtr);

SQLRETURN SQL_API
SQLGetDiagRec(SQLSMALLINT handleType,
              SQLHANDLE handle,
              SQLSMALLINT recordNum,
	          SQLCHAR *sqlstate,
              SQLINTEGER *nativeerr,
              SQLCHAR *msg,
	          SQLSMALLINT buflen,
              SQLSMALLINT *msglen);

SQLRETURN SQL_API
SQLGetInfo(SQLHDBC connHandle,
           SQLUSMALLINT infoType,
           SQLPOINTER infoValuePtr,
           SQLSMALLINT buflen,
           SQLSMALLINT *msglen);

SQLRETURN SQL_API
SQLDisconnect(SQLHDBC dbc);

SQLRETURN SQL_API
SQLAllocHandle(SQLSMALLINT handleType,
               SQLHANDLE inputHandle,
               SQLHANDLE *outputHandle);

SQLRETURN SQL_API
SQLFreeHandle(SQLSMALLINT handleType, SQLHANDLE handle);

SQLRETURN SQL_API
SQLDriverConnect(SQLHDBC connectionHandle,
                 SQLHWND windowHandle,
		         SQLCHAR *inConnectString,
                 SQLSMALLINT inConnectStringLen,
		         SQLCHAR *connOut,
                 SQLSMALLINT outConnectString,
		         SQLSMALLINT *outConnectStringLen,
                 SQLUSMALLINT driverCompletion);

SQLRETURN SQL_API
SQLTables(SQLHSTMT statementHandle,
	      SQLCHAR *catalogName,
          SQLSMALLINT catalogNameLen,
	      SQLCHAR *schemaName,
          SQLSMALLINT schemaNameLen,
	      SQLCHAR *tableName,
          SQLSMALLINT tableNameLen,
	      SQLCHAR *tableType,
          SQLSMALLINT tableTypeLen);

SQLRETURN SQL_API
SQLExecDirect(SQLHSTMT statementHandle,
              SQLCHAR *query,
              SQLINTEGER queryLen);
}

#endif
