#ifndef TCG_INCLUDE_GEMM_HPP_INCLUDED
#define TCG_INCLUDE_GEMM_HPP_INCLUDED

#include <complex>

/*!
 * GEMM  performs one of the matrix-matrix operations
 * 
 *    C := alpha*op( A )*op( B ) + beta*C,
 * 
 * where  op( X ) is one of
 * 
 *    op( X ) = X   or   op( X ) = X**T,
 * 
 * alpha and beta are scalars, and A, B and C are matrices, with op( A )
 * an m by k matrix,  op( B )  a  k by n matrix and  C an m by n matrix.
 */
//! single precision gemm
void sgemm( char*, char*, int*, int*, int*, float*
          , float*, int*
          , float*, int*
          , float*, float*, int*);

//! double precision gemm
void dgemm( char*, char*, int*, int*, int*, double*
          , double*, int*
          , double*, int*
          , double*, double*, int*);

//! complex single precision gemm
void cgemm( char*, char*, int*, int*, int*, std::complex<float>*
          , std::complex<float>*, int*
          , std::complex<float>*, int*
          , std::complex<float>*, std::complex<float>*, int*);

//! complex double precision gemm
void zgemm( char*, char*, int*, int*, int*, std::complex<double>*
          , std::complex<double>*, int*
          , std::complex<double>*, int*
          , std::complex<double>*, std::complex<double>*, int*);

#endif /* TCG_INCLUDE_GEMM_HPP_INCLUDED */
