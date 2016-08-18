#include "../gen/general.hpp"
#include "0010_generated.hpp"

int main()
{
int i = 4;
int j = 3;

int size = i*j;
double* A = new double[size];
double* B = new double[size];
double* C = new double[size];
double* D = new double[size];

fill_from_file(B, size, "B.data");
fill_from_file(C, size, "C.data");
fill_from_file(D, size, "D.data");

/******
 * FILL IN CODE HERE
 *****/
func1(A, B, C, D, i, j);
/******
 * FILL IN CODE HERE END END END
 *****/

double* A_check = new double[size];
fill_from_file(A_check, size, "A.data");
auto check = check_result(A, A_check, size, 2);

delete[] A;
delete[] A_check;
delete[] B;
delete[] C;
delete[] D;

return (check ? 0 : 1);
};
