#ifndef _PHILPHIX_H
#define _PHILPHIX_H

/* The pointer to store the dictionary for replacement entries */
extern struct HashTable *dictionary;

extern unsigned int stringHash(void *s);

extern int stringEquals(void *s1, void *s2);

extern void readDictionary(char *dictName);

extern void processInput();

void case23String(char *buffer, char *writer, int flag);
void getPuncts(char *words, char *writer);
void removePuncts(char *words);

#endif /* _PHILPHIX_H */
