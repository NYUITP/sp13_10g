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


// main.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include "sql_parser.h"
#include "sql_statement.h"


int main(int argc, char* argv[])
{

	std::string str;
	std::cout << "SQL >> ";
	while (getline (std::cin, str))
	{
		if (str.empty() || str[0]=='q' || str[0]=='Q')
			break;
		
		SQLParser sqlp(str);
		try {
			std::auto_ptr<SqlStatement> sqlSt(sqlp.parseQuery());
			sqlSt->show();
		}
		catch(ParserExceptions& e){
			std::cout << e.what() << std::endl;
		}

		std::cout << "\n\nSQL >> ";
		
	}
	return 0;
}

