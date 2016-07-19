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
      x3::symbols<ast::optoken> equality_op;
      x3::symbols<ast::optoken> relational_op;
      x3::symbols<ast::optoken> logical_op;
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

         // equality operators
         equality_op.add
            ("==", ast::op_equal)
            ("!=", ast::op_not_equal)
            ;

         // relational operators
         relational_op.add
            ("<",  ast::op_less)
            ("<=", ast::op_less_equal)
            (">",  ast::op_greater)
            (">=", ast::op_greater_equal)
            ;

         // logical opeartors
         logical_op.add
            ("&&", ast::op_and)
            ("||", ast::op_or)
            ;

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
            ("!", ast::op_not)
            ;

         //
         keywords.add
            ("var")
            ("while")
            ("if")
            ("else")
            ("true")
            ("false")
            ;
      }
      
      /***************************************************************************
       * Main expression grammar
       ***************************************************************************/
      // define expr classes
      struct equality_expr_class;
      struct relational_expr_class;
      struct logical_expr_class;
      struct additive_expr_class;
      struct multiplicative_expr_class;
      struct unary_expr_class;
      struct primary_expr_class;
      
      using equality_expr_type = x3::rule<equality_expr_class, ast::expression>;
      using relational_expr_type = x3::rule<relational_expr_class, ast::expression>;
      using logical_expr_type = x3::rule<logical_expr_class, ast::expression>;
      using additive_expr_type = x3::rule<additive_expr_class, ast::expression>;
      using multiplicative_expr_type = x3::rule<multiplicative_expr_class, ast::expression>;
      using unary_expr_type = x3::rule<unary_expr_class, ast::operand>;
      using primary_expr_type = x3::rule<primary_expr_class, ast::operand>;

      const expression_type expression = "expression";
      const equality_expr_type equality_expr = "equality_expr";
      const relational_expr_type relational_expr = "relational_expr";
      const logical_expr_type logical_expr = "logical_expr";
      const additive_expr_type additive_expr = "additive_expr";
      const multiplicative_expr_type multiplicative_expr = "multiplicative_expr";
      const unary_expr_type unary_expr = "unary_expr";
      const primary_expr_type primary_expr = "primary_expr";
      
      // define actual expression grammar
      const auto logical_expr_def
       = equality_expr
      >> *(logical_op > equality_expr)
       ;

      const auto equality_expr_def
       = relational_expr
      >> *(equality_op > relational_expr)
       ;

      const auto relational_expr_def
       = additive_expr
      >> *(relational_op > additive_expr)
       ;

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
       | x3::bool_
       | (!keywords >> identifier)
       | "(" > expression > ")"
       ;

      const auto expression_def = logical_expr;

      BOOST_SPIRIT_DEFINE
       ( expression
       , logical_expr
       , equality_expr
       , relational_expr
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
