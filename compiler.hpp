#ifndef TCG_COMPILER_HPP_INCLUDED
#define TCG_COMPILER_HPP_INCLUDED

#include <functional>
#include <boost/assert.hpp>

#include "ast.hpp"
#include "error_handler.hpp"


namespace tcg
{
   namespace code_gen
   {
      namespace x3 = boost::spirit::x3;

      /***************************************************************************
       * Compiler
       ***************************************************************************/
      struct compiler
      {
         using error_handler_type = std::function<void(x3::position_tagged, std::string const&)>;

         //! constructor
         template<class ErrorHandler>
         compiler
          ( const ErrorHandler& error_handler
          )
          : error_handler_
            ( [&](x3::position_tagged pos, const std::string& msg) { error_handler(pos, msg); }
            )
          {
          }

         //! handle nil node in ast
         bool operator()(const ast::nil& x ) const { BOOST_ASSERT(0); return false; }

         //! handle expression nodes in ast
         bool operator()(const double& x) const;
         bool operator()(const unsigned& x) const;
         bool operator()(const bool& x) const;
         bool operator()(const ast::variable& x) const;
         bool operator()(const ast::tensor_litteral& x) const;
         bool operator()(const ast::operation& x) const;
         bool operator()(const ast::unary& x) const;
         bool operator()(const ast::expression& x) const;

         
         //! handle statement nodes in ast
         bool operator()(const ast::statement& x) const;
         bool operator()(const ast::statement_list& x) const;
         bool operator()(const ast::variable_declaration& x) const;
         bool operator()(const ast::assignment& x) const;
         bool operator()(const ast::if_statement& x) const;
         bool operator()(const ast::while_statement& x) const;

         //! start compilation
         bool start(const ast::statement_list& x) const;

         
         error_handler_type error_handler_;
      };
   } /* namespace code_gen */
} /* namespace tcg */

#endif /* TCG_COMPILER_HPP_INCLUDED */
