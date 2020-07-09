#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>

int isUpper;

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
      if (isUpper == 1) {
	if ((int)tempA >= 0 && (int)tempB >= 0)
	  {
	    tempA = toupper((unsigned char) tempA);
	    tempB = toupper((unsigned char) tempB);
	  }
	else
	  {
	    write(2, "Invalid character\n" , 18);
	    exit(1);
	  }
      }
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

int main(int argc, const char *argv[]) {
  switch (argc)
    {
    case 1:
      isUpper = 0;
      break;
    case 2:
      //check for -fg (still works)
      //needs to only have length 2
      if (argv[1][0] != '-' || argv[1][1] != 'f')
	{
	  write(2, "Invalid option\n" , 15);
	  exit(1);
	}
      else
	{
	  isUpper = 1;
	}
      break;
    default:
      write(2, "Invalid option\n" , 15);
      exit(1);
    }
    
  int isFile = 0;
  struct stat fileInfo;
  if (fstat(0, &fileInfo) != -1)
    {
      if (S_ISREG(fileInfo.st_mode) != 0)
	isFile = 1;
    }
    
  int readStatus = -1;
  int writeStatus;
  size_t size;
  int wordCount = 0;
  char* cPtr = NULL;
  char** wordsPtr = (char**) malloc(sizeof(char*));
  int cIndex = 0;
  int wordsIndex = 0;
  int i = 0;
  int j = 0;
  //if size = 0, check stdin
  if (isFile == 1 && fileInfo.st_size != 0)
    {
      cPtr = (char*) malloc(sizeof(char));
      size = fileInfo.st_size;
      char* tempFile = (char*) malloc(size);
      readStatus = read(0, tempFile, size);
      if (readStatus < 0)
        {
	  write(2, "Input error\n", 12);
	  exit(1);
        }
      if (tempFile[size-1] != ' ')
        {
	  char* temp = realloc(tempFile, (size + 1));
	  if (temp == NULL)
            {
	      free(wordsPtr);
	      free(cPtr);
	      free(tempFile);
	      write(2, "Memory allocation error\n", 24);
	      exit(1);
            }
	  tempFile = temp;
	  tempFile[size] = ' ';
	  size++;
        }
      i = 0;
      while (i < size) {
	//beginning with space
	if (i == 0 && tempFile[i] != ' ')
	  {
	    wordCount++;
	  }
            
	if (tempFile[i] == ' ')
	  {
	    while (i < size && tempFile[i] == ' ')
	      {
		i++;
	      }
	    if (i < size)
	      {
		wordCount++;
	      }
	  }
	i++;
      }
      if (wordCount > 0)
        {
	  char** tempWords = (char **)realloc(wordsPtr, wordCount * sizeof(char *));
	  if (tempWords == NULL)
            {
	      free(cPtr);
	      free(tempFile);
	      write(2, "Memory allocation error\n", 24);
	      exit(1);
            }
	  wordsPtr = tempWords;
	  for (j = 0; j < size; j++)
            {
	      char c = tempFile[j];
	      if (c == ' ')
                {
		  if (cIndex == 0)
                    {
		      continue;
                    }
		  char* tempC = (char *)realloc(cPtr, (cIndex + 1) * sizeof(char));
		  if (tempC == NULL)
                    {
		      free(tempFile);
		      if (wordsPtr[0] != NULL)
                        {
			  for (i = 0; i < wordsIndex; i++) {
			    free(wordsPtr[i]);
			  }
                        }
		      free(wordsPtr);
		      if (cPtr != NULL)
			free(cPtr);
		      write(2, "Memory allocation error\n", 24);
		      exit(1);
                    }
		  cPtr = tempC;
		  cPtr[cIndex] = c;
		  wordsPtr[wordsIndex] = cPtr;
		  wordsIndex++;
		  cIndex = 0;
		  cPtr = (char*) malloc(sizeof(char));
                }
	      else {
		char* tempC = (char *)realloc(cPtr, (cIndex + 1) * sizeof(char));
		if (tempC == NULL)
		  {
		    free(tempFile);
		    if (wordsPtr[0] != NULL)
		      {
			for (i = 0; i < wordsIndex; i++) {
			  free(wordsPtr[i]);
			}
		      }
		    if (cPtr != NULL)
		      free(cPtr);
		    free(wordsPtr);
		    write(2, "Memory allocation error\n", 24);
		    exit(1);
		  }
		cPtr = tempC;
		cPtr[cIndex] = c;
		cIndex++;
	      }
            }
        }
      free(cPtr);
      cPtr = NULL;
      free(tempFile);
    }
    
  char current[1];
  readStatus = read(0, current, 1);
  if (readStatus < 0)
    {
      write(2, "Input error\n", 12);
      exit(1);
    }
  else if (readStatus == 0)
    {
      //leave this
    }
  else
    {
      cPtr = (char*) malloc(sizeof(char));
      if (current[0] != ' ')
        {
	  cPtr[cIndex] = current[0];
	  cIndex++;
        }
      while(readStatus != 0)
        {
	  readStatus = read(0, current, 1);
	  if (readStatus < 0)
            {
	      write(2, "Input error\n", 12);
	      exit(1);
            }
	  if (current[0] == ' ' || readStatus == 0)
            {
	      if (cIndex == 0 && readStatus != 0)
                {
		  continue;
                }
	      char* tempC = (char *)realloc(cPtr, (cIndex + 1) * sizeof(char));
	      if (tempC == NULL)
                {
		  if (wordsPtr[0] != NULL)
                    {
		      for (i = 0; i < wordsIndex; i++) {
			free(wordsPtr[i]);
		      }
                    }
		  if (cPtr != NULL)
		    free(cPtr);
		  free(wordsPtr);
		  write(2, "Memory allocation error\n", 24);
		  exit(1);
                }
	      cPtr = tempC;
	      if (readStatus == 0)
                {
		  if (cIndex != 0)
                    {
		      cPtr[cIndex] = ' ';
		      char** tempWords = (char **)realloc(wordsPtr, (wordsIndex + 1) * sizeof(char *));
		      if (tempWords == NULL)
                        {
			  if (wordsPtr[0] != NULL)
                            {
			      for (i = 0; i < wordsIndex; i++) {
				free(wordsPtr[i]);
			      }
                            }
			  if (cPtr != NULL)
			    free(cPtr);
			  free(wordsPtr);
			  write(2, "Memory allocation error\n", 24);
			  exit(1);
                        }
		      wordsPtr = tempWords;
		      wordsPtr[wordsIndex] = cPtr;
		      wordsIndex++;
                    }
		  else
                    {
		      free(cPtr);
                    }
                }
	      else
                {
		  cPtr[cIndex] = current[0];
		  char** tempWords = (char **)realloc(wordsPtr, (wordsIndex + 1) * sizeof(char *));
		  if (tempWords == NULL)
                    {
		      if (wordsPtr[0] != NULL)
                        {
			  for (i = 0; i < wordsIndex; i++) {
			    free(wordsPtr[i]);
			  }
                        }
		      if (cPtr != NULL)
			free(cPtr);
		      free(wordsPtr);
		      write(2, "Memory allocation error\n", 24);
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
	    if (tempC == NULL)
	      {
		if (wordsPtr[0] != NULL)
		  {
		    for (i = 0; i < wordsIndex; i++) {
		      free(wordsPtr[i]);
		    }
		  }
		if (cPtr != NULL)
		  free(cPtr);
		free(wordsPtr);
		write(2, "Memory allocation error\n", 24);
		exit(1);
	      }
	    cPtr = tempC;
	    cPtr[cIndex] = current[0];
	    cIndex++;
	  }
        }
    }
    
  qsort(wordsPtr, wordsIndex, sizeof(char *), wrapper);
    
  //print 2D array to stdout
  int tempSize = 0;
  for (i = 0; i < wordsIndex; i++) {
    for (j = 0; ;j++) {
      tempSize++;
      if (wordsPtr[i][j] == ' ')
	break;
    }
    writeStatus = write(1, wordsPtr[i], tempSize);
    if (writeStatus < 0)
      {
	write(2, "Output error\n", 13);
	exit(1);
      }
    tempSize = 0;
  }
    
  for (i = 0; i < wordsIndex; i++) {
    free(wordsPtr[i]);
  }
  free(wordsPtr);
  return 0;
}
