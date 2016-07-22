#include "axpy.hpp"

extern "C"
{
   void saxpy_(int*, float*, float*, int*, float*, int*);

   void daxpy_(int*, double*, double*, int*, double*, int*);

   void caxpy_(int*, std::complex<float>*, std::complex<float>*, int*, std::complex<float>*, int*);

   void zaxpy_(int*, std::complex<double>*, std::complex<double>*, int*, std::complex<double>*, int*);
}

/*!
 * Single precision axpy
 */
void saxpy
 ( int* n
 , float* da
 , float* dx
 , int* incx
 , float* dy
 , int* incy
 )
{
   saxpy_(n, da, dx, incx, dy, incy);
}

/*!
 * Double precision axpy
 */
void daxpy
 ( int* n
 , double* da
 , double* dx
 , int* incx
 , double* dy
 , int* incy
 )
{
   daxpy_(n, da, dx, incx, dy, incy);
}

/*!
 * Complex single precision axpy
 */
void caxpy
 ( int* n
 , std::complex<float>* da
 , std::complex<float>* dx
 , int* incx
 , std::complex<float>* dy
 , int* incy
 )
{
   caxpy_(n, da, dx, incx, dy, incy);
}

/*!
 * Complex double precision axpy
 */
void zaxpy
 ( int* n
 , std::complex<double>* da
 , std::complex<double>* dx
 , int* incx
 , std::complex<double>* dy
 , int* incy
 )
{
   zaxpy_(n, da, dx, incx, dy, incy);
}
