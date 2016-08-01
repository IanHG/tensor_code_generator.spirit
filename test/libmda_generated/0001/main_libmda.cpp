#include <iostream>
#include <chrono>
#include <limits>
#include <fstream>
#include "../../../../../libmda/arrays/SDArray2D.h"
#include "../../../../../libmda/util/rand_seed.h"

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
   of << std::showpos;
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

   mat_type matA(ii,jj);
   mat_type matB(ii,jj);
   mat_type matC(ii,jj);
   mat_type matD(ii,jj);
   mat_type matE(ii,jj);

   fill_random(matB);
   fill_random(matC);
   fill_random(matD);
   
   libmda::cindex<'i'> i;
   libmda::cindex<'j'> j;
   libmda::cindex<'k'> k;
   
   auto start = std::chrono::steady_clock::now();
   matA(i,j) = matB(i,j) + matC(i,j) + matD(i,j);
   auto stop = std::chrono::steady_clock::now();
   auto diff = stop - start;
   std::cout << std::chrono::duration<double, std::milli>(diff).count() << "ms" << std::endl; 

   print(matA, "A.data");
   print(matB, "B.data");
   print(matC, "C.data");
   print(matD, "D.data");

   return 0;
}
