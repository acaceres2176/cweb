#include <stdlib.h>
#include <string.h>

#include "hash.h"

unsigned int hash(char* s)
{
    unsigned int hashval;
  
    for(hashval=0;s!='\0';s++)
    {
        hashval = *s + 31 * hashval;
    }
  
    return hashval % HASHSIZE;
}

struct entry* lookup(char* s, struct entry* table)
{
    struct entry* np;
  
    for(np=&table[hash(s)];np!=NULL;np=np->next)
    {
        if(strcmp(s, np->key) == 0)
        {
            return np;
        }
    }
  
    return NULL;
}

struct entry* insert(char* key, char* value, struct entry* table)
{
    struct entry* np;
    unsigned int hashval;
  
    if((np = lookup(key, table)) == NULL)
    {
        /* not found */
        np = malloc(sizeof(np));
    
        if(np == NULL || (np->key = strdup(key)) == NULL)
        {
            return NULL;
        }
    
        hashval = hash(key);
        np->next = &table[hashval];
        table[hashval] = *np;
    }
    else
    {
        free((void*)np->value);
    }
  
    if((np->value = strdup(value)) == NULL)
    {
        return NULL;
    }
  
    return np;
}
