#ifndef HASH_H_
#define HASH_H_

#define HASHSIZE 101

struct entry
{
  struct entry* next;
  char* key;
  char* value;
};

unsigned int hash(char* s);
struct entry* lookup(char* s, struct entry* table);
struct entry* insert(char* key, char* value, struct entry* table);

#endif // HASH_H_