#include "code_generator.hpp"

#include <iostream>

namespace tcg
{
   namespace code_gen
   {
      std::string op_as_string(ast::optoken op)
      {
         switch(op)
         {
            case ast::op_plus:  return "+";
            case ast::op_minus: return "-";
            case ast::op_mult:  return "*";
            case ast::op_div:   return "/";
            case ast::op_equal: return "=";
            default: BOOST_ASSERT(0);

         }
      }

      std::string index_prefix()
      {
         //return "tcg_extent_";
         return "";
      }

      std::string new_line()
      {
         return "\n";
      }

      std::string multi_index_multiplication(const multi_index_type& m)
      {
         std::string str;
         for(size_t i = 0; i < m.size() - 1; ++i)
         {
            str += index_prefix() + m[i] + "*";
         }
         if(m.size() > 0)
         {
            str += index_prefix() + m[m.size() - 1];
         }
         return str;
      }

      /*!
       *
       */
      void code_generator::write_allocation(const tac_variable& t) const
      {
         std::string type = type_as_string(t);
         std::string name = t.name_;
         std::string size = size_var_ != "" ? size_var_ : multi_index_multiplication(t.indices_);
         of_ << type << "* " << name << " = new " << type 
             << "[" << size << "];" << new_line();
      }

      /*!
       *
       */
      void code_generator::write_deallocation(const tac_variable& t) const
      {
         of_ << "delete[] " << t.name_ << ";" << new_line();
      }

      /*!
       *
       */
      void code_generator::write_assignment_loop
         ( ast::optoken op
         , const tac_variable& arg1
         , const tac_variable& result
         ) const
      {
         if(size_var_ == "") BOOST_ASSERT(0);
         of_ << "for(int isize = 0; isize < " << size_var_ << "; ++isize) \n"
             << "{\n"
             << "   " << result.name_ << "[isize] " << op_as_string(op) << " " << arg1.name_ << "[isize];" << new_line()
             << "}\n";
      }

      /*!
       *
       */
      void code_generator::write_axpy
         ( ast::optoken op
         , const tac_variable& arg1
         , const tac_variable& arg2
         ) const
      {
         auto type = type_as_string(arg1);
         of_ << "{" << new_line()
             << type << " alpha = ";
         switch(op)
         {
            case ast::op_plus:
               of_ << "1.0";
               break;
            case ast::op_minus:
               of_ << "-1.0";
               break;
            default:
               BOOST_ASSERT(0);
         }
         of_ << ";" << new_line()
             << "int incx = 1;" << new_line()
             << "int incy = 1;" << new_line()
             << "daxpy(&" << size_var_ << ", &alpha, " << arg1.name_ << ", &incx, " << arg2.name_ << ", &incy);" << new_line()
             << "}" << new_line();
      }

      /*!
       *
       */
      void code_generator::write_sum(const tac& t) const
      {
         size_var_ = "size" + tensor_intermed::create_guid();
         of_ << "int " << size_var_ << " = " << multi_index_multiplication(t.arg1_.indices_) << ";" << new_line(); 
         if(is_temporary(t.result_))
         {
            allocation_table_.push_back(t.result_);
            write_allocation(t.result_);
         }
         write_assignment_loop(ast::op_equal, t.arg1_, t.result_);
         write_axpy(t.op_, t.arg2_, t.result_);
         size_var_ = "";
      }

      /*!
       *
       */
      void code_generator::write_gemm
         ( const tac_variable& arg1
         , const tac_variable& arg2
         , const tac_variable& result
         , const std::tuple<std::string, std::string, std::string>& extents
         ) const
      {
         auto type = type_as_string(arg1);
         of_ << "{" << new_line()
             << "   char tcg_gemm_transa = 'N';" << new_line()
             << "   char tcg_gemm_transb = 'N';" << new_line()
             << "   " << type << " tcg_gemm_alpha = 1.0; " << new_line()
             << "   " << type << " tcg_gemm_beta  = 0.0; " << new_line()
             << "   int tcg_gemm_m = " << std::get<0>(extents) << ";" << new_line()
             << "   int tcg_gemm_n = " << std::get<1>(extents) << ";" << new_line()
             << "   int tcg_gemm_k = " << std::get<2>(extents) << ";" << new_line()
             << "   int lda = std::max(1, tcg_gemm_m);" << new_line()
             << "   int ldb = std::max(1, tcg_gemm_k);" << new_line()
             << "   int ldc = std::max(1, tcg_gemm_m);" << new_line()
             << "   dgemm(&tcg_gemm_transa, &tcg_gemm_transb"
             << ", &tcg_gemm_m, &tcg_gemm_n, &tcg_gemm_k, &tcg_gemm_alpha, " 
             << arg1.name_ << ", &lda, " << arg2.name_ 
             << ", &ldb, &tgc_gemm_beta, " << result.name_ << ", &ldc);" << new_line()
             << "}" << new_line();
      }

      /*!
       *
       */
      void code_generator::write_contraction(const tac& t) const
      {
         if(is_temporary(t.result_))
         {
            allocation_table_.push_back(t.result_);
            write_allocation(t.result_);
         }
         
         // find contraction indices
         auto overlap_indices = find_overlapping_indices(t.arg2_.indices_, t.arg1_.indices_);
         auto arg1_remaining_indices = remove_indices(t.arg1_.indices_, overlap_indices);
         auto arg2_remaining_indices = remove_indices(t.arg2_.indices_, overlap_indices);
      
         auto arg1_indices = catenate(arg1_remaining_indices, overlap_indices);
         auto arg2_indices = catenate(overlap_indices, arg2_remaining_indices);

         auto arg1_permutation = find_permutation(arg1_indices, t.arg1_.indices_);
         auto arg2_permutation = find_permutation(arg2_indices, t.arg2_.indices_);

         if(!is_unit_permutation(arg1_permutation) || !is_unit_permutation(arg2_permutation))
         {
            BOOST_ASSERT(0);
         }
         
         auto extent1 = multi_index_multiplication(arg1_remaining_indices);
         auto extent2 = multi_index_multiplication(arg2_remaining_indices);
         auto extent3 = multi_index_multiplication(overlap_indices);
         std::tuple<std::string, std::string, std::string> extents{ extent1
                                                                  , extent2
                                                                  , extent3
                                                                  };
         write_gemm(t.arg1_, t.arg2_, t.result_, extents);
      }

      /*!
       *
       */
      void code_generator::write_assignment(const tac& t) const
      {
         size_var_ = "size" + tensor_intermed::create_guid();
         of_ << "{" << new_line()
             << "int " << size_var_ << " = " << multi_index_multiplication(t.arg1_.indices_) << ";" << new_line(); 
         write_assignment_loop(t.op_, t.arg1_, t.result_);
         of_ << "}" << new_line();
         size_var_ = "";
      }

      /*!
       *
       */
      void code_generator::generate_code(const tac& t) const
      {
         switch(t.op_)
         {
            case ast::op_plus:
            case ast::op_minus:
               write_sum(t);
               break;
            case ast::op_mult:
               write_contraction(t);
               break;
            case ast::op_equal:
               write_assignment(t);
               break;
            default:
               std::cout << " lol default " << std::endl;
               //BOOST_ASSERT(0); break;
         }
      }

      /*!
       *
       */
      void code_generator::generate_code(const tac_program& t) const
      {
         for(const auto& line : t)
         {
            generate_code(line);
         }
         for(const auto& x : allocation_table_)
         {
            write_deallocation(x);
         }
         allocation_table_.clear();
      }
   } /* namespace code_gen */
} /* namespace tcg */
