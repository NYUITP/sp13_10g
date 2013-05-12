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
#include "mongo/client/dbclient.h"
#include "mongoactions.h"

using namespace mongo;

void printQueryResults(auto_ptr<DBClientCursor> cursor){
    std::cout<<"Enter BSON DUMPER"<<std::endl;
    
    while( cursor->more() ) {
        std::cout<<cursor->next().toString()<<std::endl;
    }
}

void printSet(set<string> stringset){
    
    set<string>::const_iterator it; // declare an read-only iterator
    it = stringset.begin(); // assign it to the start of the vector
    while (it != stringset.end()) // while it hasn't reach the end
    {
        cout << *it << " "; // print the value of the element it points to
        it++; // and iterate to the next element
    }
    cout << endl;
}

void run() {
    mongo::DBClientConnection c;
    c.connect("localhost");
    mongo::BSONObj cx = fromjson("{age: {$gt : 12, $lt : 40}}");
    
    string db = "Monsters";
    string collection = "ghosts";
    string dbcoll = db+"."+collection;
    cout<<"db.collection = "<<dbcoll<<endl;
    //simCount(c, dbcoll, cx);
   
    printSet(getCollFields(c, db, collection));
    
    
    
    mongo::BSONObj dx = fromjson("{age: {$gt : 12}, age:{$lt : 40}}");
    
    printQueryResults(mongoSelect(c, db, collection, dx));
}

int main() {
    try {
    	run();
        std::cout << "Running Tests" << std::endl;
    } catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
    
}