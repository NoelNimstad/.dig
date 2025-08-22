#include "../src/dig.h"

#define DIG_FIELD_CHARACTER 0
#define DIG_FIELD_VALUE     1
#define DIG_FIELD_NEXT      2
size_t dig_interface(char *field)
{
  size_t res = -1;
  if(field[0] == 'c') res = DIG_FIELD_CHARACTER;
  if(field[0] == 'v') res = DIG_FIELD_VALUE;
  if(field[0] == 'n') res = DIG_FIELD_NEXT;
  return res + 1; // error
}

typedef struct
{
  char *key;
  char *values[2];
  char *next;
} dig_entry_t;

void dig_set_at(void *entry, size_t field, char *value)
{
  dig_entry_t *dig_entry = (dig_entry_t *)entry;
  if(0 == field)
  {
    dig_entry->key = value;
    return;
  } else if(3 == field)
  {
    dig_entry->next = value;
    return;
  }

  dig_entry->values[field - 1] = value;
}

int main(int argc, char const *argv[])
{
  if(2 > argc)
  {
    return 1;
  }

  dig_t *dig = dig_initialise(argv[1], dig_interface, dig_set_at, sizeof(dig_entry_t));

  printf("--------------------------------------------\n");

  dig_entry_t *test = (dig_entry_t *)dig_retrieve(dig, "DIALOGUE_INTRO_1");
  while(NULL != test->next)
  {
    printf("%s: \"%s\"\n", test->values[DIG_FIELD_CHARACTER], test->values[DIG_FIELD_VALUE]);
    test = (dig_entry_t *)dig_retrieve(dig, test->next);
  }

  dig_destroy(dig);

  return 0;
}