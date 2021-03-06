#ifndef TCG_CODE_GENERATOR_HPP_INCLUDED
#define TCG_CODE_GENERATOR_HPP_INCLUDED

#include <fstream>
#include <iostream>
#include "compiler.hpp"

namespace tcg
{
   namespace code_gen
   {
      struct tab
      {
         std::string tab_ = "";

         void push()
         {
            tab_ += "   ";
         }

         void pop()
         {
            if(tab_.size() >= 3)
               tab_.erase(tab_.end() - 3, tab_.end());
         }
      };

      inline std::ostream& operator<<(std::ostream& os, const tab& t)
      {
         os << t.tab_;
         return os;
      }

      struct code_generator
      {
         //!
         code_generator
            ( std::ofstream& ofhpp
            , std::ofstream& ofcpp
            , const std::string& filename
            )
            : ofhpp_(ofhpp)
            , ofcpp_(ofcpp)
            , filename_(filename)
         {
         }

         //!
         void write_allocation(const tac_variable&) const;
         void write_deallocation(const tac_variable&) const;
         void write_assignment_loop(ast::optoken, const tac_variable&, const tac_variable&) const;
         void write_permuted_assignment(ast::optoken, const tac_variable&, const tac_variable&, const permutation_type&) const;
         void write_axpy(ast::optoken, const tac_variable&, const tac_variable&) const;
         void write_gemm(const tac_variable&, const tac_variable&, const tac_variable&, const std::tuple<std::string, std::string, std::string>&) const;
         void write_permutation(ast::optoken op, const tac_variable& arg, const tac_variable& permuted_arg) const;
         void write_sum(const tac&) const;
         void write_assignment(const tac&) const;
         void write_contraction(const tac&) const;
         

         //!
         void operator()(const tac&) const;
         void operator()(const tac_function&) const;
         
         //!
         void operator()(const autogen_permutation& t) const;

         //!
         void initialize_files() const;
         void finalize_files() const;
         void generate_code(const intermediate_program&) const;

         std::ofstream& ofhpp_;
         std::ofstream& ofcpp_;
         std::string filename_;
         mutable std::list<tac_variable> allocation_table_;

         mutable std::string size_var_ = "";
         mutable tab tab_;
      };
   }
} /* namespace tcg */

#endif /* TCG_CODE_GENERATOR_HPP_INCLUDED */
