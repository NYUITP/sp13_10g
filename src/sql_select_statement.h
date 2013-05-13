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
#include "sql_element_search_condition.h"

#include <mongo/bson/bsonobj.h>

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
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
    std::vector<std::string> _tableRefList;
    boost::optional<mongo::Query> _whereClause;

    SQLSelectStatement();
};
inline std::ostream& operator<<(std::ostream& stream, const SQLSelectStatement& rhs);


/*
* Parser for SQL SELECT statements.
*/
template <typename It>
struct SQLSelectStatementParser : qi::grammar<It, SQLSelectStatement(), ascii::space_type> {
    qi::rule<It, std::string(), ascii::space_type> _namespace;
    qi::rule<It, SQLSelectStatement(), ascii::space_type> _rule;
    SQLElementExpressionParser<It> _exprParser;
    SQLElementSearchConditionParser<It> _searchCondParser;

    SQLSelectStatementParser();
};

template <typename It>
SQLSelectStatementParser<It>::SQLSelectStatementParser()
    : SQLSelectStatementParser::base_type(_rule)
    , _searchCondParser(&_exprParser)
{
    _namespace %= qi::lexeme[ascii::alpha >> *ascii::alnum >> ascii::char_('.') >> ascii::alpha >> *ascii::alnum];
    _rule = ascii::no_case["select"]
             >> -(ascii::no_case["all"] [phoenix::at_c<0>(qi::_val) = true] |
                 ascii::no_case["distinct"] [phoenix::at_c<1>(qi::_val) = true])
             >> ( '*' |
                  (_exprParser._rule [phoenix::push_back(phoenix::at_c<2>(qi::_val), qi::_1)] % ','))
             >> ascii::no_case["from"]
             >> _namespace [phoenix::push_back(phoenix::at_c<3>(qi::_val), qi::_1)] % ','
             >> -(ascii::no_case["where"]
                  >> _searchCondParser._rule) [phoenix::at_c<4>(qi::_val) = phoenix::construct<mongo::Query>(qi::_1)];

    BOOST_SPIRIT_DEBUG_NODE(_rule);
};

} // close mongoodbc namespace

BOOST_FUSION_ADAPT_STRUCT(
    mongoodbc::SQLSelectStatement,
    (bool, _all)
    (bool, _distinct)
    (std::vector<mongoodbc::SQLElementExpression>, _selectList)
    (std::vector<std::string>, _tableRefList)
    (boost::optional<mongo::Query>, _whereClause));

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
            columns << ",";
        }
    }

    std::stringstream tables;
    for (size_t i = 0; i < rhs._tableRefList.size(); ++i) {
        tables << rhs._tableRefList[i];
        if (i < rhs._tableRefList.size() - 1) {
            tables << ",";
        }
    }

    stream << "SELECT "
           << (rhs._all ? "ALL " : "")
           << (rhs._distinct ? "DISTINCT " : "")
           << columns.str()
           << " FROM " << tables.str()
           << (rhs._whereClause ? " WHERE " : "")
           << (rhs._whereClause ? rhs._whereClause->toString() : "");

   return stream;        
}

#endif

