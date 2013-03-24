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

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(mongoodbc CXX C)

ADD_SUBDIRECTORY(gtest-1.6.0)

SET(mongoodbc_VERSION_MAJOR 0)
SET(mongoodbc_VERSION_MINOR 1)

SET(PROJECT_SOURCE_DIR src)

SET(CMAKE_CXX_FLAGS -g)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/gtest-1.6.0/include)

ADD_LIBRARY(mongoodbc SHARED
src/odbcintf.h
src/odbcintf.cpp
src/connection_handle.h
src/connection_handle.cpp
src/environment_handle.h
src/environment_handle.cpp
src/statement_handle.h
src/statement_handle.cpp
src/sql_parser.h
src/sql_parser.cpp
src/sql_select_statement.h
src/sql_select_statement.cpp
src/sql_element_search_condition.h
src/sql_element_search_condition.cpp
src/sql_element_expression.h
src/sql_element_expression.cpp
)

TARGET_LINK_LIBRARIES(mongoodbc
mongoclient
boost_filesystem
boost_system
boost_thread
)

# unit tests
ADD_EXECUTABLE(sql_parser_unittest
src/sql_parser.t.cpp)

TARGET_LINK_LIBRARIES(sql_parser_unittest
mongoodbc
gtest)

ADD_EXECUTABLE(breathing
test/breathing.cpp
)

TARGET_LINK_LIBRARIES(breathing
odbc
)

INSTALL(TARGETS mongoodbc
        LIBRARY DESTINATION lib
)
