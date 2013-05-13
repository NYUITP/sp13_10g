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

        SQLRETURN ret;
        // allocate environment handle
        ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_endHandle);
        ASSERT_TRUE(SQL_SUCCEEDED(ret));

        // ask for ODBC 3.0
        ret = SQLSetEnvAttr(_endHandle, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
        ASSERT_TRUE(SQL_SUCCEEDED(ret));

        // allocate a connection handle
        ret = SQLAllocHandle(SQL_HANDLE_DBC, _endHandle, &_dbHandle);
        ASSERT_TRUE(SQL_SUCCEEDED(ret));

        SQLCHAR outstr[1024];
        SQLSMALLINT outstrlen;
        SQLCHAR *dsn = (SQLCHAR *)"DSN=MongoDB";
        // connect to the mongodb driver
        ret = SQLDriverConnect(_dbHandle, NULL, dsn ,SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_COMPLETE);
        ASSERT_TRUE(SQL_SUCCEEDED(ret));

        ret = SQLAllocHandle(SQL_HANDLE_STMT, _dbHandle, &_stmtHandle);
        ASSERT_TRUE(SQL_SUCCEEDED(ret));

    }

    virtual void TearDown()
    {
        SQLRETURN ret;
        // free statement handle
        ret = SQLFreeHandle(SQL_HANDLE_STMT, _stmtHandle);
        EXPECT_TRUE(SQL_SUCCEEDED(ret));

        // free connection handle
        ret = SQLFreeHandle(SQL_HANDLE_DBC, _dbHandle);
        EXPECT_TRUE(SQL_SUCCEEDED(ret));

        // free environment handle
        ret = SQLFreeHandle(SQL_HANDLE_ENV, _endHandle);
        EXPECT_TRUE(SQL_SUCCEEDED(ret));
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
            std::map<std::string, std::set<std::string> >::iterator it =
                _dbs.insert(std::make_pair(dbNameStream.str(), std::set<std::string>())).first;
            std::set<std::string>& collections = it->second;
            for (int j = 0; j < NUM_COLLECTIONS_PER_DB; ++j) {
                std::stringstream collectionNameStream;
                collectionNameStream << "collection"
                                     << i
                                     << j;
                collections.insert(collectionNameStream.str());
                ASSERT_NO_THROW(_conn.createCollection(
                                    dbNameStream.str() +
                                    '.' +
                                    collectionNameStream.str()));
            }
        }
    }

    virtual void TearDown()
    {
        for (std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
             it != _dbs.end();
             ++it) {
            ASSERT_NO_THROW(_conn.dropDatabase(it->first));
        }

        ODBCIntfTest::TearDown();
    }

    std::map<std::string, std::set<std::string> > _dbs;
};

TEST_F(SQLTablesTest, Breathing)
{
    SQLTables(_stmtHandle, NULL, 0, NULL, 0, NULL, 0, (SQLCHAR*)"TABLE", SQL_NTS);
}

TEST_F(SQLTablesTest, AllDbs)
{
    SQLRETURN ret = SQLTables(_stmtHandle, NULL, 0, NULL, 0, NULL, 0, (SQLCHAR*)"TABLE", SQL_NTS);
    ASSERT_TRUE(SQL_SUCCEEDED(ret));

    SQLSMALLINT numCols;
    ret = SQLNumResultCols(_stmtHandle, &numCols);
    EXPECT_TRUE(SQL_SUCCEEDED(ret));
    EXPECT_EQ(5, numCols);

    int numResults = 0;
    char schemaName[256];
    char tableName[256];
    while(SQL_SUCCEEDED(ret = SQLFetch(_stmtHandle))) {
        SQLLEN len;
        ret = SQLGetData(_stmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)schemaName, 256, &len);
        EXPECT_TRUE(SQL_SUCCEEDED(ret));
        std::map<std::string, std::set<std::string> >::const_iterator schemaIt =
            _dbs.find(schemaName);
        EXPECT_NE(_dbs.end(), schemaIt);
        if (_dbs.end() == schemaIt) {
            continue;
        }
        ret = SQLGetData(_stmtHandle, 3, SQL_C_CHAR, (SQLPOINTER)tableName, 256, &len);
        EXPECT_TRUE(SQL_SUCCEEDED(ret));
        std::set<std::string>::const_iterator tableIt = schemaIt->second.find(tableName);
        EXPECT_NE(schemaIt->second.end(), tableIt);
        ++numResults;
    }
    EXPECT_EQ(25, numResults);
}

