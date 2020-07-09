#include <stdio.h>
#include <stdlib.h>

int frobcmp(char const * a, char const * b)
{
  if (*a == ' ' && *b == ' ')
    return 0;
  if (*a == ' ')
    return -1;
  if (*b == ' ')
    return 1;
  while (*a != ' ' || *b != ' ')
    {
      char tempA = *a ^ 42;
      char tempB = *b ^ 42;
      if (tempA < tempB)
	return -1;
      if(*a == ' ' && *b != ' ')
	return -1;
      if (tempA > tempB)
	return 1;
      if (*a != ' ' && *b == ' ')
	return 1;
      a++;
      b++;
    }
  //if they exit the loop, they are the same string
  return 0;
}

int wrapper(const void *A, const void* B)
{
  char* a = *((char**)A);
  char* b = *((char**)B);
  return frobcmp(a,b);
}

int main() {
  int c = 'a';
  char* cPtr = (char*) malloc(sizeof(char));
  char** wordsPtr = (char**) malloc(sizeof(char*));
  int cIndex = 0;
  int wordsIndex = 0;
  int i;
  int j;
  while(c != EOF)
    {
      c = getchar();
      if (ferror(stdin) != 0)
        {
	  fprintf(stderr, "Stdin error");
	  exit(1);
        }
      if (c == ' ' || feof(stdin))
        {
	  if (cIndex == 0 && !(feof(stdin)))
            {
	      continue;
            }
	  char* tempC = (char *)realloc(cPtr, (cIndex + 1) * sizeof(char));
	  if (cPtr == NULL)
            {
	      for (i = 0; i <= wordsIndex; i++) {
		free(wordsPtr[i]);
	      }
	      free(wordsPtr);
	      fprintf(stderr, "Memory allocation error");
	      exit(1);
            }
	  cPtr = tempC;
	  if (feof(stdin))
            {
	      if (cIndex != 0)
                {
		  cPtr[cIndex] = ' ';
		  char** tempWords = (char **)realloc(wordsPtr, (wordsIndex + 1) * sizeof(char *));
		  if (wordsPtr == NULL)
                    {
		      for (i = 0; i <= wordsIndex; i++) {
			free(wordsPtr[i]);
		      }
		      free(wordsPtr);
		      fprintf(stderr, "Memory allocation error");
		      exit(1);
                    }
		  wordsPtr = tempWords;
		  wordsPtr[wordsIndex] = cPtr;
                }
	      else
                {
		  wordsIndex--;
		  free(cPtr);
                }
            }
	  else
            {
	      cPtr[cIndex] = c;
	      char** tempWords = (char **)realloc(wordsPtr, (wordsIndex + 1) * sizeof(char *));
	      if (wordsPtr == NULL)
                {
		  for (i = 0; i <= wordsIndex; i++) {
		    free(wordsPtr[i]);
		  }
		  free(wordsPtr);
		  fprintf(stderr, "Memory allocation error");
		  exit(1);
                }
	      wordsPtr = tempWords;
	      wordsPtr[wordsIndex] = cPtr;
	      wordsIndex++;
	      cIndex = 0;
	      cPtr = (char*) malloc(sizeof(char));
            }
        }
      else {
	char* tempC = (char *)realloc(cPtr, (cIndex + 1) * sizeof(char));
	if (cPtr == NULL)
	  {
	    for (i = 0; i <= wordsIndex; i++) {
	      free(wordsPtr[i]);
	    }
	    free(wordsPtr);
	    fprintf(stderr, "Memory allocation error");
	    exit(1);
	  }
	cPtr = tempC;
	cPtr[cIndex] = c;
	cIndex++;
	//wordsPtr[wordsIndex] = cPtr;
      }
    }
    
  qsort(wordsPtr, wordsIndex + 1, sizeof(char *), wrapper);
    
  //print 2D array to stdout
  for (i = 0; i <= wordsIndex; i++) {
    for (j = 0; ;j++) {
      putchar(wordsPtr[i][j]);
      if (ferror(stdout) != 0)
	{
	  fprintf(stderr, "Stdout error");
	  exit(1);
	}
      if (wordsPtr[i][j] == ' ')
	break;
    }
  }
    
  for (i = 0; i <= wordsIndex; i++) {
    free(wordsPtr[i]);
  }
  free(wordsPtr);
  return 0;
}
