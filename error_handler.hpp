#ifndef TCG_ERROR_HANDLER_H_INCLUDED
#define TCG_ERROR_HANDLER_H_INCLUDED

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include "expression.hpp"
//#include "statement.hpp"

namespace tcg
{
   namespace parser 
   {
      namespace x3 = boost::spirit::x3;

      template <typename Iterator>
      using error_handler = x3::error_handler<Iterator>;

      // tag used to get our error handler from the context
      using error_handler_tag = x3::error_handler_tag;
          
      struct error_handler_base
      {
         template <typename Iterator, typename Exception, typename Context>
         x3::error_handler_result on_error
             ( Iterator& first
             , Iterator const& last
             , Exception const& x
             , Context const& context
             )
            {
               std::string message = "Error! Expecting: " + x.which() + " here:";
               auto& error_handler = x3::get<error_handler_tag>(context).get();
               error_handler(x.where(), message);
               return x3::error_handler_result::fail;
            }
      };
          
   } /* namespace parser */
} /* namespace tcg */

#endif /* TCG_ERROR_HANDLER_H_INCLUDED */
