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

#include <odbcintf.h>

#include <connection_handle.h>
#include <environment_handle.h>

#include <iostream>
#include <assert.h>

SQLRETURN SQL_API
SQLGetDiagRec(SQLSMALLINT handleType,
              SQLHANDLE handle,
              SQLSMALLINT recordNum,
	          SQLCHAR *sqlstate,
              SQLINTEGER *nativeerr,
              SQLCHAR *msg,
	          SQLSMALLINT buflen,
              SQLSMALLINT *msglen)
{
    std::cout << "SQLGetDiagRec" << std::endl;
    return SQL_NO_DATA;
}

SQLRETURN SQL_API
SQLGetInfo(SQLHDBC connHandle,
           SQLUSMALLINT infoType,
           SQLPOINTER info,
           SQLSMALLINT buflen,
           SQLSMALLINT *msglen)
{
    std::cout << "SQLGetInfo" << std::endl;
    return SQL_ERROR;
}

SQLRETURN SQL_API
SQLConnect(SQLHDBC dbc, SQLCHAR *dsn, SQLSMALLINT dsnLen,
	   SQLCHAR *uid, SQLSMALLINT uidLen,
	   SQLCHAR *pwd, SQLSMALLINT pwdLen)
{
    std::cout << "SQLConnect" << std::endl;
    return SQL_SUCCESS;
}

SQLRETURN SQL_API
SQLDisconnect(SQLHDBC dbc)
{
    std::cout << "SQLDisconnect" << std::endl;
    return SQL_SUCCESS;
}

SQLRETURN SQL_API
SQLAllocHandle(SQLSMALLINT handleType,
               SQLHANDLE inputHandle,
               SQLHANDLE *outputHandle)
// 'handleType' is the handle handleType, SQL_HANDLE_DBC for connection
// TODO: SQL_HANDLE_STMT
// TODO: SQL_HANDLE_DESC
// 'input' is the environment handle
{
    std::cout << "SQLAllocHandle" << std::endl;
    if (SQL_HANDLE_ENV == handleType) {
        mongoodbc::EnvironmentHandle *env =
            new mongoodbc::EnvironmentHandle();
        std::cout << "Successfully allocated environemtn handle" << std::endl;
        *outputHandle = static_cast<SQLHANDLE> (env);
    }
    else if (SQL_HANDLE_DBC == handleType) {
        mongoodbc::ConnectionHandle *conn =
            new mongoodbc::ConnectionHandle();
        std::cout << "Successfully allocated connection handle" << std::endl;
        *outputHandle = static_cast<SQLHANDLE> (conn);
    }
    else if (SQL_HANDLE_STMT == handleType) {
        return SQL_ERROR;
    }
    else if (SQL_HANDLE_DESC == handleType) {
        return SQL_ERROR;
    }
    else {
        std::cerr << "Invalid handle handleType - "
                  << handleType
                  << std::endl;
        return SQL_ERROR;
    }
    return SQL_SUCCESS;
}

SQLRETURN SQL_API
SQLFreeHandle(SQLSMALLINT handleType, SQLHANDLE handle)
{
    std::cout << "SQLFreeHandle" << std::endl;

    if (SQL_HANDLE_ENV == handleType) {
        mongoodbc::EnvironmentHandle *env =
            static_cast<mongoodbc::EnvironmentHandle *> (handle);
            delete env;
            std::cout << "Successfully freed environemnt handle" << std::endl;
    }
    else if (SQL_HANDLE_DBC == handleType) {
        mongoodbc::ConnectionHandle *conn =
            static_cast<mongoodbc::ConnectionHandle *> (handle);
        delete conn;
        std::cout << "Successfully freed connection handle" << std::endl;
    }
    else if (SQL_HANDLE_STMT == handleType) {
        return SQL_ERROR;
    }
    else if (SQL_HANDLE_DESC == handleType) {
        return SQL_ERROR;
    }
    else {
        std::cerr << "Invalid handle handleType - "
                  << handleType
                  << std::endl;
        return SQL_ERROR;
    }

    return SQL_SUCCESS;
}

