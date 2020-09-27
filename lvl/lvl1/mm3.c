#include <cblas.h>

#include "common.h"

//
void mm3(double *restrict a, double *restrict b, double *restrict c, u64 n)
{
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, a, n, b, n, 0.0, c, n);
}
