#ifndef TCG_COMMON_H_INCLUDED
#define TCG_COMMON_H_INCLUDED

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace tcg 
{
   namespace parser
   {
      namespace x3 = boost::spirit::x3;
      struct identifier_class;
      struct tensor_litteral_class;
      
      using identifier_type      = x3::rule<identifier_class, std::string>;
      using tensor_litteral_type = x3::rule<tensor_litteral_class, ast::tensor_litteral>;
      
      identifier_type const identifier = "identifier";
      const tensor_litteral_type tensor_litteral = "tensor_litteral";
      
      const auto identifier_def
       = x3::raw[x3::lexeme[(x3::alpha | '_') >> *(x3::alnum | '_')]]
       ;
      
      const auto tensor_litteral_def
       = identifier 
      >> x3::lit("[") 
       > *(x3::alpha) 
       > x3::lit("]")
       ;
      
      BOOST_SPIRIT_DEFINE
       ( identifier
       , tensor_litteral
       );

   } /* namespace parser */
} /* namespace tcg */

#endif /* TCG_COMMON_H_INCLUDED */
