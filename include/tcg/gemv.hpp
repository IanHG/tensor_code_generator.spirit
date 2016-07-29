#ifndef TCG_INCLUDE_GEMV_HPP_INCLUDED
#define TCG_INCLUDE_GEMV_HPP_INCLUDED

#include <complex>

/*!
 * GEMV performs one of the matrix-vector operations
 *
 * y := alpha*A*x + beta*y,   or   y := alpha*A**T*x + beta*y,
 *
 * where alpha and beta are scalars, x and y are vectors and A is an
 * m by n matrix.
 */
//! single precision gemv
void sgemv(char*, int*, int*, float*, float*, int*, float*, int*, float*, float*, int*);

//! double precision gemv
void dgemv(char*, int*, int*, double*, double*, int*, double*, int*, double*, double*, int*);

//! complex single precision gemv
void cgemv( char*, int*, int*, std::complex<float>*
          , std::complex<float>*, int*
          , std::complex<float>*, int*
          , std::complex<float>*, std::complex<float>*, int*
          );

//! complex double precision gemv
void zgemv( char*, int*, int*, std::complex<double>*
          , std::complex<double>*, int*
          , std::complex<double>*, int*
          , std::complex<double>*, std::complex<double>*, int*
          );

#endif /* TCG_INCLUDE_GEMV_HPP_INCLUDED */
