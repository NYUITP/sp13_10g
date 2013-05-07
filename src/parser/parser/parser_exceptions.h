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


#include <exception>

enum ExceptionType{
	UNKNOWN_QUERY,
	SYNTAX_ERROR,
	UNHANDLED_QUERY
};

struct ParserExceptions : public std::exception
{
	ParserExceptions (int exceptionId): eType(exceptionId)
	{
	}

	const char * what () const throw ()
	{
		//ExceptionType exx;
		switch (eType){
		case UNKNOWN_QUERY:
			return "Not an SQL query";

		case SYNTAX_ERROR:
			return "Inappropriate SQL query. Please check your SQL syntax";

		case UNHANDLED_QUERY:
			return "Query not handled yet !";

		default:
			return "Parsing failed for unknown reason";
		}
		
	}
private:
	int eType;
};