TEST_F(SQLTablesTest, AllTablesInDb)
{
    for (std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
         it != _dbs.end();
         ++it) {
        const std::string& dbName = it->first;
        SQLRETURN ret =
            SQLTables(_stmtHandle,
                      NULL,
                      0,
                      (SQLCHAR*)dbName.c_str(),
                      SQL_NTS,
                      NULL,
                      0,
                      (SQLCHAR*)"TABLE",
                      SQL_NTS);
        ASSERT_TRUE(SQL_SUCCEEDED(ret));

        SQLSMALLINT numCols;
        ret = SQLNumResultCols(_stmtHandle, &numCols);
        EXPECT_TRUE(SQL_SUCCEEDED(ret));
        EXPECT_EQ(5, numCols);

        int numResults = 0;
        char schemaName[256];
        char tableName[256];
        while(SQL_SUCCEEDED(ret = SQLFetch(_stmtHandle))) {
            SQLLEN len;
            ret = SQLGetData(_stmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)schemaName, 256, &len);
            EXPECT_TRUE(SQL_SUCCEEDED(ret));
            std::map<std::string, std::set<std::string> >::const_iterator schemaIt =
                _dbs.find(schemaName);
            EXPECT_NE(_dbs.end(), schemaIt);
            if (_dbs.end() == schemaIt) {
                continue;
            }
            ret = SQLGetData(_stmtHandle, 3, SQL_C_CHAR, (SQLPOINTER)tableName, 256, &len);
            EXPECT_TRUE(SQL_SUCCEEDED(ret));
            std::set<std::string>::const_iterator tableIt = schemaIt->second.find(tableName);
            EXPECT_NE(schemaIt->second.end(), tableIt);
            ++numResults;
        }
        EXPECT_EQ(5, numResults);
    }
}

TEST_F(SQLTablesTest, FilterSingleTable)
{
    std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
    for (; it != _dbs.end(); ++it) {
        std::set<std::string>::const_iterator colIt = it->second.begin();
        for (; colIt != it->second.end(); ++colIt) {
            std::stringstream collectionNameStream;
            collectionNameStream << it->first
                                 << '.'
                                 << *colIt;
            SQLRETURN ret =
                SQLTables(_stmtHandle,
                          NULL,
                          0,
                          (SQLCHAR*)it->first.c_str(),
                          SQL_NTS,
                          (SQLCHAR*)colIt->c_str(),
                          SQL_NTS,
                          (SQLCHAR*)"TABLE",
                          SQL_NTS);
            ASSERT_TRUE(SQL_SUCCEEDED(ret));

            SQLSMALLINT numCols;
            ret = SQLNumResultCols(_stmtHandle, &numCols);
            EXPECT_TRUE(SQL_SUCCEEDED(ret));
            EXPECT_EQ(5, numCols);

            int numResults = 0;
            char schemaName[256];
            char tableName[256];
            while(SQL_SUCCEEDED(ret = SQLFetch(_stmtHandle))) {
                SQLLEN len;
                ret = SQLGetData(_stmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)schemaName, 256, &len);
                EXPECT_TRUE(SQL_SUCCEEDED(ret));
                std::map<std::string, std::set<std::string> >::const_iterator schemaIt =
                    _dbs.find(schemaName);
                EXPECT_NE(_dbs.end(), schemaIt);
                if (_dbs.end() == schemaIt) {
                    continue;
                }
                ret = SQLGetData(_stmtHandle, 3, SQL_C_CHAR, (SQLPOINTER)tableName, 256, &len);
                EXPECT_TRUE(SQL_SUCCEEDED(ret));
                std::set<std::string>::const_iterator tableIt = schemaIt->second.find(tableName);
                EXPECT_NE(schemaIt->second.end(), tableIt);
                ++numResults;
            }
            EXPECT_EQ(1, numResults);
        }
    }
}

