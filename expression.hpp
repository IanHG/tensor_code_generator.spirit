#ifndef TCG_EXPRESSION_H_INCLUDED
#define TCG_EXPRESSION_H_INCLUDED

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace tcg
{
   namespace parser
   {
      namespace x3 = boost::spirit::x3;
      struct expression_class;
      using expression_type = x3::rule<expression_class, ast::expression>;

      BOOST_SPIRIT_DECLARE(expression_type);
   } /* namespace parser */

   parser::expression_type const& expression();

} /* namespace tcg */

#endif /* TCG_EXPRESSION_H_INCLUDED */
