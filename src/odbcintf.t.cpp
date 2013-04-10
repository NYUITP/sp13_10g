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

#include <mongo/client/dbclient.h>

#include <gtest/gtest.h>

#include <cstdlib>
#include <iostream>
#include <string>


namespace {
    char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
} // close unnamed namespace

class ODBCIntfTest : public ::testing::Test {
  protected:
    ODBCIntfTest()
    {
        for (int i = 0; i < 10; ++i) {
            _dbName += alpha[rand() % (sizeof(alpha) - 1)];
        }
        _dbName += "testdb";
    }

    virtual void SetUp()
    {

        ASSERT_NO_THROW(_conn.connect("localhost"));

        ASSERT_NO_THROW(_conn.createCollection(_dbName + ".testcollection"));

        // allocate environment handle
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_endHandle);

        // ask for ODBC 3.0
        SQLSetEnvAttr(_endHandle, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

        // allocate a connection handle
        SQLAllocHandle(SQL_HANDLE_DBC, _endHandle, &_dbHandle);

        SQLCHAR outstr[1024];
        SQLSMALLINT outstrlen;
        SQLCHAR *dsn = (SQLCHAR *)"DSN=MongoDB";
        // connect to the mongodb driver
        SQLRETURN ret = SQLDriverConnect(_dbHandle, NULL, dsn ,SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_COMPLETE);

        if (SQL_SUCCEEDED(ret)) {
            std::cout << "Successfully connected - returned connection string: "
                      << (outstrlen > 0 ? outstr : (SQLCHAR *)"")
                      << std::endl;
        }

        SQLAllocHandle(SQL_HANDLE_STMT, _dbHandle, &_stmtHandle);

    }

    virtual void TearDown()
    {
        // free statement handle
        SQLFreeHandle(SQL_HANDLE_STMT, _stmtHandle);

        // free connection handle
        SQLFreeHandle(SQL_HANDLE_DBC, _dbHandle);

        // free environment handle
        SQLFreeHandle(SQL_HANDLE_ENV, _endHandle);

        // Drop Test database
        ASSERT_NO_THROW(_conn.dropDatabase(_dbName));
    }
    
    std::string _dbName;
    mongo::DBClientConnection _conn;

    SQLHENV _endHandle;
    SQLHDBC _dbHandle;
    SQLHSTMT _stmtHandle;
};

class SQLTablesTest : public ODBCIntfTest {
  protected:
    enum {
        NUM_DBS = 5,
        NUM_COLLECTIONS_PER_DB = 5
    };

    virtual void SetUp()
    {
        ODBCIntfTest::SetUp();

        for (int i = 0; i < NUM_DBS; ++i) {
            std::stringstream dbNameStream;
            dbNameStream << _dbName
                         << "db"
                         << i;
            _dbs.push_back(dbNameStream.str());
            for (int j = 0; j < NUM_COLLECTIONS_PER_DB; ++j) {
                std::stringstream collectionNameStream;
                collectionNameStream << "collection"
                                     << j;
                _collections.push_back(collectionNameStream.str());
                ASSERT_NO_THROW(_conn.createCollection(_dbs.back() + '.' + _collections.back()));
            }
        }
    }

    virtual void TearDown()
    {
        for (int i = 0; i < NUM_DBS; ++i) {
            ASSERT_NO_THROW(_conn.dropDatabase(_dbs[i]));
        }

        ODBCIntfTest::TearDown();
    }

    std::vector<std::string> _dbs;
    std::vector<std::string> _collections;
};

TEST_F(SQLTablesTest, Breathing) {
    SQLTables(_stmtHandle, NULL, 0, NULL, 0, NULL, 0, (SQLCHAR*)"TABLE", SQL_NTS);
}

TEST_F(SQLTablesTest, AllDbs) {
    SQLRETURN ret = SQLTables(_stmtHandle, NULL, 0, NULL, 0, NULL, 0, (SQLCHAR*)"TABLE", SQL_NTS);
    ASSERT_TRUE(SQL_SUCCEEDED(ret));

    SQLSMALLINT numCols;
    ret = SQLNumResultCols(_stmtHandle, &numCols);
    ASSERT_TRUE(SQL_SUCCEEDED(ret));
    EXPECT_EQ(5, numCols);

    int numResults = 0;
    char schemaName[256];
    char columnName[256];
    while(SQL_SUCCEEDED(ret = SQLFetch(_stmtHandle))) {
        SQLLEN len;
        SQLGetData(_stmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)schemaName, 256, &len);
        std::cout << "schemaName: " << schemaName << std::endl;
        SQLGetData(_stmtHandle, 3, SQL_C_CHAR, (SQLPOINTER)columnName, 256, &len);
        std::cout << "columnName: " << columnName << std::endl;
        ++numResults;
    }
    EXPECT_EQ(26, numResults);
}

TEST_F(SQLTablesTest, AllTablesInDb) {
    for (size_t i = 0; i < _dbs.size(); ++i) {
        const std::string& dbName = _dbs[i];
        SQLRETURN ret =
            SQLTables(_stmtHandle, NULL, 0, (SQLCHAR*)dbName.c_str(), SQL_NTS, NULL, 0, (SQLCHAR*)"TABLE", SQL_NTS);
        ASSERT_TRUE(SQL_SUCCEEDED(ret));

        SQLSMALLINT numCols;
        ret = SQLNumResultCols(_stmtHandle, &numCols);
        ASSERT_TRUE(SQL_SUCCEEDED(ret));
        EXPECT_EQ(5, numCols);

        int numResults = 0;
        char schemaName[256];
        char columnName[256];
        while(SQL_SUCCEEDED(ret = SQLFetch(_stmtHandle))) {
            SQLLEN len;
            SQLGetData(_stmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)schemaName, 256, &len);
            std::cout << "schemaName: " << schemaName << std::endl;
            SQLGetData(_stmtHandle, 3, SQL_C_CHAR, (SQLPOINTER)columnName, 256, &len);
            std::cout << "columnName: " << columnName << std::endl;
            ++numResults;
        }
        EXPECT_EQ(5, numResults);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

