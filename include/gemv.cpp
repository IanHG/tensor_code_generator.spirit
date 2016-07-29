#include "tcg/gemv.hpp"

extern "C"
{
   void sgemv_(char*, int*, int*, float*, float*, int*, float*, int*, float*, float*, int*);

   void dgemv_(char*, int*, int*, double*, double*, int*, double*, int*, double*, double*, int*);

   void cgemv_(char*, int*, int*, std::complex<float>*, std::complex<float>*, int*, std::complex<float>*, int*, std::complex<float>*, std::complex<float>*, int*);

   void zgemv_(char*, int*, int*, std::complex<double>*, std::complex<double>*, int*, std::complex<double>*, int*, std::complex<double>*, std::complex<double>*, int*);
}

/*!
 *
 */
void sgemv
 ( char* trans
 , int* m
 , int* n
 , float* alpha
 , float* a
 , int* lda
 , float* x
 , int* incx
 , float* beta
 , float* y
 , int* incy
 )
{
   sgemv_(trans, m, n, alpha, a, lda, x, incx, beta, y, incy);
}

/*!
 *
 */
void dgemv
 ( char* trans
 , int* m
 , int* n
 , double* alpha
 , double* a
 , int* lda
 , double* x
 , int* incx
 , double* beta
 , double* y
 , int* incy
 )
{
   dgemv_(trans, m, n, alpha, a, lda, x, incx, beta, y, incy);
}

/*!
 *
 */
void cgemv
 ( char* trans
 , int* m
 , int* n
 , std::complex<float>* alpha
 , std::complex<float>* a
 , int* lda
 , std::complex<float>* x
 , int* incx
 , std::complex<float>* beta
 , std::complex<float>* y
 , int* incy
 )
{
   cgemv_(trans, m, n, alpha, a, lda, x, incx, beta, y, incy);
}

/*!
 *
 */
void zgemv
 ( char* trans
 , int* m
 , int* n
 , std::complex<double>* alpha
 , std::complex<double>* a
 , int* lda
 , std::complex<double>* x
 , int* incx
 , std::complex<double>* beta
 , std::complex<double>* y
 , int* incy
 )
{
   zgemv_(trans, m, n, alpha, a, lda, x, incx, beta, y, incy);
}
