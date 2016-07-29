#include <iostream>
#include <chrono>
#include <limits>
#include <fstream>
#include "../../../../libmda/arrays/SDArray2D.h"
#include "../../../../libmda/util/rand_seed.h"

using mat_type = midas::mmv::SDArray2D<double>;

void fill_random(mat_type& mat)
{
   for(int j = 0; j < mat.extent<1>(); ++j)
   {
      for(int i = 0; i < mat.extent<0>(); ++i)
      {
         mat[i][j] = libmda::util::rand_signed_float<double>();
      }
   }
}

void print(const mat_type& mat, const std::string& str)
{
   std::ofstream of(str);
   using limit_type = std::numeric_limits<typename mat_type::value_type>;
   of << std::scientific;
   of << std::setprecision(limit_type::max_digits10);
   for(int j = 0 ; j < mat.extent<1>(); ++j)
      for(int i = 0 ; i < mat.extent<0>(); ++i)
   {
      of << mat.at(i,j) << std::endl;
   }
}

int main()
{
   int ii = 4;
   int jj = 3;
   int kk = 2;

   mat_type matA(jj,ii);
   mat_type matB(ii,kk);
   mat_type matC(kk,ii);
   mat_type matD(jj,kk);
   mat_type matE(ii,kk);

   fill_random(matB);
   fill_random(matC);
   fill_random(matD);
   
   libmda::cindex<'i'> i;
   libmda::cindex<'j'> j;
   libmda::cindex<'k'> k;
   
   auto start = std::chrono::steady_clock::now();
   //matE(i,k)  = matB(i,k); 
   //matE(i,k) += matC(k,i);
   //matA(j,i)  = matE(i,k) * matD(j,k);
   matA(j,i) = (matB(i,k) + matC(k,i)) * matD(j,k);
   auto stop = std::chrono::steady_clock::now();
   auto diff = stop - start;
   std::cout << std::chrono::duration<double, std::milli>(diff).count() << "ms" << std::endl; 

   print(matA, "A.mat");
   print(matB, "B.mat");
   print(matC, "C.mat");
   print(matD, "D.mat");

   return 0;
}
