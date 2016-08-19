#ifndef TCG_STATEMENT_DEF_HPP_INCLUDED
#define TCG_STATEMENT_DEF_HPP_INCLUDED

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include "ast.hpp"
#include "ast_adapted.hpp"
#include "statement.hpp"
#include "expression.hpp"
#include "common.hpp"
#include "error_handler.hpp"

namespace tcg
{
   namespace parser
   {
      /***************************************************************************
       * Keywords
       ***************************************************************************/
      struct function_symbols:
         x3::annotate_on_success
       , x3::symbols<ast::functoken> 
      {
      };

      function_symbols                function_optionals;
      x3::symbols<>  autogen_function;

      
      /*!
       * add keywords
       */
      void add_keywords_statement()
      {
         // only allow one call
         static bool once = false;
         if(once) return;
         once = true;

         //
         autogen_function.add
            ("permute")
            ;


         // 
         function_optionals.add
            ("cuda", ast::func_cuda)
            ;
      }

      /***************************************************************************
       * Statement grammar
       ***************************************************************************/
      struct statement_list_class;
      struct statement_explicit_class;
      struct variable_declaration_class;
      struct assignment_class;
      struct variable_class;
      struct block_class;
      struct if_statement_class;
      struct while_statement_class;
      struct function_definition_class;
      struct autogen_statement_class;

      using statement_list_type       = x3::rule<statement_list_class, ast::statement_list>;
      using statement_explicit_type   = x3::rule<statement_explicit_class, ast::statement>;
      using variable_declaration_type = x3::rule<variable_declaration_class, ast::variable_declaration>;
      using assignment_type           = x3::rule<assignment_class, ast::assignment>;
      using variable_type             = x3::rule<variable_class, ast::operand>;
      using block_type                = x3::rule<block_class, ast::statement>;
      using if_statement_type         = x3::rule<if_statement_class, ast::if_statement>;
      using while_statement_type      = x3::rule<while_statement_class, ast::while_statement>;
      using function_definition_type  = x3::rule<function_definition_class, ast::function_definition>;
      using autogen_statement_type    = x3::rule<autogen_statement_class, ast::autogen_statement>;
      
      statement_type const statement("statement");
      statement_list_type const statement_list("statement_list");
      statement_explicit_type const statement_explicit("statement_explicit");
      variable_declaration_type const variable_declaration("variable_declaration");
      assignment_type const assignment("assignment");
      variable_type const variable("variable");
      block_type const block("block");
      if_statement_type const if_statement("if_statement");
      while_statement_type const while_statement("while_statement");
      function_definition_type const function_definition("function_definition");
      autogen_statement_type const autogen_statement("autogen_statement");

      // Import the expression rule
      namespace { auto const& expression = tcg::expression(); }
      
      auto const statement_list_def 
         = +statement_explicit
         ;

      auto const statement_explicit_def
         = (function_definition | autogen_statement | while_statement | if_statement | variable_declaration | assignment)
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
      
      auto const if_statement_def
         = x3::lit("if")
         > x3::lit("(")
         > expression
         > x3::lit(")")
         > block
         >> -( x3::lit("else")
             > block
             )
         ;

      auto const while_statement_def
         = x3::lit("while")
         > x3::lit("(")
         > expression
         > x3::lit(")")
         > block
         ;
      
      auto const block_def
         = x3::lit("{")
         > statement
         > x3::lit("}")
         ;

      auto const function_definition_def
         = x3::lit("function")
         > identifier
         > x3::lit("()")
         > -( x3::lit("[")
            //> (function_optionals >> identifier)
            > (function_optionals)
            > x3::lit("]")
            )
         > block
         ;

      auto const autogen_statement_def
         = x3::lit("autogenerate")
         > autogen_function
         > +(x3::digit)
         > ";"
         ;

      auto const variable_def = tensor_litteral | identifier;
      auto const statement_def = statement_list;
      
      BOOST_SPIRIT_DEFINE
         ( statement
         , statement_list
         , statement_explicit
         , variable_declaration 
         , assignment
         , if_statement
         , while_statement
         , block
         , variable
         , function_definition
         , autogen_statement
         );  
      
      struct function_class : error_handler_base, x3::annotate_on_success {};
      struct statement_class : error_handler_base, x3::annotate_on_success {};
      struct assignment_class : x3::annotate_on_success {};
      struct variable_class : x3::annotate_on_success {};

   } /* namespace parser */
} /* namespace tcg */

namespace tcg
{   
   parser::statement_type const& statement()
   {
      parser::add_keywords_statement();
      return parser::statement;
   }
}

#endif /* TCG_STATEMENT_DEF_HPP_INCLUDED */
