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


#pragma once

#include "parser_includes.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>



namespace qi = boost::spirit::qi;

struct SelectStatement{

public:
	
	//friend std::ostream& operator<<(std::ostream& os, SelectStatement& ss){
	void show(){
		std::vector<int>::size_type i=0;
		std::string output;
		output.append("SELECT\nFields: ");
			
		for (i=0; i!=columns.size(); i++){
			output.append("<");
			output.append(columns[i]);
			output.append(">  ");
		}

		output.append ("\nTables: ");
		for (i=0; i!=tables.size(); i++){
			output.append("{");
			output.append(tables[i]);
			output.append("}  ");
		}

		output.append ("\nKeywords: ");
		for (i=0; i!=keywords.size(); i++){
			output.append(keywords[i]);
			output.append("  ");
		}

		output.append ("\nSearch Condition: ");
		for (i=0; i!=comp_predicates.size(); i++)
		{
			output.append ("[ ");
			output.append (comp_predicates[i]);
			output.append (" ]");
		}
		std::cout << output;
		//return os << output;
	}

	std::string query_type;
	std::vector<std::string> keywords;
	std::vector<std::string> columns;
	std::vector<std::string> tables;
	std::vector<std::string> comp_predicates;
};

template <typename Iterator, typename Skipper = qi::space_type>
struct SelectStatement_parser : qi::grammar <Iterator, SelectStatement(), Skipper>
{
	SelectStatement_parser() : SelectStatement_parser::base_type(start)
	{
		using qi::lexeme;
		using qi::lit;
		using qi::char_;
		using qi::alpha;
		using qi::alnum;
		using qi::string;
		using qi::matches;
		using qi::no_case;

		
		quoted_string %= lexeme [ '"' >> +(char_ - '"') >> '"'];
		table_column_name %= lexeme [ alpha >> *alnum] ;
			
		query_type =  no_case["SELECT"]
					| no_case["UPDATE"]
					;
		keywords %= no_case [string(SqlConstants::distinct)]
					| no_case [string(SqlConstants::all)];
		columns %=  ( table_column_name % ',' ) ;
		tables  %=  (table_column_name % ',');

		//Maintain the order of representation of operators in the rule below.
		op %=  string (SqlConstants::net)
			| string (SqlConstants::lte) 
			| string (SqlConstants::lt) 
			| string (SqlConstants::et)
			| string (SqlConstants::gte)
			| string (SqlConstants::gt)
			;
		
		boolean_literal %= no_case [string(SqlConstants::bLiteral_true)]
			| no_case [string(SqlConstants::bLiteral_false)]
			| no_case [string(SqlConstants::bLiteral_yes)]
			| no_case [string(SqlConstants::bLiteral_no)]
			| no_case [string(SqlConstants::bLiteral_on)]
			| no_case [string(SqlConstants::bLiteral_off)]
			;

			
		numeric_literal %= lexeme [+qi::digit];
			
		any_literal %= quoted_string 
			| numeric_literal 
			| boolean_literal;
			
		comp_predicates %= table_column_name >> op >> any_literal;
		/*
		comp_predicates %= table_column_name >> op >> quoted_string;
		*/
		start %= query_type 
			>> -( keywords ) 
			>> columns
			>> no_case ["from"] 
			>> tables 
			>> -(no_case ["where"] 
			>> comp_predicates)
			>> -char_(";")
			;

		
		//comp_predicates.name("predicate");
		//debug(comp_predicates);
		//start.name("start");
		//debug (start);
	}

	qi::rule <Iterator, std::string(), Skipper> quoted_string;
	qi::rule <Iterator, std::string(), Skipper> table_column_name;
	qi::rule <Iterator, std::string()> op;
	qi::rule <Iterator, std::vector<std::string>(), Skipper> comp_predicates;
	qi::rule <Iterator, SelectStatement(), Skipper> start;
	//qi::rule <Iterator, std::vector<std::string>(), Skipper> start;
	qi::rule <Iterator, std::string()> query_type;
	qi::rule <Iterator, std::string()> keywords;
	qi::rule <Iterator, std::vector<std::string>(), Skipper> columns, tables;
	qi::rule <Iterator, std::string(), Skipper> boolean_literal;
	qi::rule <Iterator, std::string(), Skipper> numeric_literal;
	qi::rule <Iterator, std::string(), Skipper> any_literal;
		
};

BOOST_FUSION_ADAPT_STRUCT( 
		SelectStatement, 
		(std::string, query_type)
		(std::vector<std::string>, keywords)
		(std::vector<std::string>, columns)
		(std::vector<std::string>, tables)
		(std::vector<std::string>, comp_predicates)
		)

/*
struct QueryParser{
	QueryParser(std::string input_query) : iQuery (input_query)
	{
		std::string::const_iterator strtIter = iQuery.begin();
		std::string::const_iterator endIter = iQuery.end();
		//SQLParser::proposition p;
		SelectStatement p;
		SelectStatement_parser<std::string::const_iterator> g;

		bool r = qi::phrase_parse (strtIter, endIter, g, qi::space, p);

		if (r && strtIter == endIter)
		{
			//std::cout << boost::fusion::tuple_open('[');
			//std::cout << boost::fusion::tuple_close(']');
			//std::cout << boost::fusion::tuple_delimiter(", ");
			std::cout << "-------------------------\n";
			std::cout << "Parsing succeeded\n";
			//std::cout << "got: " << boost::fusion::as_vector(p) << std::endl;
			std::cout << "\n-------------------------\n";

			std::cout << std::endl << p << "\n";
		}
		else
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << "-------------------------\n";
		}
	}

	std::string iQuery;
};
*/