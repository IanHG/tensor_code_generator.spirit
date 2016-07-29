#include "../gen/general.hpp"
#include "0001_generated.hpp"

int main()
{
int i = 4;
int j = 3;

int size = i*j; 
double* A = new double[size];
double* B = new double[size];
double* C = new double[size];
double* D = new double[size];

fill_from_file(B, size, "B.mat");
fill_from_file(C, size, "C.mat");
fill_from_file(D, size, "D.mat");

/******
 * FILL IN CODE HERE
 *****/
func1(A, B, C, D, i, j);
/******
 * FILL IN CODE HERE END END END
 *****/

double* A_check = new double[size];
fill_from_file(A_check, size, "A.mat");
auto check = check_result(A, A_check, size);

delete[] A;
delete[] A_check;
delete[] B;
delete[] C;
delete[] D;

return (check ? 0 : 1);
};
