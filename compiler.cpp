#include <iostream>
#include "compiler.hpp"

#include <boost/variant/apply_visitor.hpp> 

namespace tcg
{
   namespace code_gen
   {
      /*!
       *
       */
      bool compiler::operator()(const ast::expression& x) const
      {
         std::cout << " compiling expression " << std::endl;
         return true;
      }

      /*!
       *
       */
      bool compiler::operator()(const ast::statement& x) const
      {
         return boost::apply_visitor(*this, x);
      }

      /*!
       *
       */
      bool compiler::operator()(const ast::statement_list& x) const
      {
         for(const auto& s : x)
         {
            if(!(*this)(s))
            {
               return false;
            }
         }
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::variable_declaration& x) const
      {
         std::cout << " variable_declaration statement " << std::endl;
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::assignment& x) const
      {
         std::cout << " assignment statement " << std::endl;
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::if_statement& x) const
      {
         std::cout << " if statement " << std::endl;
         return true;
      }

      /*!
       *
       */
      bool compiler::operator()(const ast::while_statement& x) const
      {
         std::cout << " while statement " << std::endl;
         return true;
      }

      /*!
       * Start compilation of AST.
       */
      bool compiler::start(const ast::statement_list& x) const
      {
         if(!(*this)(x))
         { 
            return false; // compilation failed return false
         }
         return true;
      }
   } /* namespace code_gen */
} /* namespace tcg */
