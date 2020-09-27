#include "common.h"

//
void mm4(double *restrict a, double *restrict b, double *restrict c, u64 n)
{
#pragma omp parallel for
  for (u64 i = 0; i < n; i++)
    for (u64 k = 0; k < n; k++)
      {
	const double _aik_ = a[i * n + k];
	
	for (u64 j = 0; j < n; j++)
	  c[i * n + j] += _aik_ * b[k * n + j]; 
      }
}
