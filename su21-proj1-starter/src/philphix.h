#ifndef _PHILPHIX_H
#define _PHILPHIX_H

/* The pointer to store the dictionary for replacement entries */
extern struct HashTable *dictionary;

extern unsigned int stringHash(void *s);

extern int stringEquals(void *s1, void *s2);

extern void readDictionary(char *dictName);

extern void processInput();

void printString(char *str1, char *str2, char *str3, int i);

#endif /* _PHILPHIX_H */
