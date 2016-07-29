#ifndef TCG_TEST_GENERAL_HPP_INCLUDED
#define TCG_TEST_GENERAL_HPP_INCLUDED

#include <string>

//! fill pointer from file
void fill_from_file(double* pd, int size, const std::string& filename);

//void print(double* pd, int size);

//! check against result
bool check_result(double* d, double* e, int size);

#endif /* TCG_TEST_GENERAL_HPP_INCLUDED */
