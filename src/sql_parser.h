#pragma once
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
#ifndef MONGOODBC_SQL_PARSER_H_
#define MONGOODBC_SQL_PARSER_H_

#include <sql_select_statement.h>
#include <sql_element_search_condition.h>

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>

#include <string>
#include <vector>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

namespace mongoodbc {

/*
* Class implementing an SQL parser for the minimal set of SQL required by ODBC.
*/
template <typename It>
struct SQLParser : qi::grammar<It, SQLSelectStatement(), ascii::space_type> {
  private:
    qi::rule<It, std::vector<std::string>(), ascii::space_type> _tables;
    //qi::rule<It, SQLElementSearchCondition(), ascii::space_type> _searchCond;
    qi::rule<It, SQLSelectStatement(), ascii::space_type> _start;
    qi::rule<It, std::string(), ascii::space_type> _userDefinedName;
  public:
    SQLParser();
};

template <typename It>
SQLParser<It>::SQLParser()
    : SQLParser::base_type(_start)
{
    _userDefinedName %= qi::lexeme[ascii::alpha >> *ascii::alnum];
    _tables %= (_userDefinedName % ',');

    _start %= ascii::no_case["select"]
             >> '*'
             >> ascii::no_case["from"]
             >> _tables;

    BOOST_SPIRIT_DEBUG_NODE(_start);
    BOOST_SPIRIT_DEBUG_NODE(_tables);
    BOOST_SPIRIT_DEBUG_NODE(_userDefinedName);
}

} // close mongoodbc namespace

#endif
