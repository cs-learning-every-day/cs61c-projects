/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  void *s can be safely cast
 * to a char * (null terminated string)
 */
unsigned int stringHash(void *s)
{
  // -- TODO --
  // fprintf(stderr, "need to implement stringHash\n");

  /* To suppress compiler warning until you implement this function, */

  char *str = (char *)s;
  unsigned long hash = 0;
  for (int i = 0; i < strlen(str); i++)
  {
    hash = hash * 31 + str[i];
  }
  return hash % dictionary->size;

  // http://www.cse.yorku.ca/~oz/hash.html
  // unsigned long hash = 5381;
  // int c;
  // while (c = *str)
  // {
  //   hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  //   str++;
  // }
  // return hash;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2)
{
  // -- TODO --
  // fprintf(stderr, "You need to implement stringEquals");

  /* To suppress compiler warning until you implement this function */
  char *str1 = (char *)s1;
  char *str2 = (char *)s2;
  int len1 = strlen(str1);
  int len2 = strlen(str2);

  if (len1 != len2)
  {
    return 0;
  }

  for (int i = 0; i < len1; i++)
  {
    if (str1[i] != str2[i])
    {
      return 0;
    }
  }
  return 1;
}

/*
 * This function should read in every word and replacement from the dictionary
 * and store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final bit of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(61)
 * to cleanly exit the program.
 */
void readDictionary(char *dictName)
{
  // -- TODO --
  // fprintf(stderr, "You need to implement readDictionary\n");
  FILE *fptr;

  fptr = fopen(dictName, "r");
  if (fptr == NULL)
  {
    printf("File Open Error!");
    exit(EXIT_FAILURE);
  }

  char buffer[1024];
  char *str1 = malloc(sizeof(char) * 61);
  char *str2 = malloc(sizeof(char) * 61);
  while (fgets(buffer, sizeof(buffer), fptr) != NULL) // one line
  {
    sscanf(buffer, "%s %s", str1, str2);
    insertData(dictionary, str1, str2);
    str1 = malloc(sizeof(char) * 61);
    str2 = malloc(sizeof(char) * 61);
  }

  fclose(fptr);
}

/*
 * This should process standard input (stdin) and perform replacements as 
 * described by the replacement set then print either the original text or 
 * the replacement to standard output (stdout) as specified in the spec (e.g., 
 * if a replacement set of `taest test\n` was used and the string "this is 
 * a taest of  this-proGram" was given to stdin, the output to stdout should be 
 * "this is a test of  this-proGram").  All words should be checked
 * against the replacement set as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the replacement set should 
 * it report the original word.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final bit of your grade, you cannot assume words have a bounded length.
 */
void processInput()
{
  // -- TODO --
  // fprintf(stderr, "You need to implement processInput\n");
  char buffer[1024];
  char words[61];
  char *data = malloc(sizeof(char) * 61);
  if (NULL == data)
  {
    fprintf(stderr, "Malloc Error");
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, sizeof(buffer), stdin) != NULL) // one line
  {

    // remove \n
    buffer[strlen(buffer) - 1] = '\0';

    // one word

    int nums_now, bytes_now;
    int bytes_consumed = 0;

    while ((nums_now =
                sscanf(buffer + bytes_consumed, "%s%n", words, &bytes_now)) > 0)
    {
      if (bytes_consumed != 0)
      {
        printf(" ");
      }
      bytes_consumed += bytes_now;
      words[bytes_now] = '\0';

      // ipsuM! -> i%#@!!
      char origin_punct[strlen(words)];
      getPuncts(words, origin_punct);

      char tmp_remove_str[61];
      memset(tmp_remove_str, 0, 61);
      strncpy(tmp_remove_str, words, strlen(words));
      removePuncts(tmp_remove_str);

      // case 1
      data = (char *)findData(dictionary, (void *)tmp_remove_str);
      if (data != NULL)
      {
        fprintf(stdout, "%s%s", data, origin_punct);
        continue;
      }

      // case 2
      char tmp_words[61];
      case23String(tmp_remove_str, tmp_words, 2);
      data = (char *)findData(dictionary, (void *)tmp_words);
      if (data != NULL)
      {
        fprintf(stdout, "%s%s", data, origin_punct);
        continue;
      }

      // case 3
      case23String(tmp_remove_str, tmp_words, 3);
      data = (char *)findData(dictionary, (void *)tmp_words);

      if (data != NULL)
      {
        fprintf(stdout, "%s%s", data, origin_punct);
        continue;
      }
      fprintf(stdout, "%s", words);
    }

    fprintf(stdout, "\n");
  }
}

// 去除标点符号
void removePuncts(char *words)
{
  int idx = 0;
  for (int i = 0; i < strlen(words); i++)
  {
    if (!ispunct(words[i]))
    {
      words[idx++] = words[i];
    }
  }
  words[idx] = '\0';
}

// 获取除标点符号
void getPuncts(char *words, char *writer)
{
  int idx = 0;
  for (int i = 0; i < strlen(words); i++)
  {
    if (ispunct(words[i]))
    {
      writer[idx++] = words[i];
    }
  }
  writer[idx] = '\0';
}

void case23String(char *buffer, char *writer, int flag)
{
  if (strlen(buffer) == 0)
    return;

  writer[0] = buffer[0];
  if (flag == 2 && strlen(buffer) == 1)
  {
    return;
  }

  int i;

  if (flag == 2)
  {
    i = 1;
  }
  else
  {
    i = 0;
  }

  for (; buffer[i] != '\0'; i++)
  {
    if (isalpha(buffer[i]))
    {
      writer[i] = tolower(buffer[i]);
    }
    else
    {
      writer[i] = buffer[i];
    }
  }
  writer[i] = '\0';
}