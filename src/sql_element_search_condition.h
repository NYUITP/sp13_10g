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
#ifndef MONGOODBC_SQL_ELEMENT_SEARCH_CONDITION_H_
#define MONGOODBC_SQL_ELEMENT_SEARCH_CONDITION_H_

#include "sql_element_expression.h"

#include <mongo/client/dbclient.h>
#include <mongo/bson/bsonobj.h>
#include <mongo/bson/bsonmisc.h>

#include <boost/bind.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/home/phoenix/operator/member.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
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

namespace {

struct BSONFromComparison {

    template<typename Arg1, typename Arg2, typename Arg3>
    struct result {
        typedef mongo::BSONObj type;
    };

    template<typename Arg1, typename Arg2, typename Arg3>
    mongo::BSONObj operator()(const Arg1& lhs,
                              const Arg2& op,
                              const Arg3& rhs) const
    {
        std::string lhsStr;
        lhs.toString(&lhsStr);
        std::string rhsStr;
        rhs.toString(&rhsStr);
        lhsStr.append(" ");
        lhsStr.append(op);
        lhsStr.append(" ");
        lhsStr.append(rhsStr);
        mongo::BSONObjBuilder obj;
        return obj.append("$where", lhsStr).obj();
    }
};

struct BSONFromNot {

    template<typename Arg1>
    struct result {
        typedef mongo::BSONObj type;
    };

    template<typename Arg1>
    mongo::BSONObj operator()(const Arg1& rhs) const
    {
        mongo::BSONObjBuilder obj;
        return obj.append("$not", rhs).obj();
    }
};

struct BSONFromAnd {

    template<typename Arg1, typename Arg2>
    struct result {
        typedef mongo::BSONObj type;
    };

    template<typename Arg1, typename Arg2>
    mongo::BSONObj operator()(const Arg1& lhs,
                              const Arg2& rhs) const
    {
        mongo::BSONArrayBuilder arr;
        arr.append(lhs);
        arr.append(rhs);
        mongo::BSONObjBuilder obj;
        return obj.append("$and", arr.arr()).obj();
    }
};

struct BSONFromOr {

    template<typename Arg1, typename Arg2>
    struct result {
        typedef mongo::BSONObj type;
    };

    template<typename Arg1, typename Arg2>
    mongo::BSONObj operator()(const Arg1& lhs,
                              const Arg2& rhs) const
    {
        mongo::BSONArrayBuilder arr;
        arr.append(lhs);
        arr.append(rhs);
        mongo::BSONObjBuilder obj;
        return obj.append("$or", arr.arr()).obj();
    }
};

} // close unnamed namespace

namespace mongoodbc {

template<typename It>
struct SQLElementSearchConditionParser;

template <typename It>
struct SQLElementBooleanPrimaryParser : qi::grammar<It, mongo::BSONObj(), ascii::space_type> {
    qi::rule<It, mongo::BSONObj(), ascii::space_type> _rule;
    qi::rule<It, std::string(), ascii::space_type> _comparisonOp;

    SQLElementExpressionParser<It> *_exprParser;
    SQLElementSearchConditionParser<It> *_searchCondParser;

    SQLElementBooleanPrimaryParser(SQLElementExpressionParser<It> *exprParser,
                                   SQLElementSearchConditionParser<It> *searchCondParser);
};

template <typename It>
SQLElementBooleanPrimaryParser<It>::SQLElementBooleanPrimaryParser(
    SQLElementExpressionParser<It> *exprParser,
    SQLElementSearchConditionParser<It> *searchCondParser)
    : SQLElementBooleanPrimaryParser::base_type(_rule)
    , _exprParser(exprParser)
    , _searchCondParser(searchCondParser)
{
    _comparisonOp = qi::lexeme[qi::lit('<') >> qi::lit('=')] [qi::_val = "<="] |
                    qi::lexeme[qi::lit('>') >> qi::lit('=')] [qi::_val = ">="] |
                    qi::lexeme[qi::lit('<') >> qi::lit('>')] [qi::_val = "!="] |
                    qi::lit('>') [qi::_val = ">"] |
                    qi::lit('<') [qi::_val = "<"] |
                    qi::lit('=') [qi::_val = "=="];

    phoenix::function<BSONFromComparison> bsonFromComparison;

    _rule = (_exprParser->_rule 
             >> qi::as_string[_comparisonOp]
             >> _exprParser->_rule) [qi::_val = bsonFromComparison(qi::_1, qi::_2, qi::_3)] |
            _searchCondParser->_rule [qi::_val = qi::_1];
}

template <typename It>
struct SQLElementBooleanFactorParser : qi::grammar<It, mongo::BSONObj(), ascii::space_type> {
    qi::rule<It, mongo::BSONObj(), ascii::space_type> _rule;

    SQLElementBooleanPrimaryParser<It> _primaryParser;

    SQLElementBooleanFactorParser(SQLElementExpressionParser<It> *exprParser,
                                  SQLElementSearchConditionParser<It> *searchCondParser);
};

template <typename It>
SQLElementBooleanFactorParser<It>::SQLElementBooleanFactorParser(
    SQLElementExpressionParser<It> *exprParser,
    SQLElementSearchConditionParser<It> *searchCondParser)
    : SQLElementBooleanFactorParser::base_type(_rule)
    , _primaryParser(exprParser, searchCondParser)
{
    phoenix::function<BSONFromNot> bsonFromNot;

    _rule = (ascii::no_case["NOT"] >> _primaryParser._rule) [qi::_val = bsonFromNot(qi::_1)] |
            _primaryParser._rule [qi::_val = qi::_1];
}

template <typename It>
struct SQLElementBooleanTermParser : qi::grammar<It, mongo::BSONObj(), ascii::space_type> {
    qi::rule<It, mongo::BSONObj(), ascii::space_type> _rule;

    SQLElementBooleanFactorParser<It> _factorParser;

    SQLElementBooleanTermParser(SQLElementExpressionParser<It> *exprParser,
                                SQLElementSearchConditionParser<It> *searchCondParser);
};

template <typename It>
SQLElementBooleanTermParser<It>::SQLElementBooleanTermParser(
    SQLElementExpressionParser<It> *exprParser,
    SQLElementSearchConditionParser<It> *searchCondParser)
    : SQLElementBooleanTermParser::base_type(_rule)
    , _factorParser(exprParser, searchCondParser)
{
    phoenix::function<BSONFromAnd> bsonFromAnd;

    _rule = (_factorParser._rule >> ascii::no_case["AND"] >> _rule) [qi::_val = bsonFromAnd(qi::_1, qi::_2)] |
            _factorParser._rule [qi::_val = qi::_1];
}

template <typename It>
struct SQLElementSearchConditionParser : qi::grammar<It, mongo::BSONObj(), ascii::space_type> {
    qi::rule<It, mongo::BSONObj(), ascii::space_type> _rule;

    SQLElementBooleanTermParser<It> _termParser;

    SQLElementSearchConditionParser(SQLElementExpressionParser<It> *exprParser);
};

template <typename It>
SQLElementSearchConditionParser<It>::SQLElementSearchConditionParser(
    SQLElementExpressionParser<It> *exprParser)
    : SQLElementSearchConditionParser::base_type(_rule)
    , _termParser(exprParser, this)
{
    phoenix::function<BSONFromOr> bsonFromOr;

    _rule = (_termParser._rule >> ascii::no_case["OR"] >> _rule) [qi::_val = bsonFromOr(qi::_1, qi::_2)] |
            _termParser._rule [qi::_val = qi::_1];
}

}
#endif

