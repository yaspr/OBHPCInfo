/*
  Compilation: gcc main.c -o main -lm
  
 */
#include <math.h>
#include <stdio.h>

//
int is_prime(int n)
{
  int div = 0;
  
  for (int i = 2; !div && i <= sqrt(n); i++)
    div = !(n % i);
  
  return !div;
}

//
int main()
{
  int n;
  
  //
  printf("Entrer un nombre: ");
  scanf("%d", &n);

  //
  printf("%d %s premier\n", n, (is_prime(n)) ? "est" : "n'est pas");
  
  return 0;
}
