#include "compiler_test.hpp"

int main()
{
   test_suite suite("tcg unit test suite");
   
   suite.add_test<tcg::test::is_permutation_test>("is_permutation_test");
   suite.add_test<tcg::test::find_overlapping_indices_test>("find_overlapping_indices_test");
   suite.add_test<tcg::test::remove_indices_test>("remove_indices_test");
   suite.add_test<tcg::test::catenate_test>("catenate_test");
   suite.add_test<tcg::test::find_permutation_test>("find_permutation_test");
   suite.add_test<tcg::test::is_unit_permutation_test>("is_unit_permutation_test");
   suite.add_test<tcg::test::create_permuted_indices_test>("create_permuted_indices_test");
   
   suite.do_tests();

   return 0;
}
