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

#include "sql_element_expression.h"

#include <gtest/gtest.h>

#include <iostream>

TEST(ElementExpression_PrimaryParseTest, DynamicParameter)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_PrimaryParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Primary primary;
    std::string str("?");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, primary));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_PrimaryParseTest, ColumnName)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_PrimaryParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Primary primary;
    std::string str("column");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, primary));
        std::cout << "Column Name: " << primary._columnName << std::endl;
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_PrimaryParseTest, TableNameColumnName)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_PrimaryParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Primary primary;
    std::string str("table.column");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, primary));
        std::cout << "Column Name: " << primary._columnName << std::endl;
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_PrimaryParseTest, StringLiteral)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_PrimaryParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Primary primary;
    std::string str("\"literal\"");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, primary));
        std::cout << "Literal: " << primary._literal << std::endl;
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_PrimaryParseTest, StringLiteralDoubleQuote)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_PrimaryParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Primary primary;
    std::string str("\"lite\"\"ral\"\"\"");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, primary));
        std::cout << "Literal: " << primary._literal << std::endl;
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_FactorParseTest, StringLiteral)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_FactorParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Factor factor;
    std::string str("\"literal\"");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, factor));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_FactorParseTest, ColumnName)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_FactorParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Factor factor;
    std::string str("column");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, factor));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_FactorParseTest, TableNameColumnName)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_FactorParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Factor factor;
    std::string str("table.column");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, factor));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_TermParseTest, StringLiteral)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_TermParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Term term;
    std::string str("\"literal\"");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, term));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_TermParseTest, ColumnName)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_TermParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Term term;
    std::string str("column");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, term));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpression_TermParseTest, TableNameColumnName)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementExpression_TermParser<std::string::const_iterator> parser(&exprParser);
    mongoodbc::SQLElementExpression_Term term;
    std::string str("table.column");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, term));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpressionParseTest, StringLiteral)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> parser;
    mongoodbc::SQLElementExpression expr;
    std::string str("\"literal\"");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, expr));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpressionParseTest, ColumnName)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> parser;
    mongoodbc::SQLElementExpression expr;
    std::string str("column");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, expr));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(ElementExpressionParseTest, TableNameColumnName)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> parser;
    mongoodbc::SQLElementExpression expr;
    std::string str("table.column");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        EXPECT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, expr));
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