SQLRETURN SQL_API
SQLDriverConnect(SQLHDBC connectionHandle,
                 SQLHWND windowHandle,
		         SQLCHAR *inConnectString,
                 SQLSMALLINT inConnectStringLen,
		         SQLCHAR *connOut,
                 SQLSMALLINT outConnectString,
		         SQLSMALLINT *outConnectStringLen,
                 SQLUSMALLINT driverCompletion)
{
    std::cout << "SQLDriverConnect" << std::endl;
    return SQL_SUCCESS;
}


// FUNCTIONS BELOW THIS LINE ARE NOT IMPLEMENTED
extern "C" {
SQLRETURN SQL_API
SQLBulkOperations(SQLHSTMT stmt, SQLSMALLINT oper);

SQLRETURN SQL_API
SQLDataSources(SQLHENV env, SQLUSMALLINT dir, SQLCHAR *srvname,
	       SQLSMALLINT buflen1, SQLSMALLINT *lenp1,
	       SQLCHAR *desc, SQLSMALLINT buflen2, SQLSMALLINT *lenp2);

SQLRETURN SQL_API
SQLDrivers(SQLHENV env, SQLUSMALLINT dir, SQLCHAR *drvdesc,
	   SQLSMALLINT descmax, SQLSMALLINT *desclenp,
	   SQLCHAR *drvattr, SQLSMALLINT attrmax, SQLSMALLINT *attrlenp);

SQLRETURN SQL_API
SQLBrowseConnect(SQLHDBC dbc, SQLCHAR *connin, SQLSMALLINT conninLen,
		 SQLCHAR *connout, SQLSMALLINT connoutMax,
		 SQLSMALLINT *connoutLen);

SQLRETURN SQL_API
SQLPutData(SQLHSTMT stmt, SQLPOINTER data, SQLLEN len);

SQLRETURN SQL_API
SQLBindParameter(SQLHSTMT stmt, SQLUSMALLINT pnum, SQLSMALLINT iotype,
		 SQLSMALLINT buftype, SQLSMALLINT ptype, SQLULEN coldef,
		 SQLSMALLINT scale,
		 SQLPOINTER data, SQLLEN buflen, SQLLEN *len);

SQLRETURN SQL_API
SQLBindParam(SQLHSTMT stmt, SQLUSMALLINT pnum, SQLSMALLINT vtype,
	     SQLSMALLINT ptype, SQLULEN lenprec,
	     SQLSMALLINT scale, SQLPOINTER val,
	     SQLLEN *lenp);

SQLRETURN SQL_API
SQLNumParams(SQLHSTMT stmt, SQLSMALLINT *nparam);

SQLRETURN SQL_API
SQLParamData(SQLHSTMT stmt, SQLPOINTER *pind);

SQLRETURN SQL_API
SQLDescribeParam(SQLHSTMT stmt, SQLUSMALLINT pnum, SQLSMALLINT *dtype,
		 SQLULEN *size, SQLSMALLINT *decdigits, SQLSMALLINT *nullable);

SQLRETURN SQL_API
SQLSetParam(SQLHSTMT stmt, SQLUSMALLINT par, SQLSMALLINT type,
	    SQLSMALLINT sqltype, SQLULEN coldef,
	    SQLSMALLINT scale, SQLPOINTER val, SQLLEN *nval);

SQLRETURN SQL_API
SQLParamOptions(SQLHSTMT stmt, SQLULEN rows, SQLULEN *rowp);

SQLRETURN SQL_API
SQLGetDescField(SQLHDESC handle, SQLSMALLINT recno,
		SQLSMALLINT fieldid, SQLPOINTER value,
		SQLINTEGER buflen, SQLINTEGER *strlen);

SQLRETURN SQL_API
SQLSetDescField(SQLHDESC handle, SQLSMALLINT recno,
		SQLSMALLINT fieldid, SQLPOINTER value,
		SQLINTEGER buflen);

SQLRETURN SQL_API
SQLSetDescRec(SQLHDESC handle, SQLSMALLINT recno,
	      SQLSMALLINT type, SQLSMALLINT subtype,
	      SQLLEN len, SQLSMALLINT prec,
	      SQLSMALLINT scale, SQLPOINTER data,
	      SQLLEN *strlen, SQLLEN *indicator);

SQLRETURN SQL_API
SQLTablePrivileges(SQLHSTMT stmt,
		   SQLCHAR *catalog, SQLSMALLINT catalogLen,
		   SQLCHAR *schema, SQLSMALLINT schemaLen,
		   SQLCHAR *table, SQLSMALLINT tableLen);

SQLRETURN SQL_API
SQLColumnPrivileges(SQLHSTMT stmt,
		    SQLCHAR *catalog, SQLSMALLINT catalogLen,
		    SQLCHAR *schema, SQLSMALLINT schemaLen,
		    SQLCHAR *table, SQLSMALLINT tableLen,
		    SQLCHAR *column, SQLSMALLINT columnLen);

SQLRETURN SQL_API
SQLPrimaryKeys(SQLHSTMT stmt,
	       SQLCHAR *cat, SQLSMALLINT catLen,
	       SQLCHAR *schema, SQLSMALLINT schemaLen,
	       SQLCHAR *table, SQLSMALLINT tableLen);

SQLRETURN SQL_API
SQLSpecialColumns(SQLHSTMT stmt, SQLUSMALLINT id,
		  SQLCHAR *cat, SQLSMALLINT catLen,
		  SQLCHAR *schema, SQLSMALLINT schemaLen,
		  SQLCHAR *table, SQLSMALLINT tableLen,
		  SQLUSMALLINT scope, SQLUSMALLINT nullable);

SQLRETURN SQL_API
SQLForeignKeys(SQLHSTMT stmt,
	       SQLCHAR *PKcatalog, SQLSMALLINT PKcatalogLen,
	       SQLCHAR *PKschema, SQLSMALLINT PKschemaLen,
	       SQLCHAR *PKtable, SQLSMALLINT PKtableLen,
	       SQLCHAR *FKcatalog, SQLSMALLINT FKcatalogLen,
	       SQLCHAR *FKschema, SQLSMALLINT FKschemaLen,
	       SQLCHAR *FKtable, SQLSMALLINT FKtableLen);

SQLRETURN SQL_API
SQLEndTran(SQLSMALLINT type, SQLHANDLE handle, SQLSMALLINT comptype);

SQLRETURN SQL_API
SQLTransact(SQLHENV env, SQLHDBC dbc, SQLUSMALLINT type);

SQLRETURN SQL_API
SQLCopyDesc(SQLHDESC source, SQLHDESC target);

SQLRETURN SQL_API
SQLNativeSql(SQLHSTMT stmt, SQLCHAR *sqlin, SQLINTEGER sqlinLen,
	     SQLCHAR *sql, SQLINTEGER sqlMax, SQLINTEGER *sqlLen);

SQLRETURN SQL_API
SQLProcedures(SQLHSTMT stmt,
	      SQLCHAR *catalog, SQLSMALLINT catalogLen,
	      SQLCHAR *schema, SQLSMALLINT schemaLen,
	      SQLCHAR *proc, SQLSMALLINT procLen);

SQLRETURN SQL_API
SQLProcedureColumns(SQLHSTMT stmt,
		    SQLCHAR *catalog, SQLSMALLINT catalogLen,
		    SQLCHAR *schema, SQLSMALLINT schemaLen,
		    SQLCHAR *proc, SQLSMALLINT procLen,
		    SQLCHAR *column, SQLSMALLINT columnLen);

SQLRETURN SQL_API
SQLGetDiagField(SQLSMALLINT htype, SQLHANDLE handle, SQLSMALLINT recno,
		SQLSMALLINT id, SQLPOINTER info, 
		SQLSMALLINT buflen, SQLSMALLINT *stringlen);

SQLRETURN SQL_API
SQLGetStmtAttr(SQLHSTMT stmt, SQLINTEGER attr, SQLPOINTER val,
	       SQLINTEGER bufmax, SQLINTEGER *buflen);

SQLRETURN SQL_API
SQLSetStmtAttr(SQLHSTMT stmt, SQLINTEGER attr, SQLPOINTER val,
	       SQLINTEGER buflen);

SQLRETURN SQL_API
SQLGetStmtOption(SQLHSTMT stmt, SQLUSMALLINT opt, SQLPOINTER param);

SQLRETURN SQL_API
SQLSetPos(SQLHSTMT stmt, SQLSETPOSIROW row, SQLUSMALLINT op, SQLUSMALLINT lock);

SQLRETURN SQL_API
SQLSetScrollOptions(SQLHSTMT stmt, SQLUSMALLINT concur, SQLLEN rowkeyset,
		    SQLUSMALLINT rowset);

SQLRETURN SQL_API
SQLGetConnectAttr(SQLHDBC dbc, SQLINTEGER attr, SQLPOINTER val,
		  SQLINTEGER bufmax, SQLINTEGER *buflen);

SQLRETURN SQL_API
SQLSetConnectAttr(SQLHDBC dbc, SQLINTEGER attr, SQLPOINTER val,
		  SQLINTEGER len);

SQLRETURN SQL_API
SQLGetConnectOption(SQLHDBC dbc, SQLUSMALLINT opt, SQLPOINTER param);

SQLRETURN SQL_API
SQLSetConnectOption(SQLHDBC dbc, SQLUSMALLINT opt, SQLULEN param);

SQLRETURN SQL_API
SQLCancel(SQLHSTMT stmt);

SQLRETURN SQL_API
SQLGetCursorName(SQLHSTMT stmt, SQLCHAR *cursor, SQLSMALLINT buflen,
		 SQLSMALLINT *lenp);

SQLRETURN SQL_API
SQLSetCursorName(SQLHSTMT stmt, SQLCHAR *cursor, SQLSMALLINT len);

SQLRETURN SQL_API
SQLCloseCursor(SQLHSTMT stmt);

SQLRETURN SQL_API
SQLBindCol(SQLHSTMT stmt, SQLUSMALLINT col, SQLSMALLINT type,
	   SQLPOINTER val, SQLLEN max, SQLLEN *lenp);

SQLRETURN SQL_API
SQLTables(SQLHSTMT stmt,
	  SQLCHAR *cat, SQLSMALLINT catLen,
	  SQLCHAR *schema, SQLSMALLINT schemaLen,
	  SQLCHAR *table, SQLSMALLINT tableLen,
	  SQLCHAR *type, SQLSMALLINT typeLen);

SQLRETURN SQL_API
SQLColumns(SQLHSTMT stmt,
	   SQLCHAR *cat, SQLSMALLINT catLen,
	   SQLCHAR *schema, SQLSMALLINT schemaLen,
	   SQLCHAR *table, SQLSMALLINT tableLen,
	   SQLCHAR *col, SQLSMALLINT colLen);

SQLRETURN SQL_API
SQLGetTypeInfo(SQLHSTMT stmt, SQLSMALLINT sqltype);

SQLRETURN SQL_API
SQLStatistics(SQLHSTMT stmt, SQLCHAR *cat, SQLSMALLINT catLen,
	      SQLCHAR *schema, SQLSMALLINT schemaLen,
	      SQLCHAR *table, SQLSMALLINT tableLen,
	      SQLUSMALLINT itype, SQLUSMALLINT resv);

SQLRETURN SQL_API
SQLGetData(SQLHSTMT stmt, SQLUSMALLINT col, SQLSMALLINT type,
	   SQLPOINTER val, SQLLEN len, SQLLEN *lenp);

SQLRETURN SQL_API
SQLFetch(SQLHSTMT stmt);

SQLRETURN SQL_API
SQLFetchScroll(SQLHSTMT stmt, SQLSMALLINT orient, SQLLEN offset);

SQLRETURN SQL_API
SQLNumResultCols(SQLHSTMT stmt, SQLSMALLINT *ncols);

SQLRETURN SQL_API
SQLDescribeCol(SQLHSTMT stmt, SQLUSMALLINT col, SQLCHAR *name,
	       SQLSMALLINT nameMax, SQLSMALLINT *nameLen,
	       SQLSMALLINT *type, SQLULEN *size,
	       SQLSMALLINT *digits, SQLSMALLINT *nullable);

SQLRETURN SQL_API
SQLColAttributes(SQLHSTMT stmt, SQLUSMALLINT col, SQLUSMALLINT id,
		 SQLPOINTER val, SQLSMALLINT valMax, SQLSMALLINT *valLen,
		 SQLLEN *val2);

SQLRETURN SQL_API
SQLError(SQLHENV env, SQLHDBC dbc, SQLHSTMT stmt,
	 SQLCHAR *sqlState, SQLINTEGER *nativeErr,
	 SQLCHAR *errmsg, SQLSMALLINT errmax, SQLSMALLINT *errlen);

SQLRETURN SQL_API
SQLMoreResults(SQLHSTMT stmt);

SQLRETURN SQL_API
SQLPrepare(SQLHSTMT stmt, SQLCHAR *query, SQLINTEGER queryLen);

SQLRETURN SQL_API
SQLExecute(SQLHSTMT stmt);

SQLRETURN SQL_API
SQLExecDirect(SQLHSTMT stmt, SQLCHAR *query, SQLINTEGER queryLen);
}

