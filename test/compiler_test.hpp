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
            code_gen::multi_index_type vec1{'i', 'j', 'k'};
            code_gen::multi_index_type vec2{'j', 'i', 'k'};
            code_gen::multi_index_type vec3{'i', 'l'};
            UNIT_ASSERT( code_gen::is_permutation(vec1, vec2), "is_permutation error.");
            UNIT_ASSERT(!code_gen::is_permutation(vec1, vec3), "is_permutation error.");
         }
      };

      /*!
       *
       */
      struct find_overlapping_indices_test : public virtual unit_test
      {
         void do_test()
         {
            code_gen::multi_index_type vec1{'i', 'k'};
            code_gen::multi_index_type vec2{'k', 'j'};
            UNIT_ASSERT_EQUAL( code_gen::find_overlapping_indices(vec1, vec2)
                             , code_gen::multi_index_type{'k'}
                             , "find_overlapping_indices error.");
         }
      };

      /*!
       *
       */
      struct remove_indices_test : public virtual unit_test
      {
         void do_test()
         {
            code_gen::multi_index_type vec1{'i', 'j', 'k'};
            code_gen::multi_index_type vec2{'k'};
            UNIT_ASSERT_EQUAL( code_gen::remove_indices(vec1, vec2)
                             ,(code_gen::multi_index_type{'i', 'j'})
                             , "remove_indices error.");
         }
      };

      /*!
       *
       */
      struct catenate_test : public virtual unit_test
      {
         void do_test()
         {
            code_gen::multi_index_type vec1{'i','j'};
            code_gen::multi_index_type vec2{'k'};
            UNIT_ASSERT_EQUAL( code_gen::catenate(vec1, vec2)
                             ,(code_gen::multi_index_type{'i','j','k'})
                             , "catenate_test error.");
            UNIT_ASSERT_EQUAL( code_gen::catenate(vec2, vec1)
                             ,(code_gen::multi_index_type{'k','i','j'})
                             , "catenate_test error.");
         }
      };

      /*!
       *
       */
      struct find_permutation_test : public virtual unit_test
      {
         void do_test()
         {
            code_gen::multi_index_type vec1{'i','j','k'};
            code_gen::multi_index_type vec2{'k','j','i'};
            UNIT_ASSERT_EQUAL( code_gen::find_permutation(vec1, vec2)
                             ,(code_gen::permutation_type{2, 1, 0})
                             , "find_permuation error.");
            
            code_gen::multi_index_type vec3{'k','i','j'};
            UNIT_ASSERT_EQUAL( code_gen::find_permutation(vec1, vec3)
                             ,(code_gen::permutation_type{2, 0, 1})
                             , "find_permuation error.");
         }
      };

      /*!
       *
       */
      struct is_unit_permutation_test : public virtual unit_test
      {
         void do_test()
         {
            code_gen::permutation_type vec1{0,1,2};
            code_gen::permutation_type vec2{2,0,1};
            UNIT_ASSERT( code_gen::is_unit_permutation(vec1), "unit_permutation_test error.");
            UNIT_ASSERT(!code_gen::is_unit_permutation(vec2), "unit_permutation_test error.");
         }
      };

      /*!
       *
       */
      struct create_permuted_indices_test : public virtual unit_test
      {
         void do_test()
         {
            code_gen::multi_index_type vec1{'i','j','k'};
            code_gen::permutation_type vec2{2,0,1};
            code_gen::permutation_type vec3{0,1,2};
            UNIT_ASSERT_EQUAL( code_gen::create_permuted_indices(vec1, vec2)
                             ,(code_gen::multi_index_type{'k','i','j'})
                             , "create_permuted_indices_test error.");
            UNIT_ASSERT_EQUAL( code_gen::create_permuted_indices(vec1, vec3)
                             ,(code_gen::multi_index_type{'i','j','k'})
                             , "create_permuted_indices_test error.");
         }
      };
   } /* namespace test */
} /* namespace tcg */

#endif /* TCG_TEST_COMPILER_TEST_HPP_INCLUDED */
