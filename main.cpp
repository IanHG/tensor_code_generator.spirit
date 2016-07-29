#include <iostream>
#include <string>
#include <fstream>

//#include <boost/spirit/include/support_istream_iterator.hpp>

#include "ast.hpp"
#include "config.hpp"
#include "error_handler.hpp"
#include "compiler.hpp"
#include "code_generator.hpp"

int main()
{
   std::ifstream inp("test.tc_inp");
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

   // parse the input
   tcg::ast::statement_list ast;
   bool success = phrase_parse(iter, end, parser, tcg::parser::skipper, ast);
   
   if(success && iter == end)
   {
      std::cout << " success!!" << std::endl;
      tcg::code_gen::intermediate_program t;
      tcg::code_gen::compiler c(t, error_handler);
      if(!c.start(ast))
      {
         std::cout << " Compilation failed " << std::endl;
      }
      std::cout << t << std::endl;
      std::ofstream ofhpp("test.hpp", std::ofstream::out | std::ofstream::trunc);
      std::ofstream ofcpp("test.cpp", std::ofstream::out | std::ofstream::trunc);
      tcg::code_gen::code_generator gen(ofhpp, ofcpp, "test");
      gen.generate_code(t);
   }
   else
   {
      std::cout << " failed :C" << std::endl;
   }

   return 0;
}
