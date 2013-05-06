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
#ifndef MONGOODBC_SQL_SELECT_STATEMENT_H_
#define MONGOODBC_SQL_SELECT_STATEMENT_H_

#include "sql_element_expression.h"

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/variant.hpp>

#include <ostream>
#include <string>
#include <vector>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace spirit = boost::spirit;
namespace phoenix = boost::phoenix;

namespace mongoodbc {

/*
* In memory representation of an SQL SELECT statement.
*/
struct SQLSelectStatement {
    bool _all;
    bool _distinct;
    std::vector<SQLElementExpression> _selectList;
    //std::vector<std::string> _selectList;
    std::vector<std::string> _tableRefList;
    //boost::optional<SQLElementSearchCondition> _searchCondition;

    SQLSelectStatement();
};
inline std::ostream& operator<<(std::ostream& stream, const SQLSelectStatement& rhs);


/*
* Parser for SQL SELECT statements.
*/
template <typename It>
struct SQLSelectStatementParser : qi::grammar<It, SQLSelectStatement(), ascii::space_type> {
    qi::rule<It, SQLSelectStatement(), ascii::space_type> _rule;
    SQLElementExpressionParser<It> _exprParser;

    SQLSelectStatementParser();
};

template <typename It>
SQLSelectStatementParser<It>::SQLSelectStatementParser()
    : SQLSelectStatementParser::base_type(_rule)
{
    _rule = ascii::no_case["select"]
             >> -(ascii::no_case[ascii::string("all")] [phoenix::at_c<0>(qi::_val) = true] |
                 ascii::no_case[ascii::string("distinct")] [phoenix::at_c<1>(qi::_val) = true])
             >> ( qi::lit('*') |
                 (_exprParser._rule [phoenix::push_back(phoenix::at_c<2>(qi::_val), qi::_1)] >>
                  _exprParser._rule [phoenix::push_back(phoenix::at_c<2>(qi::_val), qi::_1)] % ","))
             >> ascii::no_case["from"]
             >> spirit::as_string[qi::lexeme[ascii::alpha >> *ascii::alnum]]
                     [phoenix::push_back(phoenix::at_c<3>(qi::_val), qi::labels::_1)]
             >> -(spirit::as_string[qi::lexeme[ascii::alpha >> *ascii::alnum]]
                     [phoenix::push_back(phoenix::at_c<3>(qi::_val), qi::labels::_1)] % ",");

    BOOST_SPIRIT_DEBUG_NODE(_rule);
};

} // close mongoodbc namespace

BOOST_FUSION_ADAPT_STRUCT(
    mongoodbc::SQLSelectStatement,
    (bool, _all)
    (bool, _distinct)
    (std::vector<mongoodbc::SQLElementExpression>, _selectList)
    (std::vector<std::string>, _tableRefList));

inline std::ostream& mongoodbc::operator<<(std::ostream& stream,
                                           const mongoodbc::SQLSelectStatement& rhs)
{
    std::stringstream columns;
    if (0 == rhs._selectList.size()) {
        columns << "*";
    }
    for (size_t i = 0; i < rhs._selectList.size(); ++i) {
        columns << rhs._selectList[i];
        if (i < rhs._selectList.size() - 1) {
            columns << ", ";
        }
    }

    std::string tables;
    for (size_t i = 0; i < rhs._tableRefList.size(); ++i) {
        tables.append(rhs._tableRefList[i]);
        if (i < rhs._tableRefList.size() - 1) {
            tables.append(", ");
        }
    }

    stream << "SELECT "
           << (rhs._all ? "ALL " : "")
           << (rhs._distinct ? "DISTINCT " : "")
           << columns.str()
           << " FROM " << tables;

   return stream;        
}

#endif

