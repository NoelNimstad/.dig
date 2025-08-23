#ifndef DIG_H
#define DIG_H

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "tokenise.h"

typedef struct 
{
  void *_entries;
  size_t _size;
  size_t _type_size;
} dig_t;

typedef enum
{
  DIG_STRING,
  DIG_NUMBER,
  DIG_REFERENCE,
  DIG_UNKNOWN
} dig_value_type_e;

typedef struct
{
  dig_value_type_e type;
  union
  {
    char *str;
    int num;
  } value;
} dig_value_t;

dig_t *dig_initialise(const char *path, size_t(*interface)(char *), void(*set_at)(void *, size_t, void *), size_t type_size);
void *dig_retrieve(dig_t *dig, char *entry);
void dig_write(const char *path, dig_t *dig, void(*get_at)(void *, size_t, dig_value_t*), char *field_names[], size_t num_fields);
void dig_destroy(dig_t *dig);

#endif