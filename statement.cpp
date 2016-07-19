#include "statement_def.hpp"
#include "config.hpp"

namespace tcg
{ 
   namespace parser
   {
      BOOST_SPIRIT_INSTANTIATE(statement_type, iterator_type, context_type);
   } /* namespace parser */
} /* namespace tcg */
