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


#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include "mongo/client/dbclient.h"
// build note
// g++ simpSelect.cpp -pthread -Isrc -Isrc/mongo -lmongoclient -lboost_thread-mt -lboost_system-mt -lboost_filesystem-mt  -o simpSel

using namespace std;
using namespace mongo;

void printQueryResults(auto_ptr<DBClientCursor> cursor){
    cout<<"Enter BSON DUMPER"<<endl;
    
    while( cursor->more() ) {
        cout << cursor->next().toString() << endl;
    }
}


void easySelect(DBClientConnection& c, string dbcoll, string constrants){
    
    cout<<"\nSelect: * \nWhere: "<<constrants<< endl;
    
    
    
    char* holds = "name";
    
    auto_ptr<DBClientCursor> cursor = c.query(dbcoll, Query().where(constrants) );
    
    printQueryResults(c.query(dbcoll, Query().where(constrants) ));
 
}

void selectWhere(DBClientConnection& c, string dbcoll, int age) {
    cout<<"enter selectWhere"<<endl;

    char* jscode = "this.age < 30 && this.weight < 300";
    cout <<"the string feed is: "<<jscode<< endl;

    //auto_ptr<DBClientCursor> cursor = c.query(dbcoll, QUERY(tester).sort("name") );
    //Query badBalance = Query().where(tester);
    
    auto_ptr<DBClientCursor> cursor = c.query(dbcoll, Query().where(jscode) );

    //auto_ptr<DBClientCursor> cursor = c.query(dbcoll, QUERY( "sex" << "m" ).where("this.age == 40") );
    while( cursor->more() ) {
        BSONObj p = cursor->next();
        cout << p.getStringField("name") << endl;
    }
}

void selectONECond(DBClientConnection& c, string dbcoll, char* want, char* arg, char* val) {
    cout << "SELECT "<<want<<endl;
    cout << "FROM "<<dbcoll<< " "<<endl;
    cout << "WHERE "<<arg<< " = "<<val;
    cout <<"\n"<<want<< endl;

    
    auto_ptr<DBClientCursor> cursor = c.query(dbcoll, QUERY( arg << val ).sort(want) );
    
    while( cursor->more() ) {
        BSONObj p = cursor->next();
        cout << p.getStringField(want) << endl;
    }
    cout<<" "<<endl;
    
}

void selectTWOCond(DBClientConnection& c, string dbcoll, char* want, char* arg, char* val) {
    cout << "SELECT "<<want<<endl;
    cout << "FROM "<<dbcoll<< " "<<endl;
    cout << "WHERE "<<arg<< " = "<<val;
    cout <<"\n"<<want<< endl;
    
    
    auto_ptr<DBClientCursor> cursor = c.query(dbcoll, QUERY( arg << val ).sort(want) );
    
    while( cursor->more() ) {
        BSONObj p = cursor->next();
        cout << p.getStringField(want) << endl;
    }
    cout<<" "<<endl;
    
}

void selectTWOCond(DBClientConnection& c, string dbcoll, char* want, char* arg, int val) {
    cout << "SELECT "<<want<<endl;
    cout << "FROM "<<dbcoll<< " "<<endl;
    cout << "WHERE "<<arg<< " = "<<val;
    cout <<"\n"<<want<< endl;
    
    
    auto_ptr<DBClientCursor> cursor = c.query(dbcoll, QUERY( arg << val ).sort(want) );
    
    while( cursor->more() ) {
        BSONObj p = cursor->next();
        cout << p.getStringField(want) << endl;
    }
    cout<<" "<<endl;
    
}


