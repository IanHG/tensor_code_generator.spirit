#ifndef TCG_CONFIG_H_INCLUDED
#define TCG_CONFIG_H_INCLUDED

#include <boost/spirit/home/x3.hpp>
#include "error_handler.hpp"

namespace tcg
{
   namespace parser
   {
      using iterator_type = std::string::const_iterator;
      using phrase_context_type = typename x3::phrase_parse_context<x3::ascii::space_type>::type;
      using error_handler_type = error_handler<iterator_type>;

      using context_type = typename x3::with_context
       < error_handler_tag
       , std::reference_wrapper<error_handler_type> const
       , phrase_context_type
       >::type;
   } /* namespace parser */
} /* namespace tcg */

#endif /* TCG_CONFIG_H_INCLUDED */
