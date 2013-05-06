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
#ifndef MONGOODBC_SQL_ELEMENT_COLUMN_NAME_H_
#define MONGOODBC_SQL_ELEMENT_COLUMN_NAME_H_

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/variant.hpp>

#include <ostream>
#include <string>
#include <vector>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

namespace mongoodbc {

struct SQLElementColumnName {
    boost::optional<std::string> _tableName;
    std::string _columnName;
};
inline std::ostream& operator<<(std::ostream& stream, const SQLElementColumnName& rhs);

/*
* Parser for SQL 'primary' element statements.
*/
template <typename It>
struct SQLElementColumnNameParser : qi::grammar<It, SQLElementColumnName()> {
    qi::rule<It, SQLElementColumnName()> _rule;
    SQLElementColumnNameParser();
};

template <typename It>
SQLElementColumnNameParser<It>::SQLElementColumnNameParser()
    : SQLElementColumnNameParser::base_type(_rule)
{
    _rule %= -qi::lexeme[ascii::alpha >> *ascii::alnum >> '.']
             >> qi::lexeme[ascii::alpha >> *ascii::alnum];
    BOOST_SPIRIT_DEBUG_NODE(_rule);
};

} // close mongoodbc namespace

inline std::ostream& mongoodbc::operator<<(std::ostream& stream,
                                           const mongoodbc::SQLElementColumnName& rhs)
{
    stream << (rhs._tableName ? rhs._tableName.get() + "." : "")
           << rhs._columnName;
    return stream;
}

BOOST_FUSION_ADAPT_STRUCT(mongoodbc::SQLElementColumnName,
                          (boost::optional<std::string>, _tableName)
                          (std::string, _columnName));

#endif



