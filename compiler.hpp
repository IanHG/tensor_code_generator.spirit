#ifndef TCG_COMPILER_HPP_INCLUDED
#define TCG_COMPILER_HPP_INCLUDED

#include "ast.hpp"
#include "error_handler.hpp"

#include <boost/assert.hpp>

namespace tcg
{
   namespace code_gen
   {
      /***************************************************************************
       * Compiler
       ***************************************************************************/
      struct compiler
      {
         bool operator()(const ast::nil& x ) const { BOOST_ASSERT(0); return false; }

         //! handle expression nodes in ast
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
      };
   } /* namespace code_gen */
} /* namespace tcg */

#endif /* TCG_COMPILER_HPP_INCLUDED */
