#include <iostream>
#include <fstream>

#include "../../../libmda/numeric/float_eq.h"

void fill_from_file(double* pd, int size, const std::string& filename)
{
   std::ifstream ifs(filename);
   for(int i = 0; i < size; ++i)
   {
      ifs >> pd[i];
   }
}

void print(double* pd, int size)
{
   for(int i = 0; i < size; ++i)
   {
      std::cout << pd[i] << std::endl;
   }
}

bool check_result(double* d, double* e, int size)
{
   for(int i = 0; i < size; ++i)
   {
      if(!libmda::numeric::float_eq(d[i], e[i], 2))
      {
         return false;
      }
   }
   return true;
}
