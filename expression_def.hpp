#ifndef TCG_EXPRESSION_DEF_H_INCLUDED
#define TCG_EXPRESSION_DEF_H_INCLUDED

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include "ast.hpp"
#include "ast_adapted.hpp"
#include "expression.hpp"
#include "identifier.hpp"
#include "error_handler.hpp"

namespace tcg
{
   namespace parser
   {
      /***************************************************************************
       * Tokens
       ***************************************************************************/
      x3::symbols<ast::optoken> additive_op;
      x3::symbols<ast::optoken> multiplicative_op;
      x3::symbols<ast::optoken> unary_op;
      x3::symbols<> keywords;
      
      /*!
       * add keywords
       */
      void add_keywords()
      {
         // only allow one call
         static bool once = false;
         if(once) return;
         once = true;

         // additive operators
         additive_op.add
            ("+", ast::op_plus)
            ("-", ast::op_minus)
            ;

         // multiplicative operators
         multiplicative_op.add
            ("*", ast::op_mult)
            ("/", ast::op_div)
            ;
         
         // unary operators
         unary_op.add
            ("+", ast::op_positive)
            ("-", ast::op_negative)
            ;

         //
         keywords.add
            ("var")
            ;
      }
      
      /***************************************************************************
       * Main expression grammar
       ***************************************************************************/
      // define expr classes
      struct additive_expr_class;
      struct multiplicative_expr_class;
      struct unary_expr_class;
      struct primary_expr_class;

      using additive_expr_type = x3::rule<additive_expr_class, ast::expression>;
      using multiplicative_expr_type = x3::rule<multiplicative_expr_class, ast::expression>;
      using unary_expr_type = x3::rule<unary_expr_class, ast::operand>;
      using primary_expr_type = x3::rule<primary_expr_class, ast::operand>;

      const expression_type expression = "expression";
      const additive_expr_type additive_expr = "additive_expr";
      const multiplicative_expr_type multiplicative_expr = "multiplicative_expr";
      const unary_expr_type unary_expr = "unary_expr";
      const primary_expr_type primary_expr = "primary_expr";
      
      // define actual expression grammar
      const auto additive_expr_def
       = multiplicative_expr
      >> *(additive_op > multiplicative_expr)
       ;

      const auto multiplicative_expr_def
       = unary_expr
      >> *(multiplicative_op > unary_expr)
       ;

      const auto unary_expr_def
       = primary_expr
       | (unary_op > primary_expr)
       ;

      const auto primary_expr_def
       = x3::uint_
       | (!keywords >> identifier)
       | "(" > expression > ")"
       ;

      const auto expression_def = additive_expr_def;

      BOOST_SPIRIT_DEFINE
       ( expression
       , additive_expr
       , multiplicative_expr
       , unary_expr
       , primary_expr
       );
      
      // hmm?
      struct unary_expr_class : x3::annotate_on_success {};
      struct primary_expr_class : x3::annotate_on_success {};
   } /* namespace parser */
   
   /***************************************************************************
    * Interface
    ***************************************************************************/
   parser::expression_type const& expression()
   {
      parser::add_keywords();
      return parser::expression;
   }

} /* namespace tcg */
#endif /* TCG_EXPRESSION_DEF_H_INCLUDED */
