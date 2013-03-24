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

#include "sql_parser.h"

#include <gtest/gtest.h>

#include <iostream>

TEST(SelectParseTest, SelectStarParseTest)
{
    mongoodbc::SQLParser<std::string::const_iterator> parser;
    mongoodbc::SQLSelectStatement stmt;
    std::string str("SELECT * FROM table");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        ASSERT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, stmt));
        std::cout << "STMT: " << stmt << std::endl;
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(SelectParseTest, SelectAllStarParseTest)
{
    mongoodbc::SQLParser<std::string::const_iterator> parser;
    mongoodbc::SQLSelectStatement stmt;
    std::string str("SELECT ALL * FROM table");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        ASSERT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, stmt));
        std::cout << "STMT: " << stmt << std::endl;
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(SelectParseTest, SelectDistinctStarParseTest)
{
    mongoodbc::SQLParser<std::string::const_iterator> parser;
    mongoodbc::SQLSelectStatement stmt;
    std::string str("SELECT DISTINCT * FROM table");
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    try
    {
        ASSERT_TRUE(
            boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, stmt));
        std::cout << "STMT: " << stmt << std::endl;
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
