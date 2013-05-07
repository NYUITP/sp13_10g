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


#ifndef PARSER_INCLUDES_H
#define PARSER_INCLUDES_H


#include <string>
#include <iostream>
#include <vector>


struct SqlConstants{
	static const std::string lt; 
	static const std::string gt;
	static const std::string lte; 
	static const std::string gte;
	static const std::string et; 
	static const std::string net;
	static const std::string like; 
	static const std::string between;
	static const std::string distinct; 
	static const std::string all;
	static const std::string select;
	static const std::string insert;
	static const std::string sqlDelete;
	static const std::string drop;
	static const std::string create;
	static const std::string update;
	static const std::string into;
	static const std::string values;
	static const std::string bLiteral_true;
	static const std::string bLiteral_false;
	static const std::string bLiteral_yes;
	static const std::string bLiteral_no;
	static const std::string bLiteral_on;
	static const std::string bLiteral_off;

};

#endif // PARSER_INCLUDES_H