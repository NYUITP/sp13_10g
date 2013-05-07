# mongoDb ODBC Driver

This is an experimental ODBC driver for MongoDB. It attempts expose some mongoDB functionality through basic SQL queries and the ODBC driver interface.  The driver is implemented in C++, but the ODBC interface is, by necessity, a pure C interface.

This project was initially developed during the spring 2013 session of NYU's CSCI-GA.3812 Information Technology Projects in collaboration with 10gen.

This code is still very experimental and limited in terms of features, but we have released it in hopes that someone may learn from what we have done.

## License

Unless otherwise specified, for example, on our copy of gtest the code is lecensed as follows.

    #  Copyright [2013] Kyle Galloway (kyle.s.galloway@gmail.com)
    #                   Pravish Sood (pravish.sood@gmail.com)
    #                   Dylan Kelemen (dckelemen@gmail.com)

    #  Licensed under the Apache License, Version 2.0 (the "License");
    #  you may not use this file except in compliance with the License.
    #  You may obtain a copy of the License at

    #      http://www.apache.org/licenses/LICENSE-2.0

    #  Unless required by applicable law or agreed to in writing, software
    #  distributed under the License is distributed on an "AS IS" BASIS,
    #  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    #  See the License for the specific language governing permissions and
    #  limitations under the License.

## Building

### Requirements

* cmake
* boost\_system
* C++ mongoDB driver

### Procedure

A typical build is done by creating a build directory, running cmake, then running make.

    mkdir build
    cd build
    cmake ..
    make
    make install

## Testing

The build will create several executables of the form `***_unittest`.  These are intended to test the varrious components of the system and will, when executed, report success or failure.

