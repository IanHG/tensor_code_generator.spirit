#include "../gen/general.hpp"
#include "0003_generated.hpp"

int main()
{
int i = 4;
int j = 3;
int k = 2;
int l = 5;

int sizea = i*j*l; 
double* A = new double[sizea];
int sizeb = i*j*k; 
double* B = new double[sizeb];
int sizec = k*l; 
double* C = new double[sizec];

fill_from_file(B, sizeb, "B.mat");
fill_from_file(C, sizec, "C.mat");

/******
 * FILL IN CODE HERE
 *****/
func1(A, B, C, i, j, k,l);
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

return (check ? 0 : 1);
};
