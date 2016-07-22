#ifndef TCG_TEST_COMPILER_TEST_HPP_INCLUDED
#define TCG_TEST_COMPILER_TEST_HPP_INCLUDED

#include "../../../libmda/testing/testing_interface.h"

#include "../compiler.hpp"

namespace tcg
{
   namespace test
   {
      /*!
       *
       */
      struct is_permutation_test : public virtual unit_test
      {
         void do_test()
         {
            std::vector<char> vec1{'i', 'j', 'k'};
            std::vector<char> vec2{'j', 'i', 'k'};
            std::vector<char> vec3{'i', 'l'};
            UNIT_ASSERT( tcg::code_gen::is_permutation(vec1, vec2), "is_permutation error.");
            UNIT_ASSERT(!tcg::code_gen::is_permutation(vec1, vec3), "is_permutation error.");
         }
      };
   } /* namespace test */
} /* namespace tcg */

#endif /* TCG_TEST_COMPILER_TEST_HPP_INCLUDED */
