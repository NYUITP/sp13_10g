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

#include <sql.h>
#include <sqlext.h>

#include <iostream>

int
main(int arc, char **argv)
{
    SQLHENV envHandle;

    // allocate environment handle
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle);

    // ask for ODBC 3.0
    SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

    SQLHDBC dbHandle;

    // allocate a connection handle
    SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &dbHandle);

    SQLCHAR outstr[1024];
    SQLSMALLINT outstrlen;
    SQLCHAR *dsn = (SQLCHAR *)"DSN=MongoDB";
    // connect to the mongodb driver
    SQLRETURN ret = SQLDriverConnect(dbHandle, NULL, dsn ,SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_COMPLETE);

    if (SQL_SUCCEEDED(ret)) {
        std::cout << "Successfully connected - returned connection string: "
                  << outstr
                  << std::endl;
    } else {
        std::cerr << "Failed to connect - ret: "
                  << ret
                  << std::endl;
        SQLCHAR error[1024];
        SQLSMALLINT errorLen;
        SQLCHAR state[7];
        SQLINTEGER native;
        SQLRETURN diagRet;
        SQLINTEGER i = 0;
        do {
            diagRet = SQLGetDiagRec(SQL_HANDLE_DBC, dbHandle, ++i, state, &native, error, sizeof(error), &errorLen);
            if (SQL_SUCCEEDED(diagRet)) {
                std::cerr << "Diag " << i << ": '"
                          << error
                          << "'"
                          << std::endl;
                break;
            }
        } while(SQL_SUCCESS == diagRet);
        // free connection handle
        SQLFreeHandle(SQL_HANDLE_DBC, dbHandle);

        // free environment handle
        SQLFreeHandle(SQL_HANDLE_ENV, envHandle);
        return -1;
    }

    SQLHSTMT stmtHandle;
    SQLAllocHandle(SQL_HANDLE_STMT, dbHandle, &stmtHandle);

    SQLExecDirect(stmtHandle, (SQLCHAR*)"select * from test", SQL_NTS);

    // free statement handle
    SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);

    // free connection handle
    SQLFreeHandle(SQL_HANDLE_DBC, dbHandle);

    // free environment handle
    SQLFreeHandle(SQL_HANDLE_ENV, envHandle);
}
