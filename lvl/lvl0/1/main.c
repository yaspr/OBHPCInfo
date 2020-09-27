/*
 Compilation: gcc main.c -o main
 
 */
#include <stdio.h>

//Définition itérative
double fibo1(int n)
{
  double phi = 0.0;
  int f0 = 0, f1 = 1, fn;
  
  //
  n--;
  
  //
  for (int i = 0; i < n; i++)
    {
      fn = f0 + f1;

      printf("%30d", fn);

      f0 = f1;
      f1 = fn;

      phi = (double)f1 / (double)f0;

      printf("\t\t%10d / %10d = %20.10lf\n", f1, f0, phi); 
    }

  return phi;
}

//Définition récursive
int fibor(int n)
{
  if (n > 2)
    return fibor(n - 1) + fibor(n - 2);
  else
    return 1;
}

//
double fibo2(int n)
{
  int f0 = fibor(n - 1);
  int f1 = fibor(n);
  double phi = (double)f1 / (double)f0;

  printf("\n%30d\t\t%10d / %10d = %20.10lf\n", f1, f1, f0, phi); 

  return phi;
}
  
//
int main()
{
  int n;
  
  printf("Entrer un nombre: ");
  scanf("%d", &n);

  if (n <= 2)
    {
      printf("Erreur: n doit être supérieur à 2\n");

      return 0;
    }
  /* else */
  /*   printf("%30d\n%30d\n", 0, 1); */
  
  //
  printf("Fibonacci itérative\n");
  fibo1(n);

  //
  printf("Fibonacci récursive\n");
  fibo2(n);
  
  printf("\n");
  
  return 0;
}
