#include "common.h"

//
void mm1(double *restrict a, double *restrict b, double *restrict c, u64 n)
{
  for (u64 i = 0; i < n; i++)
    for (u64 j = 0; j < n; j++)
      for (u64 k = 0; k < n; k++)
	c[i * n + j] += a[i * n + k] * b[k * n + j];
}
