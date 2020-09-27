#include "common.h"

//
void mm5(double *restrict a, double *restrict b, double *restrict c, u64 n)
{
#pragma omp parallel for
  for (u64 i = 0; i < n; i++)
    for (u64 k = 0; k < n; k++)
      {
	const double _aik_ = a[i * n + k];

#ifdef __INTEL_COMPILER__
#pragma omp vector
#endif
	for (u64 j = 0; j < (n - (n & 7)); j += 8)
	  {
	    c[i * n + j]     += _aik_ * b[k * n + j]; 
	    c[i * n + j + 1] += _aik_ * b[k * n + j + 1]; 
	    c[i * n + j + 2] += _aik_ * b[k * n + j + 2]; 
	    c[i * n + j + 3] += _aik_ * b[k * n + j + 3]; 
	    c[i * n + j + 4] += _aik_ * b[k * n + j + 4]; 
	    c[i * n + j + 5] += _aik_ * b[k * n + j + 5]; 
	    c[i * n + j + 6] += _aik_ * b[k * n + j + 6]; 
	    c[i * n + j + 7] += _aik_ * b[k * n + j + 7]; 
	  }

	for (u64 j = n - (n & (n & 7)); j < n; j++)
	  c[i * n + j]     += _aik_ * b[k * n + j]; 
      }
}
