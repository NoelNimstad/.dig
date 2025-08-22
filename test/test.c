#include "../src/dig.h"

#define DIG_FIELD_CHARACTER 0
#define DIG_FIELD_VALUE     1
size_t dig_interface(char *field)
{
  size_t res = -1;
  if(field[0] == 'c') res = DIG_FIELD_CHARACTER;
  if(field[0] == 'v') res = DIG_FIELD_VALUE;
  return res + 1; // error
}

typedef struct
{
  char *key;
  char *values[2];
} dig_entry_t;

void dig_set_at(void *entry, size_t field, char *value)
{
  dig_entry_t *dig_entry = (dig_entry_t *)entry;
  if(0 == field)
  {
    dig_entry->key = value;
  } else
  {
    dig_entry->values[field - 1] = value;
  }
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
  printf("%s says \"%s\"\n", test->values[DIG_FIELD_CHARACTER], test->values[DIG_FIELD_VALUE]);

  test = (dig_entry_t *)dig_retrieve(dig, "DIALOGUE_INTRO_2");
  printf("%s says \"%s\"\n", test->values[DIG_FIELD_CHARACTER], test->values[DIG_FIELD_VALUE]);

  test = (dig_entry_t *)dig_retrieve(dig, "DIALOGUE_INTRO_3");
  printf("%s says \"%s\"\n", test->values[DIG_FIELD_CHARACTER], test->values[DIG_FIELD_VALUE]);

  test = (dig_entry_t *)dig_retrieve(dig, "DIALOGUE_INTRO_ERROR");
  printf("%s says \"%s\"\n", test->values[DIG_FIELD_CHARACTER], test->values[DIG_FIELD_VALUE]);

  dig_destroy(dig);

  return 0;
}