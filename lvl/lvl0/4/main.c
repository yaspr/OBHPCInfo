/*
 */

#include <math.h>
#include <stdio.h>

//
void get_abc(float *a, float *b, float *c)
{
  float _a, _b, _c;
  
  printf("Entrer a: "); scanf("%f", &_a);
  printf("Entrer b: "); scanf("%f", &_b);
  printf("Entrer c: "); scanf("%f", &_c);

  (*a) = _a;
  (*b) = _b;
  (*c) = _c;
}

//
float delta(float a, float b, float c)
{ return (b * b) - (4 * a * c); }

//
void test_quad(float a, float b, float c, float x)
{
  float r = a * (x * x) + b * x + c;

  printf("%f * (%f)^2 + %f * %f + %f = %f\n", a, x, b, x, c, r);
}

//
void solve_quad(float a, float b, float c)
{
  float d = delta(a, b, c);

  if (d == 0)
    {
      float x = -b / (2.0 * a);
      
      printf("## Solution in |R\n");

      printf("x = -%f / (2.0 * %f) = %f\n", fabs(b), a, x);

      test_quad(a, b, c, x); 
    }
  else
    if (d > 0)
      {
	float x1 = (-b + sqrt(d)) / (2.0 * a);
	float x2 = (-b - sqrt(d)) / (2.0 * a);

	printf("## Solutions in |R\n");
	
	printf("x1 = (-%f + sqrt(%f)) / (2.0 * %f) = %f"
	       "\t\t"
	       "x2 = (-%f - sqrt(%f)) / (2.0 * %f) = %f\n",
	       fabs(b), d, a, x1,
	       fabs(b), d, a, x2);

	test_quad(a, b, c, x1); 
	test_quad(a, b, c, x2); 
      }
    else
      {
	float x1 = -b / (2.0 * a);
	float x2 = sqrt(fabs(d)) / (2.0 *a);

	printf("## Solutions in |C\n");
	
	printf("x1 = (-%f + i * sqrt(%f)) / (2.0 * %f)"
	       "\t\t"
	       "x2 = (-%f - i * sqrt(%f)) / (2.0 * %f)\n",
	       fabs(b), fabs(d), a,
	       fabs(b), fabs(d), a);
	       
      }
}

//
int main()
{
  printf("######## Résolution d'équations quadratiques dans |R & |C ########\n");
  printf("\t\t\tax^2 + bx + c = 0 \n");

  float a, b, c;
  
  get_abc(&a, &b, &c);

  solve_quad(a, b, c);
  
  return 0;
}
