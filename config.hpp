#ifndef TCG_CONFIG_H_INCLUDED
#define TCG_CONFIG_H_INCLUDED

#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/home/x3.hpp>
#include "error_handler.hpp"

namespace tcg
{
   namespace parser
   {
      using iterator_type = std::string::const_iterator;
      //using phrase_context_type = typename x3::phrase_parse_context<x3::ascii::space_type>::type;
      
      static auto skipper 
         = boost::spirit::x3::ascii::space // skip space
         | boost::spirit::x3::lit("/*") > *(boost::spirit::x3::char_ - "*/") > boost::spirit::x3::lit("*/") // c style comments
         | boost::spirit::x3::lit("//") > *(boost::spirit::x3::char_ - boost::spirit::x3::eol) >> boost::spirit::x3::eol // cpp style comments
         ;
      
      using phrase_context_type = typename x3::phrase_parse_context<decltype(skipper)>::type;
      using error_handler_type = error_handler<iterator_type>;

      using context_type = typename x3::with_context
       < error_handler_tag
       , std::reference_wrapper<error_handler_type> const
       , phrase_context_type
       >::type;
      
   } /* namespace parser */
} /* namespace tcg */

#endif /* TCG_CONFIG_H_INCLUDED */
