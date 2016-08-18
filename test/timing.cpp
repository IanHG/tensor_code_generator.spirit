#include <memory>
#include <iostream>
#include <chrono>

#include "../include/axpy.hpp"
#include "../include/gemv.hpp"
#include "../include/gemm.hpp"

using float_type = double;

void time_dgemm
   ( int m, int n, int k
   , char transa
   , char transb
   )
{
   float_type alpha = 2.0;
   float_type beta = 0.0;
   int lda = transa == 'N' ? std::max(1, m) : std::max(1, k);
   int ldb = transb == 'N' ? std::max(1, k) : std::max(1, n);
   int ldc = std::max(1, m);
   
   auto asize = m*k;
   std::unique_ptr<float_type[]> a(new float_type[asize]);
   for(int i = 0; i < asize; ++i)
      a[i] = i + 1;
   
   auto bsize = k*n;
   std::unique_ptr<float_type[]> b(new float_type[bsize]);
   for(int i = 0; i < bsize; ++i)
      b[i] = i + 1;
   
   auto csize = m*n;
   std::unique_ptr<float_type[]> c(new float_type[csize]);
   
   double time = 0;
   int ntimes = 100;
   for(int i = 0; i < ntimes; ++i)
   {
      // time the dgemm
      auto start = std::chrono::steady_clock::now();
      dgemm(&transa, &transb, &m, &n, &k, &alpha, a.get(), &lda, b.get(), &ldb, &beta, c.get(), &ldc);
      auto stop = std::chrono::steady_clock::now();

      auto diff = stop - start;
      time += std::chrono::duration<double, std::milli>(diff).count(); 
   }
   
   time /= ntimes;
   std::cout << "A:" << transa << " "
             << "B:" << transb << " "
             << time << "ms"
             << std::endl;
}
               

int main()
{
   int m = 1000;
   int n = 1000;
   int k = 1000;

   time_dgemm(m, n, k, 'N', 'N');
   time_dgemm(m, n, k, 'T', 'N');
   time_dgemm(m, n, k, 'N', 'T');
   time_dgemm(m, n, k, 'T', 'T');

   return 0;
}
