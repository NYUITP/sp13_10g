
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

namespace mongoodbc {

StatementHandle::StatementHandle(ConnectionHandle *connHandle)
    : _connHandle(connHandle)
{
}

SQLRETURN StatementHandle::getDbs(std::list<std::string> *dbs)
{
    int rc = _connHandle->getDbNames(dbs);
    if (0 != rc) {
        return SQL_ERROR;
    }

    return SQL_SUCCESS;
}

SQLRETURN StatementHandle::getCollections(const std::string& db,
                                          std::list<std::string> *collections)
{
    int rc = _connHandle->getCollectionNames(db, collections);
    if (0 != rc) {
        return SQL_ERROR;
    }

    return SQL_SUCCESS;
}

} // close mongoodbc namespace

