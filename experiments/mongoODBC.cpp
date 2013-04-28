// mongoODBC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>

// #define BOOST_SPIRIT_DEBUG
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>

namespace qi = boost::spirit::qi;

typedef boost::variant<double, std::string> double_n_string;	// string_literal, numeric_literal, boolean_literal;

struct SelectStatement
{
    std::vector<std::string> columns, fromtables;
    std::vector<std::string> whereclause; // TODO model as a vector<WhereCondition> :)

    friend std::ostream& operator<<(std::ostream& os, SelectStatement const& ss)
    {
        //return os << "SELECT [" << ss.columns.size() << " columns] from [" << ss.fromtables.size() << " tables]\nWHERE " + ss.whereclause;
		std::vector<int>::size_type i=0;
		std::string output;
		output.append("SELECT\nFields: ");

		for(std::vector<int>::size_type i = 0; i != ss.columns.size(); i++) {
			output.append(ss.columns[i]);
			output.append(", ");

		}
		//std::vector<int>::size_type i1=0;
		output.append("\nTables: ");
		for(std::vector<int>::size_type i = 0; i != ss.fromtables.size(); i++) {
			output.append(ss.fromtables[i]);
			output.append(", ");

		}
		output.append("\nSearchConditions: ");
		for(std::vector<int>::size_type i = 0; i != ss.whereclause.size(); i++) {
			output.append(ss.whereclause[i]);
			output.append(", ");

		}
		//output.append("\nWHERE: " + ss.whereclause);
		return os << output;
		//return os << "SELECT " << std::endl << ss.columns.size() << " columns] from [" << ss.fromtables.size() << " tables]\nWHERE " + ss.whereclause;
    }
};

BOOST_FUSION_ADAPT_STRUCT(SelectStatement,
        (std::vector<std::string>, columns)
        (std::vector<std::string>, fromtables)
        (std::vector<std::string>, whereclause)
       )

const std::string lt("<"); const std::string gt(">");
const std::string lte("<="); const std::string gte(">=");
const std::string et("="); const std::string net(">=");
const std::string like("like"); const std::string between("between");

template <typename It, typename Skipper = qi::space_type>
    struct parser : qi::grammar<It, SelectStatement(), Skipper>
{
    parser() : parser::base_type(start)
    {
        //using namespace qi;
		using qi::lexeme;
		//using qi::double_;
		using qi::char_;
		using qi::alpha;
		using qi::alnum;
		using qi::string;
		using qi::no_case;
		


        sqlident = lexeme [ alpha >> *alnum ]; // table or column name
        columns  = no_case [ "select" ] >> -(no_case ["distinct"] | no_case ["ALL"]) >> (sqlident % ',');
        tables   = no_case [ "from" ]   >> (sqlident % ',');
        //whr		 = no_case [ "where" ] >> (some_column);

		comparison_op =
		string(lt) | string(gt) | string(lte) | string(gte) | string(et) | string(net) | string(like) | string(between);

		//boost::spirit::qi::string(">") | boost::spirit::qi::string("<") | boost::spirit::qi::string("<=");// | lit (">=") | lit ("=") | lit ("<>");

		boolean_literal %=
			no_case ["TRUE"] | no_case ["FALSE"] | no_case ["YES"] | no_case ["NO"] | no_case ["TRUE"] | no_case ["ON"] | no_case ["OFF"];

		numeric_literal %= 
			+qi::digit;

		string_literal %=
			lexeme['"' >> +(char_ - '"') >> '"'];

		club_literal %=
			boolean_literal | numeric_literal | string_literal;

		comparison_predicate %=
			column_identifier >> comparison_op >> club_literal;
			//| club_literal >> comparison_op >> column_identifier;

		column_identifier %=
			user_defined_name;

		user_defined_name %=
			lexeme [boost::spirit::qi::alpha >> *boost::spirit::qi::alnum];

		boolean_primary %=
			comparison_predicate
			| '(' >> search_condition >> ')';

		boolean_factor %=
			-( no_case ["NOT"]) >> boolean_primary;

		boolean_term %=
			boolean_factor >> -( no_case ["AND"] >> boolean_term);

		search_condition =
			boolean_term >> -(no_case ["OR"] >> search_condition);

        start    = columns >> tables
            >> no_case [ "where" ]
            >> search_condition//lexeme [ +(char_ - ';') ]
            >> -char_(";");

        BOOST_SPIRIT_DEBUG_NODE(start);
        BOOST_SPIRIT_DEBUG_NODE(sqlident);
        BOOST_SPIRIT_DEBUG_NODE(columns);
        BOOST_SPIRIT_DEBUG_NODE(tables);
    }
	
  private:
    qi::rule<It, std::string()             , Skipper> sqlident;
    qi::rule<It, std::vector<std::string>(), Skipper> columns  , tables;//, whr;
    qi::rule<It, SelectStatement()         , Skipper> start;
	qi::rule<It, std::string(), Skipper> compOp;
	qi::rule<It, std::string(), Skipper> expression;
	qi::rule<It, std::string(), Skipper>
		boolean_literal,		// TRUE|FALSE|YES|NO|ON|OFF
		string_literal,			// A sequence of one or more characters (any excluded characters? not for now)
		comparison_op,			// < | > | <= | >= | = | <>
		column_identifier,		// A string given by the user
		user_defined_name		// letter >> [letter | digit]
		;
	//qi::rule <It, double_n_string(), Skipper>
	qi::rule <It, std::string(), Skipper>
		club_literal,
		comparison_predicate,
		boolean_primary,
		boolean_factor,
		boolean_term,
		search_condition
		;
	qi::rule<It, std::string(), Skipper >
		numeric_literal
		;

};

template <typename C, typename Skipper>
    bool doParse(const C& input, const Skipper& skipper)
{
    auto f(std::begin(input)), l(std::end(input));

    parser<decltype(f), Skipper> p;
    SelectStatement query;

    try
    {
        bool ok = qi::phrase_parse(f,l,p,skipper,query);
        if (ok)
        {
            std::cout << "parse success\n\n";
            std::cout << "query: " << query << "\n";
        }
        else      std::cerr << "parse failed: '" << std::string(f,l) << "'\n";

        if (f!=l) std::cerr << "trailing unparsed: '" << std::string(f,l) << "'\n";
        return ok;
    } catch(const qi::expectation_failure<decltype(f)>& e)
    {
        std::string frag(e.first, e.last);
        std::cerr << e.what() << "'" << frag << "'\n";
    }

    return false;
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "mongoODBC connector" << std::endl << "sql [q/Q to quit] >> ";

	bool ok;
	std::string sqlinput;
	while (std::getline(std::cin,sqlinput))
	{
		if (sqlinput.empty() || sqlinput[0] == 'q' || sqlinput[0] == 'Q')
			break;

		// = "select id, name, price from books, authors where books.author_id = authors.id;";

		ok = doParse(sqlinput, qi::space);

		std::cout << std::endl << "sql [q/Q to quit] >> ";
	}

    return 0;
}

