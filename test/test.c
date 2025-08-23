#include "../src/dig.h"

#define DIG_FIELD_TEXT      1
#define DIG_FIELD_REFERENCE 2
#define DIG_FIELD_ID        3
size_t dig_interface(char *field)
{
  size_t res = -1;
  if(field[0] == 't') res = DIG_FIELD_TEXT;
  if(field[0] == 'r') res = DIG_FIELD_REFERENCE;
  if(field[0] == 'i') res = DIG_FIELD_ID;
  return res; // error
}

typedef struct
{
  char *key;
  char *text;
  char *reference;
  int id;
} dig_entry_t;

void dig_set_at(void *entry, size_t field, void *value)
{
  dig_entry_t *dig_entry = (dig_entry_t *)entry;
  switch(field)
  {
    case 0:
      dig_entry->key = (char *)value;
      break;
    case DIG_FIELD_TEXT:
      dig_entry->text = (char *)value;
      break;
    case DIG_FIELD_ID:
      dig_entry->id = *((int *)value);
      break;
    case DIG_FIELD_REFERENCE:
      dig_entry->text = (char *)value;
      break;
  }
}

int main(int argc, char const *argv[])
{
  if(2 > argc)
  {
    return 1;
  }

  dig_t *dig = dig_initialise(argv[1], dig_interface, dig_set_at, sizeof(dig_entry_t));

  printf("--------------------------------------------------------\n");

  dig_entry_t *test = (dig_entry_t *)dig_retrieve(dig, "DIG_TESTING");
  printf("test ID: %d\n", test->id);

  dig_destroy(dig);
  return 0;
}