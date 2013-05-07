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


#include "select_statement_parser.h"
#include "insert_statement_parser.h"
#include <boost/algorithm/string.hpp>
#include "sql_statement.h"
#include "parser_exceptions.h"


struct SQLParser 
{
	std::string iQuery;
public:

	SQLParser(std::string input_query) : iQuery (input_query)
	{
		
	}

	SqlStatement* parseQuery ()
	{
		std::string::const_iterator strtIter = iQuery.begin();
		std::string::const_iterator endIter = iQuery.end();

		// Determine what query it is over here.
		// The create an appropriate SqlStatement that would point to a specific Query Structure

		using qi::phrase_parse;
		using qi::space;
		using boost::spirit::ascii::string;
		using qi::no_case;
		using qi::char_;
		using boost::phoenix::at_c;
		using boost::spirit::qi::_val;
		//using boost::phoenix::ref;
		using boost::phoenix::at_c;
		
		std::istringstream ss (iQuery);
		std::string queryType;
		ss >> queryType;

		if(boost::iequals(queryType, SqlConstants::select)) //Select query
		{
			SelectStatement* p = new SelectStatement();
			SelectStatement_parser<std::string::const_iterator> g;
			bool r= qi::phrase_parse (strtIter, endIter, g, qi::space, *p);
			if (r && strtIter==endIter)
				return new SqlStatement (p);
			else{
				delete p;
				throw ParserExceptions(SYNTAX_ERROR);
			}
			//return NULL;
		}
		else if (boost::iequals(queryType, SqlConstants::insert)){
			std::cout << iQuery << std::endl;
			throw ParserExceptions(UNHANDLED_QUERY);
		}
		else if (boost::iequals(queryType, SqlConstants::create)){
			std::cout << iQuery << std::endl;
			throw ParserExceptions(UNHANDLED_QUERY);
		}
		else if (boost::iequals(queryType, SqlConstants::drop)){
			std::cout << iQuery << std::endl;
			throw ParserExceptions(UNHANDLED_QUERY);
		}
		else if (boost::iequals(queryType, SqlConstants::sqlDelete)){
			std::cout << iQuery << std::endl;
			throw ParserExceptions(UNHANDLED_QUERY);
		}
		else if (boost::iequals(queryType, SqlConstants::update)){
			std::cout << iQuery << std::endl;
			throw ParserExceptions(UNHANDLED_QUERY);
		}
		else{
			throw ParserExceptions(UNKNOWN_QUERY);
		}
	}

	void print (const char* line){
		std::cout << line << std::endl;
	}
};