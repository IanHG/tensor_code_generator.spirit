#ifndef TCG_STATEMENT_HPP_INCLUDED
#define TCG_STATEMENT_HPP_INCLUDED

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace tcg
{
   namespace x3 = boost::spirit::x3;
   namespace parser
   {
      struct statement_class;
      typedef x3::rule<statement_class, ast::statement_list> statement_type;
      typedef statement_type::id statement_id;
      BOOST_SPIRIT_DECLARE(statement_type);
   } /* namespace parser */
   
   parser::statement_type const& statement();
} /* namespace tcg */

#endif /* TCG_STATEMENT_HPP_INCLUDED */
