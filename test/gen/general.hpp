#ifndef TCG_TEST_GENERAL_HPP_INCLUDED
#define TCG_TEST_GENERAL_HPP_INCLUDED

#include <string>

//! fill pointer from file
void fill_from_file(double* pd, int size, const std::string& filename);

//! check against result
bool check_result(double* d, double* e, int size, int ulps = 2);

//! print pointer (for debug)
void print(double* pd, int size);

#endif /* TCG_TEST_GENERAL_HPP_INCLUDED */
