#ifndef TCG_AST_H_INCLUDED
#define TCG_AST_H_INCLUDED

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <list>

namespace tcg
{
   namespace ast
   {
      namespace x3 = boost::spirit::x3;

      struct nil
      {
      };

      /***************************************************************************
       * Expression AST
       ***************************************************************************/
      struct unary;
      struct expression;

      struct tensor_litteral: x3::position_tagged
      {
         std::string name_;
         std::vector<char> indices_;
      };
      
      struct variable: x3::position_tagged
      {
         variable(const std::string& name = ""): name_(name) {};
         std::string name_ = "";
      };
      
      struct operand
       : x3::variant
         < nil
         , unsigned int
         , bool
         , double
         , tensor_litteral
         , variable
         , x3::forward_ast<unary>
         , x3::forward_ast<expression>
         >
      {
         using base_type::base_type;
         using base_type::operator=;
      };

      enum optoken
      { op_plus
      , op_minus
      , op_mult
      , op_div
      , op_positive
      , op_negative
      , op_equal
      , op_not_equal
      , op_less
      , op_less_equal
      , op_greater
      , op_greater_equal
      , op_and
      , op_or
      , op_not
      };

      enum functoken
      { func_standard
      , func_cuda
      , func_upc
      };

      struct unary
      {
         optoken operator_;
         operand operand_;
      };

      struct operation: x3::position_tagged
      {
         optoken operator_;
         operand operand_;
      };

      struct expression: x3::position_tagged
      {
         operand first_;
         std::list<operation> rest_;
      };

      /***************************************************************************
       * Statement AST
       ***************************************************************************/
      struct assignment: x3::position_tagged
      {
         //variable lhs_;
         operand lhs_;
         expression rhs_;
      };

      struct variable_declaration
      {
         assignment assign_;
      };
      
      struct if_statement;
      struct while_statement;
      struct statement_list;
      struct function_definition;
      struct autogen_statement;

      struct statement
       : x3::variant
         < variable_declaration
         , assignment
         , x3::forward_ast<if_statement>
         , x3::forward_ast<while_statement>
         , x3::forward_ast<statement_list>
         , x3::forward_ast<function_definition>
         , x3::forward_ast<autogen_statement>
         //, boost::recursive_wrapper<if_statement>
         //, boost::recursive_wrapper<while_statement>
         //, boost::recursive_wrapper<statement_list>
         >
      {
         using base_type::base_type;
         using base_type::operator=;
      };
      
      struct statement_list
         : std::list<statement>
      {
      };

      struct if_statement
      {
         expression condition_;
         statement then_;
         boost::optional<statement> else_;
      };

      struct while_statement
      {
         expression condition_;
         statement body_;
      };

      struct function_definition
      {
         std::string name_;
         boost::optional<functoken> optional_;
         statement body_;
      };

      struct autogen_statement
      {
         std::vector<char> permutation_;
      };

      //struct function_optional
      //{
      //   std::list<std::string> 
      //}

   } /* namespace ast */
} /* namespace tcg */

#endif /* TCG_AST_H_INCLUDED */
