#ifndef TCG_INCLUDE_AXPY_HPP_INCLUDED
#define TCG_INCLUDE_AXPY_HPP_INCLUDED

#include <complex>

//! single precision
void saxpy(int*, float*, float*, int*, float*, int*);

//! double precision
void daxpy(int*, double*, double*, int*, double*, int*);

//! complex single precsion
void caxpy(int*, std::complex<float>*, std::complex<float>*, int*, std::complex<float>*, int*);

//! complex double precision
void zaxpy(int*, std::complex<double>*, std::complex<double>*, int*, std::complex<double>*, int*);

#endif /* TCG_INCLUDE_AXPY_HPP_INCLUDED */
