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
#ifndef MONGOODBC_SQL_ELEMENT_EXPRESSION_H_
#define MONGOODBC_SQL_ELEMENT_EXPRESSION_H_

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>

#include <ostream>
#include <string>

namespace mongoodbc {

/*
* In memory representation of an SQL expression.
*/
struct SQLElementExpression {
    // TODO: For now, just read as a string
    std::string _expression;
};
inline std::ostream& operator<<(std::ostream& stream, const SQLElementExpression& rhs);

} // close mongoodbc namespace

BOOST_FUSION_ADAPT_STRUCT(mongoodbc::SQLElementExpression,
                          (std::string, _expression));

inline std::ostream& mongoodbc::operator<<(std::ostream& stream,
                                           const mongoodbc::SQLElementExpression& rhs)
{
    stream << rhs._expression;
    return stream;
}

#endif

