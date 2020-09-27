/* 

   Soundtrack: 
   
   Freak Kitchen - Morons
   Freak Kitchen - Freak of the week
   Freak Kitchen - Så Kan Det Gå När Inte Haspen Är På
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>

//
#include "rdtsc.h"

//
#include "protos.h"

//
#include "common.h"

//
#define ALIGN 64

//
#define CPU_FREQ _CPU_FREQ_

//
void sort(double *restrict a, u64 n)
{
  for (u64 i = 0; i < n; i++)
    for (u64 j = i + 1; j < n; j++)
      if (a[i] > a[j])
	{
	  double tmp = a[i];

	  a[i] = a[j];
	  a[j] = tmp;
	}
}

//
void init(double *restrict a, u64 n, double c)
{
#pragma omp parallel for
  for (u64 i = 0; i < n; i++)
    a[i] = c;
}

//
double measure_mm(const char *title,
		  void mm_fct(double *restrict, double *restrict, double *restrict, u64 n), const u64 n, const u64 ns, const u64 nr)
{
  //Size in MiB
  u64 s = sizeof(double) * n * n;

  //
  double after, before;
  double min, max, avg, med, bpc, gbps;
  double *restrict cycles = _mm_malloc(sizeof(double) * ns, ALIGN);
  
  //
  double *a = _mm_malloc(s, ALIGN);
  double *b = _mm_malloc(s, ALIGN);
  double *c = _mm_malloc(s, ALIGN);
  
  //Initialize arrays and warm up the caches
  init(a, n, 1.0);
  init(b, n, 1.0);
  init(c, n, 0.0);
  
  //
  for (u64 i = 0; i < ns; i++)
    {
      //Start counter
      before = rdtsc();

      //NB_RUNS should be set dynamically depending on the value of n
      for (u64 j = 0; j < nr; j++) 
	mm_fct(a, b, c, n);
      
      //Stop counter
      after = rdtsc();
      
      //Cycles per run
      cycles[i] = (after - before) / nr;

      //
      /* fprintf(stderr, "Sample %llu: %15.3lf\n", i, cycles[i]); */
    }
  
  //
  sort(cycles, ns);

  //
  min = cycles[0];
  max = cycles[ns - 1];
  avg = (min + max) / 2.0;
  med = (cycles[ns >> 1] + cycles[(ns >> 1) + 1]) / 2.0;
  
  //
  s *= 3;

  //size(A) + size(B) + size(C) = TOTAL MEMORY 
  bpc = s / med;
  
  gbps = s / (med / CPU_FREQ);
  
  //title; size(B, KiB, MiB, GiB); min; max; avg; med; bpc; gbps;
  printf("%30s;\t%20llu; %20llu; %20llu; %20llu;\t%15.3lf;\t%15.3lf;\t%15.3lf;\t%15.3lf;\t%15.3lf;\t%15.2lf GiB/s;\n",
	 title,
	 s, s >> 10, s >> 20, s >> 30,
	 min,
	 max,
	 avg,
	 med,
	 bpc,
	 gbps);
  
  //
  _mm_free(cycles);
  
  //
  _mm_free(a);
  _mm_free(b);
  _mm_free(c);

  return med;
}

//
int main(int argc, char **argv)
{
  if (argc < 4)
    return printf("Usage: %s [nb of rows or columns] [number of samples] [number of runs]\n", argv[0]), -1;

  //
  u64 n  = atoll(argv[1]);
  u64 ns = atoll(argv[2]);
  u64 nr = atoll(argv[3]);
  
  /* // */
  /* u64 s = sizeof(double) * n * n * 3; */

  /* fprintf(stderr, "Memory size: %20lluB, %20lluKiB, %20lluMiB, %20lluGiB,\n", s, s >> 10, s >> 20, s >> 30);   */
  
  //
  double vers[10];
  
  //
  vers[0] = measure_mm("ijk"               , mm1, n, ns, nr);
  vers[1] = measure_mm("ikj"               , mm2, n, ns, nr);
  vers[2] = measure_mm("cblas_mkl"         , mm3, n, ns, nr);
  vers[3] = measure_mm("ikj OpenMP"        , mm4, n, ns, nr);
  vers[4] = measure_mm("ikj OpenMP + vec + Unroll", mm5, n, ns, nr);
  
  fprintf(stderr, "Speed up:\n");
  fprintf(stderr, "\tijk / ikj\t\t\t\t: %.2lf\n", vers[0] / vers[1]);
  fprintf(stderr, "\tijk / cblas_mkl\t\t\t\t: %.2lf\n", vers[0] / vers[2]);
  fprintf(stderr, "\tijk / ikj OpenMP\t\t\t: %.2lf\n", vers[0] / vers[3]);
  fprintf(stderr, "\tijk / ikj OpenMP + vec + Unroll\t: %.2lf\n", vers[0] / vers[4]);
  
  //
  return 0;
}