SQLRETURN SQL_API
SQLBulkOperations(SQLHSTMT stmt, SQLSMALLINT oper)
{
    assert(0);
    std::cout << "SQLBulkOperations" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLDataSources(SQLHENV env, SQLUSMALLINT dir, SQLCHAR *srvname,
	       SQLSMALLINT buflen1, SQLSMALLINT *lenp1,
	       SQLCHAR *desc, SQLSMALLINT buflen2, SQLSMALLINT *lenp2)
{
    assert(0);
    std::cout << "SQLDataSources" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLDrivers(SQLHENV env, SQLUSMALLINT dir, SQLCHAR *drvdesc,
	   SQLSMALLINT descmax, SQLSMALLINT *desclenp,
	   SQLCHAR *drvattr, SQLSMALLINT attrmax, SQLSMALLINT *attrlenp)
{
    assert(0);
    std::cout << "SQLDrivers" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLBrowseConnect(SQLHDBC dbc, SQLCHAR *connin, SQLSMALLINT conninLen,
		 SQLCHAR *connout, SQLSMALLINT connoutMax,
		 SQLSMALLINT *connoutLen)
{
    assert(0);
    std::cout << "SQLBrowseConnect" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLPutData(SQLHSTMT stmt, SQLPOINTER data, SQLLEN len)
{
    assert(0);
    std::cout << "SQLPutData" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLBindParameter(SQLHSTMT stmt, SQLUSMALLINT pnum, SQLSMALLINT iotype,
		 SQLSMALLINT buftype, SQLSMALLINT ptype, SQLULEN coldef,
		 SQLSMALLINT scale,
		 SQLPOINTER data, SQLLEN buflen, SQLLEN *len)
{
    assert(0);
    std::cout << "SQLBindParameter" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLBindParam(SQLHSTMT stmt, SQLUSMALLINT pnum, SQLSMALLINT vtype,
	     SQLSMALLINT ptype, SQLULEN lenprec,
	     SQLSMALLINT scale, SQLPOINTER val,
	     SQLLEN *lenp)
{
    assert(0);
    std::cout << "SQLBindParam" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLNumParams(SQLHSTMT stmt, SQLSMALLINT *nparam)
{
    assert(0);
    std::cout << "SQLNumParams" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLParamData(SQLHSTMT stmt, SQLPOINTER *pind)
{
    assert(0);
    std::cout << "SQLParamData" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLDescribeParam(SQLHSTMT stmt, SQLUSMALLINT pnum, SQLSMALLINT *dtype,
		 SQLULEN *size, SQLSMALLINT *decdigits, SQLSMALLINT *nullable)
{
    assert(0);
    std::cout << "SQLDescribeParam" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSetParam(SQLHSTMT stmt, SQLUSMALLINT par, SQLSMALLINT type,
	    SQLSMALLINT sqltype, SQLULEN coldef,
	    SQLSMALLINT scale, SQLPOINTER val, SQLLEN *nval)
{
    assert(0);
    std::cout << "SQLSetParam" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLParamOptions(SQLHSTMT stmt, SQLULEN rows, SQLULEN *rowp)
{
    assert(0);
    std::cout << "SQLParamOptions" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetDescField(SQLHDESC handle, SQLSMALLINT recno,
		SQLSMALLINT fieldid, SQLPOINTER value,
		SQLINTEGER buflen, SQLINTEGER *strlen)
{
    assert(0);
    std::cout << "SQLGetDescField" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSetDescField(SQLHDESC handle, SQLSMALLINT recno,
		SQLSMALLINT fieldid, SQLPOINTER value,
		SQLINTEGER buflen)
{
    assert(0);
    std::cout << "SQLSetDescField" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetDescRec(SQLHDESC handle, SQLSMALLINT recno,
	      SQLCHAR *name, SQLSMALLINT buflen,
	      SQLSMALLINT *strlen, SQLSMALLINT *type,
	      SQLSMALLINT *subtype, SQLLEN *len,
	      SQLSMALLINT *prec, SQLSMALLINT *scale,
	      SQLSMALLINT *nullable)
{
    assert(0);
    std::cout << "SQLGetDescRec" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSetDescRec(SQLHDESC handle, SQLSMALLINT recno,
	      SQLSMALLINT type, SQLSMALLINT subtype,
	      SQLLEN len, SQLSMALLINT prec,
	      SQLSMALLINT scale, SQLPOINTER data,
	      SQLLEN *strlen, SQLLEN *indicator)
{
    assert(0);
    std::cout << "SQLSetDescRec" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLTablePrivileges(SQLHSTMT stmt,
		   SQLCHAR *catalog, SQLSMALLINT catalogLen,
		   SQLCHAR *schema, SQLSMALLINT schemaLen,
		   SQLCHAR *table, SQLSMALLINT tableLen)
{
    assert(0);
    std::cout << "SQLTablePrivileges" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLColumnPrivileges(SQLHSTMT stmt,
		    SQLCHAR *catalog, SQLSMALLINT catalogLen,
		    SQLCHAR *schema, SQLSMALLINT schemaLen,
		    SQLCHAR *table, SQLSMALLINT tableLen,
		    SQLCHAR *column, SQLSMALLINT columnLen)
{
    assert(0);
    std::cout << "SQLColumnPrivileges" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLPrimaryKeys(SQLHSTMT stmt,
	       SQLCHAR *cat, SQLSMALLINT catLen,
	       SQLCHAR *schema, SQLSMALLINT schemaLen,
	       SQLCHAR *table, SQLSMALLINT tableLen)
{
    assert(0);
    std::cout << "SQLPrimaryKeys" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSpecialColumns(SQLHSTMT stmt, SQLUSMALLINT id,
		  SQLCHAR *cat, SQLSMALLINT catLen,
		  SQLCHAR *schema, SQLSMALLINT schemaLen,
		  SQLCHAR *table, SQLSMALLINT tableLen,
		  SQLUSMALLINT scope, SQLUSMALLINT nullable)
{
    assert(0);
    std::cout << "SQLSpecialColumns" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLForeignKeys(SQLHSTMT stmt,
	       SQLCHAR *PKcatalog, SQLSMALLINT PKcatalogLen,
	       SQLCHAR *PKschema, SQLSMALLINT PKschemaLen,
	       SQLCHAR *PKtable, SQLSMALLINT PKtableLen,
	       SQLCHAR *FKcatalog, SQLSMALLINT FKcatalogLen,
	       SQLCHAR *FKschema, SQLSMALLINT FKschemaLen,
	       SQLCHAR *FKtable, SQLSMALLINT FKtableLen)
{
    assert(0);
    std::cout << "SQLForeignKeys" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLEndTran(SQLSMALLINT type, SQLHANDLE handle, SQLSMALLINT comptype)
{
    assert(0);
    std::cout << "SQLEndTran" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLTransact(SQLHENV env, SQLHDBC dbc, SQLUSMALLINT type)
{
    assert(0);
    std::cout << "SQLTransact" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLCopyDesc(SQLHDESC source, SQLHDESC target)
{
    assert(0);
    std::cout << "SQLCopyDesc" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLNativeSql(SQLHSTMT stmt, SQLCHAR *sqlin, SQLINTEGER sqlinLen,
	     SQLCHAR *sql, SQLINTEGER sqlMax, SQLINTEGER *sqlLen)
{
    assert(0);
    std::cout << "SQLNativeSql" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLProcedures(SQLHSTMT stmt,
	      SQLCHAR *catalog, SQLSMALLINT catalogLen,
	      SQLCHAR *schema, SQLSMALLINT schemaLen,
	      SQLCHAR *proc, SQLSMALLINT procLen)
{
    assert(0);
    std::cout << "SQLProcedures" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLProcedureColumns(SQLHSTMT stmt,
		    SQLCHAR *catalog, SQLSMALLINT catalogLen,
		    SQLCHAR *schema, SQLSMALLINT schemaLen,
		    SQLCHAR *proc, SQLSMALLINT procLen,
		    SQLCHAR *column, SQLSMALLINT columnLen)
{
    assert(0);
    std::cout << "SQLProcedureColumns" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetDiagField(SQLSMALLINT htype, SQLHANDLE handle, SQLSMALLINT recno,
		SQLSMALLINT id, SQLPOINTER info, 
		SQLSMALLINT buflen, SQLSMALLINT *stringlen)
{
    assert(0);
    std::cout << "SQLGetDiagField" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetStmtAttr(SQLHSTMT stmt, SQLINTEGER attr, SQLPOINTER val,
	       SQLINTEGER bufmax, SQLINTEGER *buflen)
{
    assert(0);
    std::cout << "SQLGetStmtAttr" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSetStmtAttr(SQLHSTMT stmt, SQLINTEGER attr, SQLPOINTER val,
	       SQLINTEGER buflen)
{
    assert(0);
    std::cout << "SQLSetStmtAttr" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetStmtOption(SQLHSTMT stmt, SQLUSMALLINT opt, SQLPOINTER param)
{
    assert(0);
    std::cout << "SQLGetStmtOption" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSetPos(SQLHSTMT stmt, SQLSETPOSIROW row, SQLUSMALLINT op, SQLUSMALLINT lock)
{
    assert(0);
    std::cout << "SQLSetPos" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSetScrollOptions(SQLHSTMT stmt, SQLUSMALLINT concur, SQLLEN rowkeyset,
		    SQLUSMALLINT rowset)
{
    assert(0);
    std::cout << "SQLSetScrollOptions" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetConnectAttr(SQLHDBC dbc, SQLINTEGER attr, SQLPOINTER val,
		  SQLINTEGER bufmax, SQLINTEGER *buflen)
{
    assert(0);
    std::cout << "SQLGetConnectAttr" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSetConnectAttr(SQLHDBC dbc, SQLINTEGER attr, SQLPOINTER val,
		  SQLINTEGER len)
{
    assert(0);
    std::cout << "SQLSetConnectAttr" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetConnectOption(SQLHDBC dbc, SQLUSMALLINT opt, SQLPOINTER param)
{
    assert(0);
    std::cout << "SQLGetConnectOption" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSetConnectOption(SQLHDBC dbc, SQLUSMALLINT opt, SQLULEN param)
{
    assert(0);
    std::cout << "SQLSetConnectOption" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLCancel(SQLHSTMT stmt)
{
    assert(0);
    std::cout << "SQLCancel" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetCursorName(SQLHSTMT stmt, SQLCHAR *cursor, SQLSMALLINT buflen,
		 SQLSMALLINT *lenp)
{
    assert(0);
    std::cout << "SQLGetCursorName" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLSetCursorName(SQLHSTMT stmt, SQLCHAR *cursor, SQLSMALLINT len)
{
    assert(0);
    std::cout << "SQLSetCursorName" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLCloseCursor(SQLHSTMT stmt)
{
    assert(0);
    std::cout << "SQLCloseCursor" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLBindCol(SQLHSTMT stmt, SQLUSMALLINT col, SQLSMALLINT type,
	   SQLPOINTER val, SQLLEN max, SQLLEN *lenp)
{
    assert(0);
    std::cout << "SQLBindCol" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLTables(SQLHSTMT stmt,
	  SQLCHAR *cat, SQLSMALLINT catLen,
	  SQLCHAR *schema, SQLSMALLINT schemaLen,
	  SQLCHAR *table, SQLSMALLINT tableLen,
	  SQLCHAR *type, SQLSMALLINT typeLen)
{
    assert(0);
    std::cout << "SQLTables" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLColumns(SQLHSTMT stmt,
	   SQLCHAR *cat, SQLSMALLINT catLen,
	   SQLCHAR *schema, SQLSMALLINT schemaLen,
	   SQLCHAR *table, SQLSMALLINT tableLen,
	   SQLCHAR *col, SQLSMALLINT colLen)
{
    assert(0);
    std::cout << "SQLColumns" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetTypeInfo(SQLHSTMT stmt, SQLSMALLINT sqltype)
{
    assert(0);
    std::cout << "SQLGetTypeInfo" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLStatistics(SQLHSTMT stmt, SQLCHAR *cat, SQLSMALLINT catLen,
	      SQLCHAR *schema, SQLSMALLINT schemaLen,
	      SQLCHAR *table, SQLSMALLINT tableLen,
	      SQLUSMALLINT itype, SQLUSMALLINT resv)
{
    assert(0);
    std::cout << "SQLStatistics" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLGetData(SQLHSTMT stmt, SQLUSMALLINT col, SQLSMALLINT type,
	   SQLPOINTER val, SQLLEN len, SQLLEN *lenp)
{
    assert(0);
    std::cout << "SQLGetData" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLFetch(SQLHSTMT stmt)
{
    assert(0);
    std::cout << "SQLFetch" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLFetchScroll(SQLHSTMT stmt, SQLSMALLINT orient, SQLLEN offset)
{
    assert(0);
    std::cout << "SQLFetchScroll" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLRowCount(SQLHSTMT stmt, SQLLEN *nrows)
{
    assert(0);
    std::cout << "SQLRowCount" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLNumResultCols(SQLHSTMT stmt, SQLSMALLINT *ncols)
{
    assert(0);
    std::cout << "SQLNumResultCols" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLDescribeCol(SQLHSTMT stmt, SQLUSMALLINT col, SQLCHAR *name,
	       SQLSMALLINT nameMax, SQLSMALLINT *nameLen,
	       SQLSMALLINT *type, SQLULEN *size,
	       SQLSMALLINT *digits, SQLSMALLINT *nullable)
{
    assert(0);
    std::cout << "SQLDescribeCol" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLColAttributes(SQLHSTMT stmt, SQLUSMALLINT col, SQLUSMALLINT id,
		 SQLPOINTER val, SQLSMALLINT valMax, SQLSMALLINT *valLen,
		 SQLLEN *val2)
{
    assert(0);
    std::cout << "SQLColAttributes" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLError(SQLHENV env, SQLHDBC dbc, SQLHSTMT stmt,
	 SQLCHAR *sqlState, SQLINTEGER *nativeErr,
	 SQLCHAR *errmsg, SQLSMALLINT errmax, SQLSMALLINT *errlen)
{
    assert(0);
    std::cout << "SQLError" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLMoreResults(SQLHSTMT stmt)
{
    assert(0);
    std::cout << "SQLMoreResults" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLPrepare(SQLHSTMT stmt, SQLCHAR *query, SQLINTEGER queryLen)
{
    assert(0);
    std::cout << "SQLPrepare" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLExecute(SQLHSTMT stmt)
{
    assert(0);
    std::cout << "SQLExecute" << std::endl;
    return 0;
}

SQLRETURN SQL_API
SQLExecDirect(SQLHSTMT stmt, SQLCHAR *query, SQLINTEGER queryLen)
{
    assert(0);
    std::cout << "SQLExecDirect" << std::endl;
    return 0;
}


