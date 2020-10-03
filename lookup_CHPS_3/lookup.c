//Header files 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

//
#define ALIGN 64

//
#define POS_TAB_SIZE 1000

//Positions table
typedef struct pos_tab_s {

  //Number of array chunk elements
  unsigned long long count;

  //Positions
  unsigned long long *positions;

  //Next table chunk
  struct pos_tab_s *next;
  
} pos_tab_t;

//Allocate memory for the positions table 
pos_tab_t *create_pos_tab()
{
  //Allocate a positions table
  pos_tab_t *pos_tab = malloc(sizeof(pos_tab_t));

  //Check pointer
  if (!pos_tab)
    return NULL;
  
  //
  pos_tab->count = 0;

  //Allocate positions array 
  pos_tab->positions = aligned_alloc(ALIGN, POS_TAB_SIZE);

  //Check pointer
  if (!pos_tab->positions)
    return NULL;
  
  //
  pos_tab->next = NULL;
  
  //
  return pos_tab;
}

//Free up linked positions tables 
void release_pos_tab(pos_tab_t *pos_tab)
{
  if (!pos_tab)
    printf("Error: NULL pointer!\n");
  else
    {
      pos_tab_t *curr = pos_tab;
      pos_tab_t *prev = pos_tab;
      
      //
      while (curr)
	{
	  //Move to the next positions table
	  curr = curr->next;

	  //Zero up the counter field
	  prev->count = 0;

	  //Zero up the next pointer
	  prev->next = NULL;

	  //Zero up the allocated memory
	  memset(prev->positions, 0, POS_TAB_SIZE);

	  //Free the allocated positions pointer
	  free(prev->positions);

	  //Free the positions table
	  free(prev);

	  //Move the previous pointer to current positions table
	  prev = curr;
	}
    }
}

//Create a new positions table and link it to the given one
pos_tab_t *extend_pos_tab(pos_tab_t *pos_tab)
{
  if (!pos_tab)
    return NULL;

  //
  pos_tab_t *p = pos_tab;

  //Allocate new positions table
  pos_tab_t *n = malloc(sizeof(pos_tab_t));

  //Check pointer
  if (!n)
    return NULL;

  //
  n->count = 0;

  //
  n->positions = aligned_alloc(ALIGN, POS_TAB_SIZE);
  
  //Check pointer
  if (!p->positions)
    return NULL;
  
  //Walk to the last entry of the list
  while (p->next)
    p = p->next;

  //Set n as the last entry of the given list
  p->next = n;

  //Return n
  return n;
}

//Return a boolean value (inserted or not inserted)
unsigned char insert_pos_tab_entry(pos_tab_t *pos_tab, unsigned long long pos)
{
  //
  if (!pos_tab)
    return 0;
  
  //
  pos_tab_t *p = pos_tab;
  unsigned char inserted = 0;
  
  //
  while (!inserted && p)
    {
      //Check if empty spot 
      if (p->count < POS_TAB_SIZE)
	{
	  //Insert position in table
	  p->positions[p->count++] = pos;
	  
	  //
	  inserted = 1;
	}
      else
	p = p->next;
    }
  
  //If not inserted
  if (!inserted)
    {
      //
      pos_tab_t *n = extend_pos_tab(pos_tab);
      
      n->positions[n->count++] = pos;
      
      inserted = 1;
    }
  
  //
  return inserted;
}

//Loads a given file text
char *load_file_text(const char *fname)
{
  //Check file name pointer
  if (!fname)
    return printf("Error: NULL pointer!\n"), NULL;
  
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
  if (t)
    free(t);
}

//Lookup string within a given text
//Return position + 1 when string is found 
pos_tab_t *lookup_file_text(const char *s, const char *t)
{
  //
  if (!s || !t)
    return printf("Error: pointers cannot be NULL\n"), NULL;

  //
  unsigned char        found = 0;                //Boolean variable
  unsigned long long   count = 0;
  unsigned long long     pos = 0;
  unsigned long long   s_len = strlen(s);
  unsigned long long   t_len = strlen(t);

  //Create first positions table
  pos_tab_t *pos_tab = create_pos_tab();
  
  //
  for (pos = 0; pos < t_len - s_len; pos++)
    {
      //
      found = !strncmp(t + pos, s, s_len);

      //
      if (found)
	insert_pos_tab_entry(pos_tab, pos); 

      //
      count += found;
    }
  
  //
  return pos_tab;
}

//
void print_pos_tab(pos_tab_t *pos_tab)
{
  //
  pos_tab_t *p = pos_tab;

  //
  while (p)
    {
      for (unsigned long long i = 0; i < p->count; i++)
	printf("%llu\n", p->positions[i]);
      
      p = p->next;
    }
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
      pos_tab_t *pos_tab = lookup_file_text(argv[1], t);

      if (pos_tab)
	{
	  print_pos_tab(pos_tab);
	  release_pos_tab(pos_tab);
	}
      else
	printf("BIG OUPS!\n");

      //
      release_file_text(t);
    }
  
  //  
  return 0;
}
