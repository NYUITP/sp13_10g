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

#include "sql_element_search_condition.h"

#include <mongo/bson/bsonobj.h>

#include <gtest/gtest.h>

#include <iostream>
#include <string>

TEST(SQLElementBooleanPrimary, Breathing)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementSearchConditionParser<std::string::const_iterator> searchCondParser(&exprParser);
    mongoodbc::SQLElementBooleanPrimaryParser<std::string::const_iterator> parser(&exprParser, &searchCondParser);
    mongo::Query query;
    std::string cond("age > 5");
    std::string::const_iterator iter = cond.begin();
    std::string::const_iterator end = cond.end();
    try
    {
        boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, query);
    }
    catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
    {
        std::string fragment(ex.first, ex.last);
        std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
    }
}

TEST(SQLElementBooleanPrimary, SimpleBinaryOp)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementSearchConditionParser<std::string::const_iterator> searchCondParser(&exprParser);
    mongoodbc::SQLElementBooleanPrimaryParser<std::string::const_iterator> parser(&exprParser, &searchCondParser);
    const char *conditions[] = {
        ">"
        ,"<"
        ,"="
        ,"<="
        ,">="
        ,"<>"
    };
    int numConditions = sizeof(conditions)/sizeof(*conditions);
    for (int i = 0; i < numConditions; ++i) {
        std::string cond("age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        std::string expected("this.age ");
        if (conditions[i] == "<>") {
            expected.append("!=");
        } else if (conditions[i] == "=") {
            expected.append("==");
        } else {
            expected.append(conditions[i]);
        }
        expected.append(" 5");
        SCOPED_TRACE(cond.c_str());
        mongo::BSONObj query;
        std::string::const_iterator iter = cond.begin();
        std::string::const_iterator end = cond.end();
        try
        {
            EXPECT_TRUE(
                boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, query));
            EXPECT_EQ(expected, query.getStringField("$where"));
            std::cout << "query: " << query << std::endl;
        }
        catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
        {
            std::string fragment(ex.first, ex.last);
            std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
        }
    }
}

TEST(SQLElementBooleanFactor, SimpleBinaryOp)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementSearchConditionParser<std::string::const_iterator> searchCondParser(&exprParser);
    mongoodbc::SQLElementBooleanFactorParser<std::string::const_iterator> parser(&exprParser, &searchCondParser);
    const char *conditions[] = {
        ">"
        ,"<"
        ,"="
        ,"<="
        ,">="
        ,"<>"
    };
    int numConditions = sizeof(conditions)/sizeof(*conditions);
    for (int i = 0; i < numConditions; ++i) {
        std::string cond("age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        std::string expected("this.age ");
        if (conditions[i] == "<>") {
            expected.append("!=");
        } else if (conditions[i] == "=") {
            expected.append("==");
        } else {
            expected.append(conditions[i]);
        }
        expected.append(" 5");
        SCOPED_TRACE(cond.c_str());
        mongo::BSONObj query;
        std::string::const_iterator iter = cond.begin();
        std::string::const_iterator end = cond.end();
        try
        {
            EXPECT_TRUE(
                boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, query));
            EXPECT_EQ(expected, query.getStringField("$where"));
            std::cout << "query: " << query << std::endl;
        }
        catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
        {
            std::string fragment(ex.first, ex.last);
            std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
        }
    }
}

TEST(SQLElementBooleanFactor, NotSimpleBinaryOp)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementSearchConditionParser<std::string::const_iterator> searchCondParser(&exprParser);
    mongoodbc::SQLElementBooleanFactorParser<std::string::const_iterator> parser(&exprParser, &searchCondParser);
    const char *conditions[] = {
        ">"
        ,"<"
        ,"="
        ,"<="
        ,">="
        ,"<>"
    };
    int numConditions = sizeof(conditions)/sizeof(*conditions);
    for (int i = 0; i < numConditions; ++i) {
        std::string cond("NOT age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        std::string expected("this.age ");
        if (conditions[i] == "<>") {
            expected.append("!=");
        } else if (conditions[i] == "=") {
            expected.append("==");
        } else {
            expected.append(conditions[i]);
        }
        expected.append(" 5");
        SCOPED_TRACE(cond.c_str());
        mongo::BSONObj query;
        std::string::const_iterator iter = cond.begin();
        std::string::const_iterator end = cond.end();
        try
        {
            EXPECT_TRUE(
                boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, query));
            mongo::BSONObj inverse = query.getObjectField("$not");
            ASSERT_TRUE(inverse.isValid());
            EXPECT_EQ(expected, inverse.getStringField("$where"));
            std::cout << "query: " << query << std::endl;
        }
        catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
        {
            std::string fragment(ex.first, ex.last);
            std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
        }
    }
}

TEST(SQLElementBooleanTerm, SimpleBinaryOp)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementSearchConditionParser<std::string::const_iterator> searchCondParser(&exprParser);
    mongoodbc::SQLElementBooleanTermParser<std::string::const_iterator> parser(&exprParser, &searchCondParser);
    const char *conditions[] = {
        ">"
        ,"<"
        ,"="
        ,"<="
        ,">="
        ,"<>"
    };
    int numConditions = sizeof(conditions)/sizeof(*conditions);
    for (int i = 0; i < numConditions; ++i) {
        std::string cond("age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        std::string expected("this.age ");
        if (conditions[i] == "<>") {
            expected.append("!=");
        } else if (conditions[i] == "=") {
            expected.append("==");
        } else {
            expected.append(conditions[i]);
        }
        expected.append(" 5");
        SCOPED_TRACE(cond.c_str());
        mongo::BSONObj query;
        std::string::const_iterator iter = cond.begin();
        std::string::const_iterator end = cond.end();
        try
        {
            EXPECT_TRUE(
                boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, query));
            EXPECT_EQ(expected, query.getStringField("$where"));
            std::cout << "query: " << query << std::endl;
        }
        catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
        {
            std::string fragment(ex.first, ex.last);
            std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
        }
    }
}