class SQLColumnsTest : public SQLTablesTest {
  protected:
    virtual void SetUp()
    {
        SQLTablesTest::SetUp();

        std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
        for (; it != _dbs.end(); ++it) {
            std::set<std::string>::const_iterator colIt = it->second.begin();
            for (; colIt != it->second.end(); ++colIt) {
                std::stringstream collectionNameStream;
                collectionNameStream << it->first
                                     << '.'
                                     << *colIt;
                mongo::BSONObjBuilder builder;
                builder.append("a", 1);
                builder.append("b", 2);
            }
        }
    }

    virtual void TearDown()
    {
        SQLTablesTest::TearDown();
    }
};

TEST_F(SQLColumnsTest, Breathing)
{
    std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
    for (; it != _dbs.end(); ++it) {
        std::set<std::string>::const_iterator colIt = it->second.begin();
        for (; colIt != it->second.end(); ++colIt) {
            std::stringstream collectionNameStream;
            collectionNameStream << it->first
                                 << '.'
                                 << *colIt;
            SQLColumns(_stmtHandle,
                       NULL,
                       0,
                       (SQLCHAR*)it->first.c_str(),
                       SQL_NTS,
                       (SQLCHAR*)colIt->c_str(),
                       SQL_NTS,
                       NULL,
                       0);
        }
    }
}

class SQLExecDirectTest : public SQLTablesTest {
  public:
    typedef std::map<std::string, int> Fields;
    typedef std::vector<Fields> Objects;
    typedef std::map<std::string, Objects> Collections;
    Collections _collections;

  protected:
    virtual void SetUp()
    {
        SQLTablesTest::SetUp();

        std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
        for (; it != _dbs.end(); ++it) {
            std::set<std::string>::const_iterator colIt = it->second.begin();
            for (; colIt != it->second.end(); ++colIt) {
                std::stringstream collectionNameStream;
                collectionNameStream << it->first
                                     << '.'
                                     << *colIt;
                std::string collectionName(collectionNameStream.str());
                Collections::iterator colIt = 
                    _collections.insert(std::make_pair(collectionName, Objects())).first;
                Objects& objs = colIt->second;
                objs.push_back(Fields());
                Fields& fields = objs.back();
                std::vector<mongo::BSONObj> bsonObjs;
                for (int i = 0; i < 5; ++i) {
                    mongo::BSONObjBuilder builder;
                    fields.insert(std::make_pair("a", 1));
                    builder.append("a", i);
                    fields.insert(std::make_pair("b", 2));
                    builder.append("b", i+10);
                    bsonObjs.push_back(builder.obj());
                }
                ASSERT_NO_THROW(_conn.insert(collectionName, bsonObjs));
            }
        }
    }

    virtual void TearDown()
    {
        SQLTablesTest::TearDown();
    }

};

TEST_F(SQLExecDirectTest, SELECT)
{
    std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
    for (; it != _dbs.end(); ++it) {
        std::set<std::string>::const_iterator colIt = it->second.begin();
        for (; colIt != it->second.end(); ++colIt) {
            std::stringstream queryStream;
            queryStream << "SELECT * FROM "
                        << it->first << '.' << *colIt;
            std::cout << queryStream.str() << std::endl;

            SQLRETURN ret = SQLExecDirect(_stmtHandle, (SQLCHAR *)queryStream.str().c_str(), SQL_NTS);
            EXPECT_EQ(SQL_SUCCESS, ret);
            int numResults = 0;
            int i = 0;
            while(SQL_SUCCEEDED(ret = SQLFetch(_stmtHandle))) {
                ++numResults;
                SQLLEN len;
                unsigned long int value;
                ret = SQLGetData(_stmtHandle, 1, SQL_C_ULONG, (SQLPOINTER)&value, sizeof(unsigned long int), &len);
                EXPECT_EQ(i, value);
                ret = SQLGetData(_stmtHandle, 2, SQL_C_ULONG, (SQLPOINTER)&value, sizeof(unsigned long int), &len);
                EXPECT_EQ(i+10, value);
                EXPECT_TRUE(SQL_SUCCEEDED(ret));
                ++i;
            }
            EXPECT_EQ(5, numResults);
        }
    }
}

