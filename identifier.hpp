#ifndef TCG_IDENTIFIER_H_INCLUDED
#define TCG_IDENTIFIER_H_INCLUDED

#include <boost/spirit/home/x3.hpp>

namespace tcg 
{
   namespace parser
   {
      namespace x3 = boost::spirit::x3;
      struct identifier_class;
      using identifier_type = x3::rule<identifier_class, std::string>;
      identifier_type const identifier = "identifier";
      
      auto const identifier_def
       = x3::raw[x3::lexeme[(x3::alpha | '_') >> *(x3::alnum | '_')]]
       ;
      
      BOOST_SPIRIT_DEFINE(identifier);

   } /* namespace parser */
} /* namespace tcg */

#endif /* TCG_IDENTIFIER_H_INCLUDED */