TEST(SQLElementBooleanTerm, SimpleBinaryOpAndSimpleBinaryOp)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementSearchConditionParser<std::string::const_iterator> searchCondParser(&exprParser);
    mongoodbc::SQLElementBooleanTermParser<std::string::const_iterator> parser(&exprParser, &searchCondParser);
    const char *conditions[] = {
        ">"
        ,"<"
        ,"="
        ,"<="
        ,">="
        ,"<>"
    };
    int numConditions = sizeof(conditions)/sizeof(*conditions);
    for (int i = 0; i < numConditions; ++i) {
        std::string cond("age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        cond.append(" AND age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        std::string expected("this.age ");
        if (conditions[i] == "<>") {
            expected.append("!=");
        } else if (conditions[i] == "=") {
            expected.append("==");
        } else {
            expected.append(conditions[i]);
        }
        expected.append(" 5");
        SCOPED_TRACE(cond.c_str());
        mongo::BSONObj query;
        std::string::const_iterator iter = cond.begin();
        std::string::const_iterator end = cond.end();
        try
        {
            EXPECT_TRUE(
                boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, query));
            std::cout << "query: " << query << std::endl;
        }
        catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
        {
            std::string fragment(ex.first, ex.last);
            std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
        }
    }
}

TEST(SQLElementBooleanTerm, SimpleBinaryOpAndNotSimpleBinaryOp)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementSearchConditionParser<std::string::const_iterator> searchCondParser(&exprParser);
    mongoodbc::SQLElementBooleanTermParser<std::string::const_iterator> parser(&exprParser, &searchCondParser);
    const char *conditions[] = {
        ">"
        ,"<"
        ,"="
        ,"<="
        ,">="
        ,"<>"
    };
    int numConditions = sizeof(conditions)/sizeof(*conditions);
    for (int i = 0; i < numConditions; ++i) {
        std::string cond("age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        cond.append(" AND NOT age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        std::string expected("this.age ");
        if (conditions[i] == "<>") {
            expected.append("!=");
        } else if (conditions[i] == "=") {
            expected.append("==");
        } else {
            expected.append(conditions[i]);
        }
        expected.append(" 5");
        SCOPED_TRACE(cond.c_str());
        mongo::BSONObj query;
        std::string::const_iterator iter = cond.begin();
        std::string::const_iterator end = cond.end();
        try
        {
            EXPECT_TRUE(
                boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, query));
            std::cout << "query: " << query << std::endl;
        }
        catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
        {
            std::string fragment(ex.first, ex.last);
            std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
        }
    }
}

TEST(SQLElementBooleanTerm, SimpleBinaryOpOrSimpleBinaryOp)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementSearchConditionParser<std::string::const_iterator> parser(&exprParser);
    const char *conditions[] = {
        ">"
        ,"<"
        ,"="
        ,"<="
        ,">="
        ,"<>"
    };
    int numConditions = sizeof(conditions)/sizeof(*conditions);
    for (int i = 0; i < numConditions; ++i) {
        std::string cond("age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        cond.append(" OR age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        std::string expected("this.age ");
        if (conditions[i] == "<>") {
            expected.append("!=");
        } else if (conditions[i] == "=") {
            expected.append("==");
        } else {
            expected.append(conditions[i]);
        }
        expected.append(" 5");
        SCOPED_TRACE(cond.c_str());
        mongo::BSONObj query;
        std::string::const_iterator iter = cond.begin();
        std::string::const_iterator end = cond.end();
        try
        {
            EXPECT_TRUE(
                boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, query));
            std::cout << "query: " << query << std::endl;
        }
        catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
        {
            std::string fragment(ex.first, ex.last);
            std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
        }
    }
}

TEST(SQLElementBooleanTerm, SimpleBinaryOpOrSimpleBinaryOpAndSimpleBinaryOp)
{
    mongoodbc::SQLElementExpressionParser<std::string::const_iterator> exprParser;
    mongoodbc::SQLElementSearchConditionParser<std::string::const_iterator> parser(&exprParser);
    const char *conditions[] = {
        ">"
        ,"<"
        ,"="
        ,"<="
        ,">="
        ,"<>"
    };
    int numConditions = sizeof(conditions)/sizeof(*conditions);
    for (int i = 0; i < numConditions; ++i) {
        std::string cond("age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        cond.append(" OR age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        cond.append(" AND age ");
        cond.append(conditions[i]);
        cond.append(" 5");
        std::string expected("this.age ");
        if (conditions[i] == "<>") {
            expected.append("!=");
        } else if (conditions[i] == "=") {
            expected.append("==");
        } else {
            expected.append(conditions[i]);
        }
        expected.append(" 5");
        SCOPED_TRACE(cond.c_str());
        mongo::BSONObj query;
        std::string::const_iterator iter = cond.begin();
        std::string::const_iterator end = cond.end();
        try
        {
            EXPECT_TRUE(
                boost::spirit::qi::phrase_parse(iter, end, parser, boost::spirit::ascii::space, query));
            std::cout << "query: " << query << std::endl;
        }
        catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
        {
            std::string fragment(ex.first, ex.last);
            std::cerr << ex.what() << "'" << fragment << "'" << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

