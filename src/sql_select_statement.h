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

#include <sql_element_search_condition.h>

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/variant.hpp>

#include <ostream>
#include <string>
#include <vector>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

namespace mongoodbc {

/*
* In memory representation of an SQL SELECT statement.
*/
struct SQLSelectStatement {
    typedef boost::variant<std::string, std::string> strStrVariant;
    boost::optional<strStrVariant> _allDistinct;
    bool _star;
    std::vector<std::string> _tableRefList;
    boost::optional<SQLElementSearchCondition> _searchCondition;

};
inline std::ostream& operator<<(std::ostream& stream, const SQLSelectStatement& rhs);

/*
* Parser for SQL SELECT statements.
*/
template <typename It>
struct SQLSelectStatementParser : qi::grammar<It, SQLSelectStatement(), ascii::space_type> {
    qi::rule<It, SQLSelectStatement(), ascii::space_type> _rule;
    SQLSelectStatementParser();
};

template <typename It>
SQLSelectStatementParser<It>::SQLSelectStatementParser()
    : SQLSelectStatementParser::base_type(_rule)
{
    _rule %= ascii::no_case["select"]
             >> -(ascii::no_case[ascii::string("all")] | ascii::no_case[ascii::string("distinct")])
             >> qi::matches['*']
             >> ascii::no_case["from"]
             >> (qi::lexeme[ascii::alpha >> *ascii::alnum] % ',');

    BOOST_SPIRIT_DEBUG_NODE(_rule);
};

} // close mongoodbc namespace

inline std::ostream& mongoodbc::operator<<(std::ostream& stream,
                                           const mongoodbc::SQLSelectStatement& rhs)
{
    std::string tables;
    for (size_t i = 0; i < rhs._tableRefList.size(); ++i) {
        tables.append(rhs._tableRefList[i]);
        if (i < rhs._tableRefList.size() - 1) {
            tables.append(", ");
        }
    }

    stream << "SELECT"
           << (rhs._allDistinct ? " " : "")
           << (rhs._allDistinct ? *rhs._allDistinct : "")
           << (rhs._star ? " *" : "")
           << " FROM " << tables
           << " WHERE " << rhs._searchCondition;

   return stream;        
}

BOOST_FUSION_ADAPT_STRUCT(mongoodbc::SQLSelectStatement,
                          (boost::optional<mongoodbc::SQLSelectStatement::strStrVariant>,
                           _allDistinct)
                          (bool, _star)
                          (std::vector<std::string>, _tableRefList));

#endif

