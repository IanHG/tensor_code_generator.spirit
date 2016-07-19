#include <iostream>
#include <string>
#include <fstream>

//#include <boost/spirit/include/support_istream_iterator.hpp>

#include "ast.hpp"
#include "config.hpp"
#include "error_handler.hpp"

int main()
{
   std::ifstream inp("test.tc");
   inp.unsetf(std::ios::skipws);
   std::string source;
   std::copy
    ( std::istream_iterator<char>(inp)
    , std::istream_iterator<char>()
    , std::back_inserter(source)
    );
   
   std::cout << "========= Source program ========= \n"
             << source
             << "========= ============== ========= \n"
             << std::flush;

   using tcg::parser::iterator_type;
   iterator_type iter(source.begin());
   iterator_type end(source.end());

   using boost::spirit::x3::with;
   using tcg::parser::error_handler_type;
   error_handler_type error_handler(iter, end, std::cerr); // Our error handler
   
   // Our parser
   auto const parser =
   // we pass our error handler to the parser so we can access
   // it later on in our on_error and on_sucess handlers
   with<tcg::parser::error_handler_tag>(std::ref(error_handler))
   [
      tcg::statement()
   ];

   // skipper
   //auto skip = boost::spirit::x3::ascii::space; //| boost::spirit::x3::lexeme["/*" >> boost::spirit::x3::char_ >> "*/"];
   auto skip = tcg::parser::skipper; //| boost::spirit::x3::lexeme["/*" >> boost::spirit::x3::char_ >> "*/"];
   
   // parse the input
   tcg::ast::statement_list ast;
   bool success = phrase_parse(iter, end, parser, skip, ast);
   
   if(success && iter == end)
   {
      std::cout << " success!!" << std::endl;
   }
   else
   {
      std::cout << " failed :C" << std::endl;
   }

   return 0;
}
