#ifndef TCG_STATEMENT_DEF_HPP_INCLUDED
#define TCG_STATEMENT_DEF_HPP_INCLUDED

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include "ast.hpp"
#include "ast_adapted.hpp"
#include "statement.hpp"
#include "expression.hpp"
#include "identifier.hpp"
#include "error_handler.hpp"

namespace tcg
{
   namespace parser
   {
      /***************************************************************************
       * Statement grammar
       ***************************************************************************/
      struct statement_list_class;
      struct variable_declaration_class;
      struct assignment_class;
      struct variable_class;

      using statement_list_type       = x3::rule<statement_list_class, ast::statement_list>;
      using variable_declaration_type = x3::rule<variable_declaration_class, ast::variable_declaration>;
      using assignment_type           = x3::rule<assignment_class, ast::assignment>;
      using variable_type             = x3::rule<variable_class, ast::variable>;
      
      statement_type const statement("statement");
      statement_list_type const statement_list("statement_list");
      variable_declaration_type const variable_declaration("variable_declaration");
      assignment_type const assignment("assignment");
      variable_type const variable("variable");

      // Import the expression rule
      namespace { auto const& expression = tcg::expression(); }
      
      auto const statement_list_def 
       = +(variable_declaration | assignment)
       ;
      
      auto const variable_declaration_def
       = x3::lexeme["var" >> !(x3::alnum | '_')] // make sure we have whole words
       > assignment
       ;
      
      auto const assignment_def
       = variable
       > '='
       > expression
       > ';'
       ;
      
      auto const variable_def = identifier;
      auto const statement_def = statement_list;
      
      BOOST_SPIRIT_DEFINE
       ( statement
       , statement_list
       , variable_declaration 
       , assignment
       , variable
       );  
      
      struct statement_class : error_handler_base, x3::annotate_on_success {};
      struct assignment_class : x3::annotate_on_success {};
      struct variable_class : x3::annotate_on_success {};

   } /* namespace parser */
} /* namespace tcg */

namespace tcg
{   
   parser::statement_type const& statement()
   {
      return parser::statement;
   }
}

#endif /* TCG_STATEMENT_DEF_HPP_INCLUDED */
