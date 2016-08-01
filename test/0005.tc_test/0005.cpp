#include "../gen/general.hpp"
#include "0005_generated.hpp"

int main()
{
int i = 4;
int j = 3;
int k = 2;
int l = 5;
int m = 6;
int n = 7;

int sizea = i*j*k*l; 
double* A = new double[sizea];
int sizeb = i*j*m*n; 
double* B = new double[sizeb];
int sizec = m*n*k*l; 
double* C = new double[sizec];

fill_from_file(B, sizeb, "B.data");
fill_from_file(C, sizec, "C.data");

/******
 * FILL IN CODE HERE
 *****/
func1(A, B, C, i, j, k, l, m, n);
/******
 * FILL IN CODE HERE END END END
 *****/

double* A_check = new double[sizea];
fill_from_file(A_check, sizea, "A.data");
auto check = check_result(A, A_check, sizea, 96);

delete[] A;
delete[] A_check;
delete[] B;
delete[] C;

return (check ? 0 : 1);
};
