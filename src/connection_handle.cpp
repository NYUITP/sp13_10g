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

#include <connection_handle.h>
#include <environment_handle.h>

namespace mongoodbc {

ConnectionHandle::ConnectionHandle(EnvironmentHandle *envHandle)
    : _envHandle(envHandle)
{
}

int ConnectionHandle::connect()
{
    try {
        _conn.connect("localhost");
        std::cerr << "Connection to mongoDB successful" << std::endl;
    } catch (const mongo::DBException &e) {
        std::cerr << "Connection to mongoDB failed" << std::endl;
        return -1;
    }

    return 0;
}

int ConnectionHandle::getDbNames(std::list<std::string> *dbs)
{
    try {
        *dbs = _conn.getDatabaseNames();
    } catch (const mongo::DBException &e) {
        std::cerr << "getDbNames failed" << std::endl;
        return -1;
    }

    return 0;
}

int ConnectionHandle::getCollectionNames(const std::string& db,
                                         std::list<std::string> *collections)
{
    try {
        *collections = _conn.getCollectionNames(db);
    } catch (const mongo::DBException &e) {
        std::cerr << "getCollectionNames "
                  << " for db "
                  << db
                  << " failed"
                  << std::endl;
        return -1;
    }

    return 0;
}

std::auto_ptr<mongo::DBClientCursor> ConnectionHandle::query(
    const std::string& collection,
    mongo::Query query,
    int numToReturn,
    int numToSkip,
    const mongo::BSONObj *fieldsToReturn,
    int queryOptions,
    int batchSize)
{
    return _conn.query(collection,
                       query,
                       numToReturn,
                       numToSkip,
                       fieldsToReturn,
                       queryOptions,
                       batchSize);
}

} // close mongoodbc namespace

