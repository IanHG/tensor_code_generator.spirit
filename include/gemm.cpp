#include "gemm.hpp"

extern "C"
{
   void sgemm_( char*, char*, int*, int*, int*, float*
              , float*, int*
              , float*, int*
              , float*, float*, int*);

   void dgemm_( char*, char*, int*, int*, int*, double*
              , double*, int*
              , double*, int*
              , double*, double*, int*);
   
   void cgemm_( char*, char*, int*, int*, int*, std::complex<float>*
              , std::complex<float>*, int*
              , std::complex<float>*, int*
              , std::complex<float>*, std::complex<float>*, int*);
   
   void zgemm_( char*, char*, int*, int*, int*, std::complex<double>*
              , std::complex<double>*, int*
              , std::complex<double>*, int*
              , std::complex<double>*, std::complex<double>*, int*);
}

/*!
 *
 */
void sgemm
   ( char* transa
   , char* transb
   , int* m
   , int* n
   , int* k
   , float* alpha
   , float* a
   , int* lda
   , float* b
   , int* ldb
   , float* beta
   , float* c
   , int* ldc
   )
{
   sgemm_(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}

/*!
 *
 */
void dgemm
   ( char* transa
   , char* transb
   , int* m
   , int* n
   , int* k
   , double* alpha
   , double* a
   , int* lda
   , double* b
   , int* ldb
   , double* beta
   , double* c
   , int* ldc
   )
{
   dgemm_(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}

/*!
 *
 */
void cgemm
   ( char* transa
   , char* transb
   , int* m
   , int* n
   , int* k
   , std::complex<float>* alpha
   , std::complex<float>* a
   , int* lda
   , std::complex<float>* b
   , int* ldb
   , std::complex<float>* beta
   , std::complex<float>* c
   , int* ldc
   )
{
   cgemm_(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}

/*!
 *
 */
void zgemm
   ( char* transa
   , char* transb
   , int* m
   , int* n
   , int* k
   , std::complex<double>* alpha
   , std::complex<double>* a
   , int* lda
   , std::complex<double>* b
   , int* ldb
   , std::complex<double>* beta
   , std::complex<double>* c
   , int* ldc
   )
{
   zgemm_(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}
