#ifndef TCG_CODE_GENERATOR_HPP_INCLUDED
#define TCG_CODE_GENERATOR_HPP_INCLUDED

#include <fstream>
#include "compiler.hpp"

namespace tcg
{
   namespace code_gen
   {
      struct code_generator
      {
         //!
         code_generator
            ( std::ofstream& of
            )
            : of_(of)
         {
         }

         //!
         void write_allocation(const tac_variable&) const;
         void write_deallocation(const tac_variable&) const;
         void write_assignment_loop(ast::optoken, const tac_variable&, const tac_variable&) const;
         void write_permuted_assignment(ast::optoken, const tac_variable&, const tac_variable&, const permutation_type&) const;
         void write_axpy(ast::optoken, const tac_variable&, const tac_variable&) const;
         void write_gemm(const tac_variable&, const tac_variable&, const tac_variable&, const std::tuple<std::string, std::string, std::string>&) const;
         void write_permutation(const tac_variable& arg, const tac_variable& permuted_arg) const;
         void write_sum(const tac&) const;
         void write_assignment(const tac&) const;
         void write_contraction(const tac&) const;
         
         //!
         void generate_code(const tac&) const;
         void generate_code(const tac_program&) const;

         std::ofstream& of_;
         mutable std::list<tac_variable> allocation_table_;

         mutable std::string size_var_ = "";
      };
   }
} /* namespace tcg */

#endif /* TCG_CODE_GENERATOR_HPP_INCLUDED */
