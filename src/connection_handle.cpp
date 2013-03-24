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

namespace mongoodbc {

ConnectionHandle::ConnectionHandle(EnvironmentHandle *envHandle)
    : _envHandle(endHandle)
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

} // close mongoodbc namespace

