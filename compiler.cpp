#include <iostream>
#include "compiler.hpp"

#include <boost/variant/apply_visitor.hpp> 

namespace tcg
{
   namespace code_gen
   {
      /***************************************************************************
       * Compile expressions
       ***************************************************************************/
      /*!
       *
       */
      bool compiler::operator()(const double& x) const
      {
         std::cout << " double " << x << std::endl;
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const unsigned& x) const
      {
         std::cout << " unsigned " << x << std::endl;
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const bool& x) const
      {
         std::cout << " bool " << x << std::endl;
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::variable& x) const
      {
         std::cout << " variable " << x.name_ << std::endl;
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::tensor_litteral& x) const
      {
         std::cout << " tensor " << x.name_ << "  ";
         for(int i = 0; i < x.indices_.size(); ++i)
         {
            std::cout << " " << x.indices_[i];
         }
         std::cout << std::endl;
         return true;
      }

      /*!
       *
       */
      bool compiler::operator()(const ast::operation& x) const
      {
         std::cout << " operation " << std::endl;
         if(!boost::apply_visitor(*this, x.operand_))
         {
            return false;
         }
         switch(x.operator_)
         {
            case ast::op_plus: std::cout << " + " << std::endl; break;
            case ast::op_mult: std::cout << " * " << std::endl; break;
            default: BOOST_ASSERT(0); return false;
         }
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::unary& x) const
      {
         std::cout << " unary " << std::endl;
         return true;
      }

      /*!
       *
       */
      bool compiler::operator()(const ast::expression& x) const
      {
         if(!boost::apply_visitor(*this, x.first_))
         {
            return false;
         }
         for(const auto& oper : x.rest_)
         {
            if(!(*this)(oper))
            {
               return false;
            }
         }
         return true;
      }
      
      
      /***************************************************************************
       * Compile statements
       ***************************************************************************/
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
         std::cout << " variable declaration not implemented yet in compiler " << std::endl;
         BOOST_ASSERT(0);
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::assignment& x) const
      {
         std::cout << " assignment statement " << std::endl;
         if(!(*this)(x.rhs_))
         {
            return false;
         }
         if(!boost::apply_visitor(*this, x.lhs_))
         {
            return false;
         }
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::if_statement& x) const
      {
         std::cout << " if statement not implemented yet in compiler " << std::endl;
         BOOST_ASSERT(0);
         return true;
      }

      /*!
       *
       */
      bool compiler::operator()(const ast::while_statement& x) const
      {
         std::cout << " while statement not implemented yet in compiler " << std::endl;
         BOOST_ASSERT(0);
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