TEST_F(SQLExecDirectTest, SELECT_WHERE)
{
    std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
    for (; it != _dbs.end(); ++it) {
        std::set<std::string>::const_iterator colIt = it->second.begin();
        for (; colIt != it->second.end(); ++colIt) {
            std::stringstream queryStream;
            queryStream << "SELECT * FROM "
                        << it->first << '.' << *colIt
                        << " WHERE a > 2";
            std::cout << queryStream.str() << std::endl;

            SQLRETURN ret = SQLExecDirect(_stmtHandle, (SQLCHAR *)queryStream.str().c_str(), SQL_NTS);
            EXPECT_EQ(SQL_SUCCESS, ret);
            int numResults = 0;
            int i = 3;
            while(SQL_SUCCEEDED(ret = SQLFetch(_stmtHandle))) {
                ++numResults;
                SQLLEN len;
                unsigned long int value;
                ret = SQLGetData(_stmtHandle, 1, SQL_C_ULONG, (SQLPOINTER)&value, sizeof(unsigned long int), &len);
                EXPECT_EQ(i, value);
                ret = SQLGetData(_stmtHandle, 2, SQL_C_ULONG, (SQLPOINTER)&value, sizeof(unsigned long int), &len);
                EXPECT_EQ(i+10, value);
                EXPECT_TRUE(SQL_SUCCEEDED(ret));
                ++i;
            }
            EXPECT_EQ(2, numResults);
        }
    }
}

TEST_F(SQLExecDirectTest, SELECT_WHERE_AND)
{
    std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
    for (; it != _dbs.end(); ++it) {
        std::set<std::string>::const_iterator colIt = it->second.begin();
        for (; colIt != it->second.end(); ++colIt) {
            std::stringstream queryStream;
            queryStream << "SELECT * FROM "
                        << it->first << '.' << *colIt
                        << " WHERE a > 1 AND b < 13";
            std::cout << queryStream.str() << std::endl;

            SQLRETURN ret = SQLExecDirect(_stmtHandle, (SQLCHAR *)queryStream.str().c_str(), SQL_NTS);
            EXPECT_EQ(SQL_SUCCESS, ret);
            int numResults = 0;
            int i = 2;
            while(SQL_SUCCEEDED(ret = SQLFetch(_stmtHandle))) {
                ++numResults;
                SQLLEN len;
                unsigned long int value;
                ret = SQLGetData(_stmtHandle, 1, SQL_C_ULONG, (SQLPOINTER)&value, sizeof(unsigned long int), &len);
                EXPECT_EQ(i, value);
                ret = SQLGetData(_stmtHandle, 2, SQL_C_ULONG, (SQLPOINTER)&value, sizeof(unsigned long int), &len);
                EXPECT_EQ(i+10, value);
                EXPECT_TRUE(SQL_SUCCEEDED(ret));
                ++i;
            }
            EXPECT_EQ(1, numResults);
        }
    }
}

TEST_F(SQLExecDirectTest, SELECT_WHERE_OR)
{
    std::map<std::string, std::set<std::string> >::const_iterator it = _dbs.begin();
    for (; it != _dbs.end(); ++it) {
        std::set<std::string>::const_iterator colIt = it->second.begin();
        for (; colIt != it->second.end(); ++colIt) {
            std::stringstream queryStream;
            queryStream << "SELECT * FROM "
                        << it->first << '.' << *colIt
                        << " WHERE a > 3 OR b < 11";
            std::cout << queryStream.str() << std::endl;

            SQLRETURN ret = SQLExecDirect(_stmtHandle, (SQLCHAR *)queryStream.str().c_str(), SQL_NTS);
            EXPECT_EQ(SQL_SUCCESS, ret);
            int numResults = 0;
            while(SQL_SUCCEEDED(ret = SQLFetch(_stmtHandle))) {
                ++numResults;
            }
            EXPECT_EQ(2, numResults);
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

