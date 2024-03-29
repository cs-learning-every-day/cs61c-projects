#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *))
{
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  if (NULL == newTable)
  {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newTable->size = size;
  newTable->data = malloc(sizeof(HashBucket *) * size);
  if (NULL == newTable->data)
  {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  for (i = 0; i < size; i++)
  {
    newTable->data[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/*
 * This inserts a key/data pair into a hash table.  To use this
 * to store strings, simply cast the char * to a void * (e.g., to store
 * the string referred to by the declaration char *string, you would
 * call insertData(someHashTable, (void *) string, (void *) string).
 */
void insertData(HashTable *table, void *key, void *data)
{
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  // 2. Allocate a new hash bucket struct.
  // 3. Append to the linked list or create it if it does not yet exist.
  int idx = table->hashFunction(key);
  HashBucket *newBucked = (HashBucket *)malloc(sizeof(HashBucket));
  if (NULL == newBucked)
  {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newBucked->key = key;
  newBucked->data = data;

  if (table->data[idx] == NULL)
  {

    table->data[idx] = newBucked;
    table->data[idx]->next = NULL;
  }
  else
  {
    newBucked->next = table->data[idx];
    table->data[idx] = newBucked;
  }
}

/*
 * This returns the corresponding data for a given key.
 * It returns NULL if the key is not found. 
 */
void *findData(HashTable *table, void *key)
{
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  // 2. Walk the linked list and check for equality with table->equalFunction.
  int idx = table->hashFunction(key);
  if (table->data[idx] != NULL)
  {
    // 可能会hash冲突
    HashBucket *bucket = table->data[idx];
    while (1)
    {
      if (table->equalFunction(bucket->key, key))
      {
        return bucket->data;
      }
      bucket = bucket->next;
      if (bucket == NULL)
        break;
    }
  }
  return NULL;
}
