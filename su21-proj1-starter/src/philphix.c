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

#include <assert.h>

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
  FILE *fptr = fopen(dictName, "r");
  if (fptr == NULL) {
    fprintf(stderr, "Error: open dictionary file not exit: %s\n", dictName);
    exit(61);
  }

  char *key = (char *)malloc(sizeof(char) * 70);
  char *value = (char *)malloc(sizeof(char) * 70);
  assert(key != NULL);
  assert(value != NULL);

  while (fscanf(fptr, " %1023s", key) == 1) {
    if (fscanf(fptr, " %1023s", value) == 1) {
      fprintf(stderr, "readDictionary: insert (%s, %s)\n", key, value);
      insertData(dictionary, key, value); // hashtable里存储的是引用
      key = (char *)malloc(sizeof(char) * 70);
      value = (char *)malloc(sizeof(char) * 70);
    }
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
  char *str1 = (char *)malloc(70); // 正常情况
  char *str2 = (char *)malloc(70); // 除第一个字母后面都是小写字母情况
  char *str3 = (char *)malloc(70); // 全是小写字母情况
  assert(str1 != NULL);
  assert(str2 != NULL);
  assert(str3 != NULL);

  int c;
  int i = 0;
  int size = 70;

  while ((c = fgetc(stdin)) != EOF) {
    if (isalpha(c) != 0) {
      if (i == size) {
        size = size * 2;
        str1 = (char *)realloc(str1, size);
        str2 = (char *)realloc(str2, size);
        str3 = (char *)realloc(str3, size);
        assert(str1 != NULL);
        assert(str2 != NULL);
        assert(str3 != NULL);
      }
      str1[i] = (char)c;
      if (i == 0) {
        str2[i] = (char)c;
      } else {
        str2[i] = (char)tolower(c);
      }

      str3[i] = (char)tolower(c);
      i++;
    } else { // 符号
      if (i != 0) {
        printString(str1, str2, str3, i);
      }
      fprintf(stdout, "%c", (char)c);
      i = 0;
      memset(str1, 0, strlen(str1));
      memset(str2, 0, strlen(str2));
      memset(str3, 0, strlen(str3));
    }
  }

  if (i != 0) {
    printString(str1, str2, str3, i);
  }

  free(str3);
  free(str2);
  free(str1);
}

void printString(char *str1, char *str2, char *str3, int i) {
  str1[i] = '\0';
  str2[i] = '\0';
  str3[i] = '\0';

  char *tmp1 = (char *)findData(dictionary, str1);
  char *tmp2 = (char *)findData(dictionary, str2);
  char *tmp3 = (char *)findData(dictionary, str3);

  if (tmp1 != NULL) { // case 1
    fprintf(stdout, "%s", tmp1);
  } else if (tmp2 != NULL) { // case 2
    fprintf(stdout, "%s", tmp2);
  } else if (tmp3 != NULL) { // case 3
    fprintf(stdout, "%s", tmp3);
  } else {
    fprintf(stdout, "%s", str1);
  }
}