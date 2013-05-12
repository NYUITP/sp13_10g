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

//  Used to Create qucik tests of mongo actions later incorportated into driver
//


// build note
// g++ mongoActions.cpp -pthread -Isrc -Isrc/mongo -lmongoclient -lboost_thread-mt -lboost_system-mt -lboost_filesystem-mt  -o mongoActions


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

void simCount(DBClientConnection& c, string dbcoll, BSONObj bobject){
        
    cout<<"\n simCount \n"<< endl;
    
    // BSONObj cx = BSONObjBuilder().append(fromjson("{age:12}")).obj();
    
    mongo::BSONObj cx = fromjson("{age: {$gt : 12, $lt : 40}}");
    mongo::BSONObj dx = fromjson("{age: {$gt : 12}, age:{$lt : 40}}");
    
    
    mongo::Query query = QUERY("age"<< NE << 12);
    auto_ptr<mongo::DBClientCursor> cursor = c.query(dbcoll,dx);
    
    set<string> fieldnames;
    
    while( cursor->more() ) {
        BSONObj p = cursor->next();
        cout<< p.nFields() <<endl;
        cout << p.getField("name").toString() << endl;
        cout << p.getFieldNames(fieldnames) << endl;
        cout << fieldnames.size() << endl;
    }
}


std::auto_ptr<mongo::DBClientCursor> mongoSelect(DBClientConnection& c, string db, string collection, string fields,  BSONObj whereCond){
    
    //    
    cout << "Hello mongoSelect" << endl;
    
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


void run() {
    mongo::DBClientConnection c;
    c.connect("localhost");
    mongo::BSONObj cx = fromjson("{age: {$gt : 12, $lt : 40}}");
    
    string db = "Monsters";
    string collection = "ghosts";
    string dbcoll = db+"."+collection;
    cout<<"db.collection = "<<dbcoll<<endl;
    simCount(c, dbcoll, cx);
   
    printSet(getCollFields(c, db, collection));
    
    
    
    mongo::BSONObj dx = fromjson("{age: {$gt : 12}, age:{$lt : 40}}");
    
    printQueryResults(mongoSelect(c, db, collection, "name", dx));
}


int main() {
    try {
        run();
        std::cout << "connected ok" << std::endl;
    } catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
    
}