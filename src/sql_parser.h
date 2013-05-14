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

#include "sql_select_statement.h"

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/variant.hpp>

#include <string>
#include <vector>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

namespace mongoodbc {

/*
* Type representing all possible SQL statements.
*/
typedef SQLSelectStatement SQLStatement;

/*
* Class implementing an SQL parser for the minimal set of SQL required by ODBC.
*/
template <typename It>
struct SQLParser : qi::grammar<It, SQLStatement(), ascii::space_type> {
    qi::rule<It, SQLStatement(), ascii::space_type> _start;

    SQLSelectStatementParser<It> _selectStmtParser;

    SQLParser();
};

template <typename It>
SQLParser<It>::SQLParser()
    : SQLParser::base_type(_start)
{
    _start %= _selectStmtParser._rule;

    BOOST_SPIRIT_DEBUG_NODE(_start);
}

} // close mongoodbc namespace

#endif
