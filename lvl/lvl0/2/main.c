/*
  Compilation: gcc main.c -o main

 */
#include <stdio.h>

//
void collatz1(int n)
{
  while (n != 1)
    {
      printf("%d\n", n);
      
      if ((n % 2) == 0)
	n /= 2;
      else
	n = (3 * n + 1);
    }
}

//
void collatz2(int n)
{
  while (n != 1)
    {
      printf("%d\n", n);
      
      if ((n & 1))
	n = (n << 1) + n + 1;
      else
	n >>= 1;
    }
}

//
int main()
{
  int n;
  
  printf("Entrer un nombre: ");
  scanf("%d", &n);

  printf("Collatz1\n"); 
  collatz1(n);

  printf("\nCollatz2\n"); 
  collatz2(n);
  
  return 0;
}
