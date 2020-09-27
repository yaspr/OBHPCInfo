#include <stdio.h>
#include <stdlib.h>

#define MAX_NB_VAL 4096

#define newline putchar('\n')

//
int read_file(char *fname, int *t)
{
  int v = 0, n = 0;
  FILE *fd = fopen(fname, "rb");

  //
  if (fd)
    {
      //Lire un entier à la fois 
      while (n < MAX_NB_VAL && (fscanf(fd, "%d", &v) != EOF))
	t[n++] = v;

      if (n >= MAX_NB_VAL)
	printf("Only 4K values are allowed in the file\n");

      fclose(fd);
      
      //Retourne le nombre d'entiers totalement lus  
      return n;
    }
  else
    {
      printf("Erreur: le fichier %s ne peut être trouve\n", fname);

      return 0;
    }
}

//
int dumb_lookup(int *t, int n, int v)
{
  int i;
  int found = 0;
  
  for (i = 0; !found && i < n; i++)
    found = (t[i] == v);

  return (found) ? i : -1;
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 3)
    return printf("Usage: %s [fichier d'entrée] [valeur entière à chercher]\n", argv[0]), 1;

  //Convertir la valeur depuis une chaîne de caractères vers en un entier 
  int v = atoi(argv[2]);

  //Allouer de la mémoire pour y charger le contenu du fichier
  int *t = malloc(sizeof(int) * MAX_NB_VAL);

  //Lire le contenu du fichier en mémoire
  int n = read_file(argv[1], t);

  //Cherche la valeur dans le tableau
  int p = dumb_lookup(t, n, v);

  if (p < 0)
    printf("Valuer non trouvée!\n");
  else
    printf("%d\n", p);
  
  free(t);
  
  return 0;
}
