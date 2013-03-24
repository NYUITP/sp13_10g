// mongoODBC.cpp : Defines the entry point for the console application.
//


// #define BOOST_SPIRIT_DEBUG
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

struct SelectStatement
{
    std::vector<std::string> columns, fromtables; 
    std::string whereclause; // TODO model as a vector<WhereCondition> :) 

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
		output.append("\nWHERE " + ss.whereclause);
		return os << output;
		//return os << "SELECT " << std::endl << ss.columns.size() << " columns] from [" << ss.fromtables.size() << " tables]\nWHERE " + ss.whereclause;
    }
};

BOOST_FUSION_ADAPT_STRUCT(SelectStatement, 
        (std::vector<std::string>, columns)
        (std::vector<std::string>, fromtables)
        (std::string, whereclause)
       )

template <typename It, typename Skipper = qi::space_type>
    struct parser : qi::grammar<It, SelectStatement(), Skipper>
{
    parser() : parser::base_type(start)
    {
        using namespace qi;

        sqlident = lexeme [ alpha >> *alnum ]; // table or column name

        columns  = no_case [ "select" ] >> (sqlident % ',');
        tables   = no_case [ "from" ]   >> (sqlident % ',');

        start    = columns >> tables 
            >> no_case [ "where" ]
            >> lexeme [ +(char_ - ';') ]
            >> ';';

        BOOST_SPIRIT_DEBUG_NODE(start);
        BOOST_SPIRIT_DEBUG_NODE(sqlident);
        BOOST_SPIRIT_DEBUG_NODE(columns);
        BOOST_SPIRIT_DEBUG_NODE(tables);
    }

  private:
    qi::rule<It, std::string()             , Skipper> sqlident;
    qi::rule<It, std::vector<std::string>(), Skipper> columns  , tables;
    qi::rule<It, SelectStatement()         , Skipper> start;
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

//int main()
//{
//    const std::string input = "select id, name, price from books, authors where books.author_id = authors.id;";
//    bool ok = doParse(input, qi::space);

//    return ok? 0 : 255;
//}


int _tmain(int argc, _TCHAR* argv[])
{

	std::cout << "mongoODBC connector" << std::endl << "sql >> ";

	std::string sqlinput; // = "select id, name, price from books, authors where books.author_id = authors.id;";
	std::getline(std::cin,sqlinput);
    bool ok = doParse(sqlinput, qi::space);
	
	
	std::string exitstring;
    std::cin >> exitstring;

    return ok? 0 : 255;
}

