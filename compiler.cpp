#include <iostream>
#include "compiler.hpp"

#include <boost/variant/apply_visitor.hpp> 

namespace tcg
{
   namespace code_gen
   {
      /***************************************************************************
       * 
       ***************************************************************************/
      std::string tensor_intermed::create_guid()
      {
         static int i = 0;
         return {"intermed" + std::to_string(++i)};
      }

      bool operator==(const tensor_intermed& t1, const tensor_intermed& t2)
      {
         return t1.guid_ == t2.guid_;
      }

      bool is_permutation(const multi_index_type& v1, const multi_index_type& v2)
      {
         if(v1.size() != v2.size()) return false;

         for(const auto& c1 : v1)
         {
            bool found = false;
            for(const auto& c2 : v2)
            {
               if(c1 == c2)
               {
                  found = true;
                  break;
               }
            }
            if(!found) return false;
         }
         return true;
      }
      
      /***************************************************************************
       * TAC program
       ***************************************************************************/
      void tac_program::op(ast::optoken op)
      {
         switch(op)
         {
            case ast::op_plus:
            {
               auto v1 = variable_stack_.top(); variable_stack_.pop();
               auto v2 = variable_stack_.top(); variable_stack_.pop();
               tac t(tensor_intermed::create_guid(), v1, v2, op);
               break;
            }
            default: 
            {
               BOOST_ASSERT(0);
               break;
            }
         }
      }

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
         if (!boost::apply_visitor(*this, x.operand_))
            return false;
         switch (x.operator_)
         {
            case ast::op_negative: std::cout << " op_negative " << std::endl; break;
            case ast::op_not: std::cout << " op_not " << std::endl; break;
            case ast::op_positive: break;
            default: BOOST_ASSERT(0); return false;
         }
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
