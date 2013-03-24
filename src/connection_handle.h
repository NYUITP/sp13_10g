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
#ifndef MONGOODBC_CONNECTION_HANDLE_H_
#define MONGOODBC_CONNECTION_HANDLE_H_

#include <mongo/client/dbclient.h>

namespace mongoodbc {

/*
* Class implementing an ODBC connection handle.
*/
class ConnectionHandle {
    // The environment handle from which this connection handle was created
    // held, not owned.
    EnvironmentHandle *_envHandle;

    // The actual connection to the mongoDB database
    mongo::DBClientConnection _conn;
  public:
    /*
    * This method establishes a connection to the underlying mongoDB.
    * @return 0 on success, non-zero otherwise
    */
    int connect();
};

} // close mongoodbc namespace

#endif

