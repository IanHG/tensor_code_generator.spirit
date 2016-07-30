#include <iostream>
#include <chrono>
#include <limits>
#include <fstream>
#include "../../../../libmda/arrays/SDArray2D.h"
#include "../../../../libmda/arrays/MultiDimArray.h"
#include "../../../../libmda/util/rand_seed.h"

//using matrix_type = midas::mmv::SDArray2D<double>;
using vector_type = libmda::MDA<double, 1>;
using matrix_type = libmda::MDA<double, 2>;
using tensor_type = libmda::MDA<double, 3>;

template<class T>
void fill_random(T& mat)
{
   for(int i = 0; i < mat.size(); ++i)
   {
      mat.vec_at(i) = libmda::util::rand_signed_float<double>();
   }
}

void print(const tensor_type& tens, const std::string& str)
{
   std::ofstream of(str);
   using limit_type = std::numeric_limits<typename tensor_type::value_type>;
   of << std::scientific;
   of << std::setprecision(limit_type::max_digits10);
   for(int k = 0 ; k < tens.extent<2>(); ++k)
      for(int j = 0 ; j < tens.extent<1>(); ++j)
         for(int i = 0 ; i < tens.extent<0>(); ++i)
      {
         of << tens.at(i,j,k) << std::endl;
      }
}

void print(const matrix_type& mat, const std::string& str)
{
   std::ofstream of(str);
   using limit_type = std::numeric_limits<typename matrix_type::value_type>;
   of << std::scientific;
   of << std::setprecision(limit_type::max_digits10);
   for(int j = 0 ; j < mat.extent<1>(); ++j)
      for(int i = 0 ; i < mat.extent<0>(); ++i)
   {
      of << mat.at(i,j) << std::endl;
   }
}

void print(const vector_type& vec, const std::string& str)
{
   std::ofstream of(str);
   using limit_type = std::numeric_limits<typename vector_type::value_type>;
   of << std::scientific;
   of << std::setprecision(limit_type::max_digits10);
   for(int i = 0 ; i < vec.extent<0>(); ++i)
   {
      of << vec.at(i) << std::endl;
   }
}

int main()
{
   int ii = 4;
   int jj = 3;
   int kk = 2;

   matrix_type matA(ii,jj);
   tensor_type matB(ii,jj,kk);
   vector_type matC(kk);

   fill_random(matB);
   fill_random(matC);
   
   libmda::cindex<'i'> i;
   libmda::cindex<'j'> j;
   libmda::cindex<'k'> k;
   
   auto start = std::chrono::steady_clock::now();
   matA(i,j) = matB(i,j,k) * matC(k);
   auto stop = std::chrono::steady_clock::now();
   auto diff = stop - start;
   std::cout << std::chrono::duration<double, std::milli>(diff).count() << "ms" << std::endl; 

   print(matA, "A.mat");
   print(matB, "B.mat");
   print(matC, "C.mat");

   return 0;
}
