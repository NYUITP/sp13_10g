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

#ifndef MONGOACTIONS_H
#define MONGOACTIONS_H

#include <cstdlib>
#include <iostream>
#include <iterator> 
#include <algorithm> 
#include <iostream> 
#include <string>
#include <list>
#include <fstream>
#include <map>
#include "mongo/client/dbclient.h"

std::set<std::string> getCollFields(mongo::DBClientConnection& c, std::string db, std::string collection);
std::auto_ptr<mongo::DBClientCursor> mongoSelect(mongo::DBClientConnection& c, std::string db, std::string collection, mongo::BSONObj whereCond);

#endif // MONGOACTIONS_H
