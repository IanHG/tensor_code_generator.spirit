#ifndef TCG_AST_ADAPTED_H_INCLUDED
#define TCG_AST_ADAPTED_H_INCLUDED

#include "ast.hpp"
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT
 ( tcg::ast::tensor_litteral
 , name_, indices_
 )

BOOST_FUSION_ADAPT_STRUCT
 ( tcg::ast::unary
 , operator_, operand_
 )

BOOST_FUSION_ADAPT_STRUCT
 ( tcg::ast::operation
 , operator_, operand_
 )

BOOST_FUSION_ADAPT_STRUCT
 ( tcg::ast::expression
 , first_, rest_
 )

BOOST_FUSION_ADAPT_STRUCT
 ( tcg::ast::variable_declaration
 , assign_
 )

BOOST_FUSION_ADAPT_STRUCT
 ( tcg::ast::assignment
 , lhs_, rhs_
 )

BOOST_FUSION_ADAPT_STRUCT
 ( tcg::ast::if_statement
 , condition_, then_, else_
 )

BOOST_FUSION_ADAPT_STRUCT
 ( tcg::ast::while_statement
 , condition_, body_
 )

#endif /* TCG_AST_ADAPTED_H_INCLUDED */
