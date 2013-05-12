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


namespace qi = boost::spirit::qi;


#include "parser_includes.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
//#include <boost/fusion/container/vector/vector_fwd.hpp>
//#include <boost/fusion/include/vector_fwd.hpp>


#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

typedef boost::variant<int, double, std::string> valueType;

class values_visitor: public boost::static_visitor<int>
{
public:
    int operator()(int& i) const
    {
        return i;
    }
    
    const std::string operator()(const std::string & str) const
    {
        return str;
    }

	double operator()(double& df) const
	{
		return df;
	}
};


struct InsertStatement{
public:

	std::vector<std::string> query_type;
	std::string table_name;
	std::vector<std::string> attribute_list;
	std::vector<valueType> value_list;
	//boost::fusion::vector<int, float> v(12, 5.5f);

	void show(){
		std::vector<int>::size_type i=0;
		std::string output;
		output.append ("Query: ");
		for(std::vector<std::string>::size_type i = 0; i != query_type.size(); i++) {
			output.append(query_type[i]);
			output.append(" ");
		}
		output.append ("\n");
		output.append ("Table: ");
		output.append (table_name);
		
		values_visitor visitor;
		for(std::vector<std::string>::size_type i = 0; i != attribute_list.size(); i++) {
			output.append("\nAttribute:  { ");
			output.append(attribute_list[i]);
			output.append (" : ");
			output.append (boost::lexical_cast<std::string> (value_list[i]));
			output.append (" }");
			///output.append(value_list[i]);
		}
		output.append ("\n");
		std::cout << output;

	}
};

template <typename Iterator, typename Skipper = qi::space_type>
struct InserStatement_parser : qi::grammar <Iterator, InsertStatement(), Skipper>
{
	InserStatement_parser() : InserStatement_parser::base_type(start)
	{
		using qi::no_case;
		using qi::string;
		using qi::lexeme;
		using qi::alpha;
		using qi::alnum;
		using qi::char_;
		using qi::int_;
		using qi::double_;


		numeric_literal %= double_ | int_  ;
		quoted_string %= lexeme [ '"' >> +(char_ - '"') >> '"'];
		aValue = numeric_literal 
				| quoted_string
				| no_case [string(SqlConstants::sqlNull)];

		value_list = aValue % ',';

		table_column_name %= lexeme [ alpha >> *alnum] ;
		table_name %= lexeme [ alpha >> * (alnum | char_(SqlConstants::underscore))];
		attribute_list = table_column_name % ',';
		query_type %= no_case [string(SqlConstants::insert)] >> no_case [string(SqlConstants::into)];
		start %= query_type 
			>> table_name 
			>> -('('
			>> attribute_list
			>> ')')
			>> no_case ["values"]
			>> '('
			>> value_list
			>> ')'
			>> -char_(";")
			;

		//table_name.name("table");
		//debug(table_name);
		start.name("start");
		debug(start);
	}

	qi::rule <Iterator, std::string(), Skipper> quoted_string;
	qi::rule <Iterator, boost::variant<int, double>()> numeric_literal;
 	qi::rule <Iterator, valueType(), Skipper> aValue;
	qi::rule <Iterator, std::vector<valueType>(), Skipper> value_list;
	qi::rule <Iterator, std::string(), Skipper> table_column_name;
	qi::rule <Iterator,  std::vector<std::string>(), Skipper> attribute_list; //locals <std::vector<std::string>>,
	qi::rule <Iterator, std::string()> table_name;
	qi::rule <Iterator, std::vector<std::string>(), Skipper> query_type;
	qi::rule <Iterator, InsertStatement(), Skipper> start;

};

BOOST_FUSION_ADAPT_STRUCT( 
		InsertStatement, 
		(std::vector<std::string>, query_type)
		(std::string, table_name)
		(std::vector<std::string>, attribute_list)
		(std::vector<valueType>, value_list)
		)