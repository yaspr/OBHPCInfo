//Header files 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

//
#define ALIGN 64

//Loads a given file text
char *load_file_text(const char *fname)
{
  //Check file name pointer
  if (!fname)
    return printf("Error: NULL pointer!"), NULL;

  //Get file meta information
  struct stat sb;

  //Check stat return value
  if (stat(fname, &sb) < 0)
    return printf("Error: cannot stat '%s\n'", fname), NULL;
  
  //Open the given file
  FILE *fp = fopen(fname, "r");

  //Check file pointer
  if (!fp)
    return printf("Error: cannot open file '%s'\n", fname), NULL;

  //Make sure the buffer ghas space for terminator
  char *t = aligned_alloc(ALIGN, sb.st_size + 1);

  //Check allocated memory pointer
  if (!t)
    return printf("Error: cannot allocate memory\n"), NULL;

  //Load the file text into the allocated memory pointer
  unsigned long long n = fread(t, sizeof(char), sb.st_size, fp);

  //Close the file (not needed anymore)
  fclose(fp);
  
  if (n != sb.st_size)
    return printf("Error: read size does not match expected size\n"), NULL;

  //Make sure the text memory is zero terminated
  t[sb.st_size] = 0;
  
  //
  return t;
}

//
static inline void release_file_text(char *t)
{
  //Check pointer
  if (t)
    free(t);
}

//
void green_print(const char *s)
{
  //
  if (!s)
    printf("Error: pointer cannot be NULL\n");
  
  //
  printf("\033[1;32m");
  printf("%s", s);
  printf("\033[0m");
}

//Lookup string within a given text
//Return position + 1 when string is found 
unsigned long long lookup_file_text(const char *s, const char *t)
{
  //CHeck pointers
  if (!s || !t)
    return printf("Error: pointers cannot be NULL\n"), 0;

  //
  unsigned char        found = 0;                //Boolean variable
  unsigned long long     pos = 0;
  unsigned long long   count = 0;
  unsigned long long   s_len = strlen(s);
  unsigned long long   t_len = strlen(t);
  
  //Go over the full text
  for (pos = 0; pos < t_len - s_len; pos++)
    {
      //
      found = !strncmp(t + pos, s, s_len);

      //Store position
      if (found)
	{
	  green_print(s);
	  printf(" found in position: %llu\n", pos);
	}
      
      //
      count += found;
    }
  
  //
  return count;
}

//Pass a string and a file name
int main(int argc, char **argv)
{
  //Check number of parameters
  if (argc < 3)
    return printf("Usage: %s [string] [file name]\n", argv[0]), 1;

  //
  char *t = load_file_text(argv[2]);

  //
  if (t)
    {
      //
      unsigned long long count = lookup_file_text(argv[1], t);
      
      //
      if (!count)
	printf("Message: cannot find '%s' in file '%s'\n", argv[1], argv[2]);
      else
	printf("Message: '%s' was found %llu time(s)\n", argv[1], count);
      
      release_file_text(t);
    }
  
  //  
  return 0;
}
