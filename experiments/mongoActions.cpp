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

#include <cstdlib>
#include <iostream>
#include <iterator> 
#include <algorithm> 
#include <iostream> 
#include <string>
#include <list>
#include <fstream>
#include <map>
#include <string>
#include "mongo/client/dbclient.h"

using namespace mongo;



// Execute Select Query and Return DB pointer
std::auto_ptr<mongo::DBClientCursor> mongoSelect(DBClientConnection& c, string db, string collection, BSONObj whereCond){
        
    std::auto_ptr<mongo::DBClientCursor> cursor = c.query(db+"."+collection, whereCond);
    return cursor;
}

//  finds the names of the document with the most fields in a collection 
std::set<string> getCollFields(DBClientConnection& c, string db, string collection){
   
    int longest;
    
    // mongo fieldnames = sql column names
    set<string> fieldnames;
    
    // get the list of ODBC supported fields (columns) from collection.meta collection
    // collection.meta should only contain one document
    std::auto_ptr<mongo::DBClientCursor> cursor = c.query(db+"."+collection+".meta");
    
    BSONObj d = cursor->next();
    
    if (d.nFields() != 0){
        longest = d.nFields();
        d.getFieldNames(fieldnames);
    }
    
    // if no meta collection find collection with most fields
    if (longest == 0) {
    
        cursor = c.query(db+"."+collection);
    
        while( cursor->more() ) {
            //  get next doc/row/tuple
            BSONObj doc = cursor->next();
            if(longest < doc.nFields()){
                longest = doc.nFields();
                doc.getFieldNames(fieldnames);
            }
        }
    }
    return fieldnames;
}
