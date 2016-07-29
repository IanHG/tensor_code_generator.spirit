#include "../gen/general.hpp"
#include "0000_generated.hpp"

int main()
{
int i = 4;
int j = 3;
int k = 2;

int sizea = i*j; 
double* A = new double[sizea];
int sizeb = i*k;
double* B = new double[sizeb];
int sizec = i*k;
double* C = new double[sizec];
int sized = k*j;
double* D = new double[sized];

fill_from_file(B, sizeb, "B.mat");
fill_from_file(C, sizec, "C.mat");
fill_from_file(D, sized, "D.mat");

/******
 * FILL IN CODE HERE
 *****/
func1(A, B, C, D, i, j, k);
/******
 * FILL IN CODE HERE END END END
 *****/

double* A_check = new double[sizea];
fill_from_file(A_check, sizea, "A.mat");
auto check = check_result(A, A_check, sizea);

delete[] A;
delete[] A_check;
delete[] B;
delete[] C;
delete[] D;

return (check ? 0 : 1);
};
