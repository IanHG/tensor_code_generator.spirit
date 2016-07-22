#include "compiler_test.hpp"

int main()
{
   test_suite suite("tcg unit test suite");
   
   suite.add_test<tcg::test::is_permutation_test>("is_permutation_test");
   
   suite.do_tests();

   return 0;
}
