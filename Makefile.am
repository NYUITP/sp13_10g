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

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libmongoodbc.la
libmongoodbc_la_SOURCES = src/odbcintf.cpp
libmongoodbc_la_CXXFLAGS = -g

bin_PROGRAMS = breathing
breathing_SOURCES = test/breathing.cpp
breathing_LDADD = -lodbc
breathing_CXXFLAGS = -g