void fillCollection(DBClientConnection& c, string dbcoll) {

    
    c.insert(dbcoll, BSON( "name" << "Billy" << "age" << 40 << "sex" << "m" << "bloodtype" << "O" << "weight" << 190));
    c.insert(dbcoll, BSON( "name" << "James" << "age" << 20 << "sex" << "m" << "bloodtype" << "A" << "weight" << 160));
    c.insert(dbcoll, BSON( "name" << "Sally" << "age" << 30 << "sex" << "f" << "bloodtype" << "B" << "weight" << 170));
    c.insert(dbcoll, BSON( "name" << "Denis" << "age" << 12 << "sex" << "m" << "bloodtype" << "O" << "weight" << 180));
    c.insert(dbcoll, BSON( "name" << "Sarah" << "age" << 20 << "sex" << "f" << "bloodtype" << "AB" << "weight" << 140));
    c.insert(dbcoll, BSON( "name" << "Jenny" << "age" << 12 << "sex" << "f" << "bloodtype" << "O" << "weight" << 140 << "hates" << "Sarah"));
    c.insert(dbcoll, BSON( "name" << "Fatso" << "age" << 40 << "sex" << "m" << "bloodtype" << "B" << "weight" << 600));
    c.insert(dbcoll, BSON( "name" << "Silver" << "BCODE" << 133 << "Floors" << 12 << "Rooms" << 201 << "weight" << 14000000 << "age" << 100 << "Firecode" << 2910));

    
    
    cout << "count:" << c.count(dbcoll) << endl;
}

void printStringList(const list<string>& s) {
    list<string>::const_iterator i;
  for( i = s.begin(); i != s.end(); ++i)
		cout << *i << " \n";
	cout << endl;
}

void getDatabases (DBClientConnection& c){
 	list<string> databases = c.getDatabaseNames();
    
    printStringList(databases);
    
}

void getCollections (DBClientConnection& c, string db){
    // Get the collections in DB
    list<string> dbCollections = c.getCollectionNames(db);
    int numCollections = dbCollections.size();
    
    cout<<"# Collections in "<<db<<" : "<<numCollections<<endl;
    printStringList(dbCollections);

    
}



void run() {

    
    char* want = "name";
    char* arg = "sex";
    char* val = "m";
    

    // Create Connection to Database
    DBClientConnection c;
    c.connect("localhost");
    cout << "connected ok" << endl;
    
    
    string db = "Monsters";
    string collection = "ghosts";
    string dbcoll = db+"."+collection;
    cout<<"db.collection = "<<dbcoll<<endl;
    
    getDatabases(c);
    getCollections(c, db);
    
    
    // Uncomment to fill dummy collection(table)
    //fillCollection(c, dbcoll);
    
    // print the collection
    auto_ptr<DBClientCursor> cursor = c.query(dbcoll, BSONObj());
    while( cursor->more() ) {
        cout << cursor->next().toString() << endl;
    }



    // a few test
    // selectWhere(c, "O", dbcoll);
    //selectTWOCond(c, dbcoll, want, arg, val);
    // selectWhere(c, "O", dbcoll);
    //selectTWOCond(c, dbcoll, "sex", "name", "Fatso");
    selectTWOCond(c, dbcoll, "name", "age", 12);
    //selectTWOCond(c, dbcoll, "weight", "sex", "m");
    //selectThreeCond(c, dbcoll, "name", "age", 12);
    
    //selectWhere(c, dbcoll, 40);

    //easySelect(c, dbcoll, "this.age < 30 && this.weight < 300");
    //easySelect(c, dbcoll, "this.age < 30 && this.weight < \"300\"");

   // easySelect(c, dbcoll, "this.bloodtype != \"O\"");
   // easySelect(c, dbcoll, "this.hates == \"Sarah\"");

   // easySelect(c, dbcoll, "this.weight == \"190\"");
    easySelect(c, dbcoll, "this.weight == 190");
    easySelect(c, dbcoll, "this.weight - this.age >= 151");
    easySelect(c, dbcoll, "this.hates");
    easySelect(c, dbcoll, "this.sex != \"f\"");
    easySelect(c, dbcoll, "(this.sex != \"f\") && this.bloodtype != \"O\"");
    easySelect(c, dbcoll, "((this.age >= 21) && this.bloodtype != \"O\") || this.age < 13");



}

int main() {
    try {
        run();
    }
    catch( DBException &e ) {
        cout << "caught " << e.what() << endl;
    }
    return 0;
}

