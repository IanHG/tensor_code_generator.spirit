#include <iostream>
#include <chrono>
#include <limits>
#include <fstream>
#include "../../../../../libmda/arrays/SDArray2D.h"
#include "../../../../../libmda/arrays/MultiDimArray.h"
#include "../../../../../libmda/util/rand_seed.h"

//using matrix_type = midas::mmv::SDArray2D<double>;
using vector_type = libmda::MDA<double, 1>;
using matrix_type = libmda::MDA<double, 2>;
using tensor_type = libmda::MDA<double, 3>;
using quad_type = libmda::MDA<double, 4>;

template<class T>
void fill_random(T& mat)
{
   for(int i = 0; i < mat.size(); ++i)
   {
      mat.vec_at(i) = libmda::util::rand_signed_float<double>();
   }
}


void fill_from_file(tensor_type& tens, const std::string& str)
{
   std::ifstream ifs(str);
   for(int k = 0 ; k < tens.extent<2>(); ++k)
      for(int j = 0 ; j < tens.extent<1>(); ++j)
         for(int i = 0 ; i < tens.extent<0>(); ++i)
      {
         ifs >> tens.at(i,j,k);
      }
}

void fill_from_file(matrix_type& tens, const std::string& str)
{
   std::ifstream ifs(str);
   for(int j = 0 ; j < tens.extent<1>(); ++j)
      for(int i = 0 ; i < tens.extent<0>(); ++i)
   {
      ifs >> tens.at(i,j);
   }
}

void print(const quad_type& tens, const std::string& str)
{
   std::ofstream of(str);
   of << std::showpos;
   using limit_type = std::numeric_limits<typename tensor_type::value_type>;
   of << std::scientific;
   of << std::setprecision(limit_type::max_digits10);
   for(int l = 0 ; l < tens.extent<3>(); ++l)
      for(int k = 0 ; k < tens.extent<2>(); ++k)
         for(int j = 0 ; j < tens.extent<1>(); ++j)
            for(int i = 0 ; i < tens.extent<0>(); ++i)
         {
            of << tens.at(i,j,k,l) << std::endl;
         }
}

void print(const tensor_type& tens, const std::string& str)
{
   std::ofstream of(str);
   of << std::showpos;
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
   of << std::showpos;
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
   of << std::showpos;
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
   
   matrix_type matA(ii,jj);
   matrix_type matB(jj,ii);
   matrix_type matC(ii,jj);
   matrix_type matD(jj,ii);
   matrix_type matE(jj,ii);

   fill_random(matB);
   fill_random(matC);
   fill_random(matD);
   fill_random(matE);
   //fill_from_file(matB, "B.mat");
   //fill_from_file(matC, "C.mat");
   
   libmda::cindex<'i'> i;
   libmda::cindex<'j'> j;
   libmda::cindex<'k'> k;
   libmda::cindex<'l'> l;
   libmda::cindex<'m'> m;
   libmda::cindex<'n'> n;
   
   auto start = std::chrono::steady_clock::now();
   matA(i,j) = matB(j,i) - matC(i,j) + matD(j,i) - matE(j,i);
   auto stop = std::chrono::steady_clock::now();
   auto diff = stop - start;
   std::cout << std::chrono::duration<double, std::milli>(diff).count() << "ms" << std::endl; 

   print(matA, "A.data");
   print(matB, "B.data");
   print(matC, "C.data");
   print(matD, "D.data");
   print(matE, "E.data");

   return 0;
}
