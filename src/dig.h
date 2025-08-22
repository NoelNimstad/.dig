#ifndef DIG_H
#define DIG_H

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "token.h"

typedef struct 
{
  void *_entries;
  size_t _size;
  size_t _type_size;
} dig_t;

dig_t *dig_initialise(const char *path, size_t(*interface)(char *), void(*set_at)(void *, size_t, char *), size_t type_size);
void *dig_retrieve(dig_t *dig, char *entry);
void dig_destroy(dig_t *dig);

#endif