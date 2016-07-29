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

      multi_index_type contraction_result_indices(const multi_index_type& m1, const multi_index_type& m2)
      {
         multi_index_type result_indices;
         for(const auto& x : m1)
         {
            bool found = false;
            for(const auto& y : m2)
            {
               if(x == y) 
               {
                  found = true;
                  break;
               }
            }
            if(!found) result_indices.emplace_back(x);
         }
         for(const auto& x : m2)
         {
            bool found = false;
            for(const auto& y : m1)
            {
               if(x == y) 
               {
                  found = true;
                  break;
               }
            }
            if(!found) result_indices.emplace_back(x);
         }
         return result_indices;
      }

      multi_index_type find_overlapping_indices
         ( const multi_index_type& m1
         , const multi_index_type& m2
         )
      {
         multi_index_type overlap;
         for(const auto& x : m1)
         {
            for(const auto& y : m2)
            {
               if(x == y)
               {
                  overlap.emplace_back(x);
                  break;
               }
            }
         }
         return overlap;
      }

      multi_index_type remove_indices
         ( const multi_index_type& m1
         , const multi_index_type& m2
         )
      {
         multi_index_type removed_indices;
         for(const auto& x : m1)
         {
            bool found = false;
            for(const auto& y : m2)
            {
               if(x == y)
               {
                  found = true;
                  break;
               }
            }
            if(!found)
            {
               removed_indices.emplace_back(x);
            }
         }
         return removed_indices;
      }

      multi_index_type catenate
         ( const multi_index_type& m1
         , const multi_index_type& m2
         )
      {
         multi_index_type catenation;
         catenation.reserve(m1.size() + m2.size());
         for(const auto& x : m1)
         {
            catenation.emplace_back(x);
         }
         for(const auto& x : m2)
         {
            catenation.emplace_back(x);
         }
         return catenation;
      }

      permutation_type find_permutation
         ( const multi_index_type& m1
         , const multi_index_type& m2
         )
      {
         permutation_type permutation;
         for(const auto& x : m2)
         {
            int position = 0;
            bool found = false;
            for(const auto& y : m1)
            {
               if(x == y)
               {
                  permutation.emplace_back(position);
                  found = true;
                  break;
               }
               ++position;
            }
            if(!found)
            {
               BOOST_ASSERT(0);
            }
         }
         return permutation;
      }

      bool is_unit_permutation(const permutation_type& p)
      {
         int position = 0;
         for(const auto& x : p)
         {
            if(x != position)
            {
               return false;     
            }
            ++position;
         }
         return true;
      }
      
      multi_index_type create_permuted_indices(const multi_index_type& m, const permutation_type& p)
      {
         assert(m.size() == p.size());
         multi_index_type permuted_indices;
         for(const auto& i : p)
         {
            permuted_indices.emplace_back(m[i]);
         }
         return permuted_indices;
      }
      /***************************************************************************
       * TAC program
       ***************************************************************************/
      void intermediate_program::add_variable(const tac_variable& v) 
      { 
         variable_stack_.push(v);
         if(!symbol_table_.get(v.name_))
         {
            symbol sym;
            sym.name_ = v.name_;
            sym.utype_ = v.utype_;
            sym.ptype_ = v.ptype_;
            sym.pflag_ = v.pflag_;
            symbol_table_.put(sym.name_, sym); 
         }
         for(size_t i = 0; i < v.indices_.size(); ++i)
         {
            std::string idx = std::string(1, v.indices_[i]);
            if(!symbol_table_.get(idx))
            {
               symbol sym;
               sym.name_ = idx;
               sym.utype_ = 'i';
               sym.ptype_ = 'v';
               sym.pflag_ = 'p';
               symbol_table_.put(sym.name_, sym); 
            }
         }
      }

      void intermediate_program::op(ast::optoken op)
      {
         switch(op)
         {
            case ast::op_plus:
            case ast::op_minus:
            {
               auto v1 = variable_stack_.top(); variable_stack_.pop();
               auto v2 = variable_stack_.top(); variable_stack_.pop();
               if(!is_permutation(v1.indices_, v2.indices_)) BOOST_ASSERT(0);
               auto result = tac_variable(tensor_intermed::create_guid(), v2.indices_);
               this->emplace_back(tac(op, v2, v1, result));
               this->add_variable(result);
               break;
            }
            case ast::op_mult:
            case ast::op_div:
            {
               auto v1 = variable_stack_.top(); variable_stack_.pop();
               auto v2 = variable_stack_.top(); variable_stack_.pop();
               auto result_indices = contraction_result_indices(v2.indices_, v1.indices_);
               auto result = tac_variable(tensor_intermed::create_guid(), result_indices);
               this->emplace_back(tac(op, v2, v1, result));
               this->add_variable(result);
               break;
            }
            case ast::op_equal:
            {
               auto v1 = variable_stack_.top(); variable_stack_.pop();
               auto v2 = variable_stack_.top(); variable_stack_.pop();
               if(!is_permutation(v1.indices_, v2.indices_)) BOOST_ASSERT(0);
               this->emplace_back(tac(op, v2, tac_variable(), v1));
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
      bool compiler::operator()(const double& x, intermediate_program& prog) const
      {
         std::cout << " double " << x << std::endl;
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const unsigned& x, intermediate_program& prog) const
      {
         std::cout << " unsigned " << x << std::endl;
         BOOST_ASSERT(0);
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const bool& x, intermediate_program& prog) const
      {
         std::cout << " bool " << x << std::endl;
         BOOST_ASSERT(0);
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::variable& x, intermediate_program& prog) const
      {
         std::cout << " variable " << x.name_ << std::endl;
         BOOST_ASSERT(0);
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::tensor_litteral& x, intermediate_program& prog) const
      {
         std::cout << " tensor " << x.name_ << "  ";
         for(size_t i = 0; i < x.indices_.size(); ++i)
         {
            std::cout << " " << x.indices_[i];
         }
         std::cout << std::endl;
         tac_variable t(x.name_, x.indices_, 'p');
         prog.add_variable(t);
         return true;
      }

      /*!
       *
       */
      bool compiler::operator()(const ast::operation& x, intermediate_program& prog) const
      {
         std::cout << " operation " << std::endl;
         //if(!boost::apply_visitor(*this, x.operand_))
         if(!boost::apply_visitor([&](auto& x){ return (*this)(x, prog); }, x.operand_))
         {
            return false;
         }
         switch(x.operator_)
         {
            case ast::op_plus: 
            case ast::op_minus: 
            case ast::op_mult: 
            case ast::op_div:
            case ast::op_equal:
               prog.op(x.operator_);
               break;
            default: BOOST_ASSERT(0); return false;
         }
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::unary& x, intermediate_program& prog) const
      {
         std::cout << " unary " << std::endl;
         BOOST_ASSERT(0);
         //if(!boost::apply_visitor(*this, x.operand_))
         if(!boost::apply_visitor([&](auto& x){ return (*this)(x, prog); }, x.operand_))
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
      bool compiler::operator()(const ast::expression& x, intermediate_program& prog) const
      {
         //if(!boost::apply_visitor(*this, x.first_))
         if(!boost::apply_visitor([&](auto& x){ return (*this)(x, prog); }, x.first_))
         {
            return false;
         }
         for(const auto& oper : x.rest_)
         {
            if(!(*this)(oper, prog))
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
      bool compiler::operator()(const ast::statement& x, intermediate_program& prog) const
      {
         //return boost::apply_visitor(*this, x);
         return boost::apply_visitor([&](auto& x){ return (*this)(x, prog); }, x);
      }

      /*!
       *
       */
      bool compiler::operator()(const ast::statement_list& x, intermediate_program& prog) const
      {
         for(const auto& s : x)
         {
            if(!(*this)(s, prog))
            {
               return false;
            }
         }
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::variable_declaration& x, intermediate_program& prog) const
      {
         std::cout << " variable declaration not implemented yet in compiler " << std::endl;
         BOOST_ASSERT(0);
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::assignment& x, intermediate_program& prog) const
      {
         std::cout << " assignment statement " << std::endl;
         if(!(*this)(x.rhs_, prog))
         {
            return false;
         }
         //if(!boost::apply_visitor(*this, x.lhs_)
         if(!boost::apply_visitor([&](auto& x){ return (*this)(x, prog); }, x.lhs_))
         {
            return false;
         }
         prog.op(ast::op_equal);
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::if_statement& x, intermediate_program& prog) const
      {
         std::cout << " if statement not implemented yet in compiler " << std::endl;
         BOOST_ASSERT(0);
         return true;
      }

      /*!
       *
       */
      bool compiler::operator()(const ast::while_statement& x, intermediate_program& prog) const
      {
         std::cout << " while statement not implemented yet in compiler " << std::endl;
         BOOST_ASSERT(0);
         return true;
      }
      
      /*!
       *
       */
      bool compiler::operator()(const ast::function_definition& x, intermediate_program& prog) const
      {
         //std::cout << " function definition not implemented yet in compiler " << std::endl;
         //BOOST_ASSERT(0);
         tac_function f(x.name_, &prog.symbol_table_);
         if(!(*this)(x.body_, f.program_))
         {
            return false;
         }
         prog.emplace_back(f);
         return true;
      }

      /*!
       * Start compilation of AST.
       */
      bool compiler::start(const ast::statement_list& x) const
      {
         if(!(*this)(x, intermediate_program_))
         { 
            return false; // compilation failed return false
         }
         return true;
      }
   } /* namespace code_gen */
} /* namespace tcg */
