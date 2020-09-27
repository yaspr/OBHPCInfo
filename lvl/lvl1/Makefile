GCC = gcc
ICC = icc

CFLAGS = -std=c99 -g3 -Wall

GLFLAGS = -lcblas
ILFLAGS = -mkl

#
GOFLAGS = -march=native -Ofast -ffast-math -fopenmp -funroll-loops -finline-functions -ftree-vectorize -ftree-slp-vectorize

#-qopt-report will generate a per file report with details on how and what code was optimized  
IOFLAGS = -march=native -Ofast -ffast-math -qopenmp -qopt-report

MM_FILES = mm1.c mm2.c mm3.c mm4.c mm5.c

all: blas mkl

blas:
	$(GCC) -D_CPU_FREQ_=3.8 $(CFLAGS) $(GOFLAGS) main.c $(MM_FILES) -o gmm_bench_$@ $(GLFLAGS)

mkl:
	$(ICC) -D_CPU_FREQ_=3.8 $(CFLAGS) $(IOFLAGS) main.c $(MM_FILES) -o gmm_bench_$@ $(ILFLAGS)

clean:
	rm -Rf *~ gmm_bench_*
