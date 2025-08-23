#include "../src/dig.h"

#define DIG_FIELD_TEXT      1
#define DIG_FIELD_REFERENCE 2
#define DIG_FIELD_ID        3
size_t dig_interface(char *field)
{
  size_t res = 0;
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
    case DIG_FIELD_REFERENCE:
      dig_entry->reference = (char *)value;
      break;
    case DIG_FIELD_ID:
      dig_entry->id = *((int *)value);
      break;
  }
}

void dig_get_at(void *entry, size_t field, dig_value_t *value)
{
  dig_entry_t *dig_entry = (dig_entry_t *)entry;
  switch(field)
  {
    case DIG_FIELD_TEXT:
      value->type = DIG_STRING;
      value->value.str = dig_entry->text;
      break;
    case DIG_FIELD_REFERENCE:
      value->type = DIG_REFERENCE;
      value->value.str = dig_entry->reference;
      break;
    case DIG_FIELD_ID:
      value->type = DIG_NUMBER;
      value->value.num = dig_entry->id;
      break;
    default:
      value->type = DIG_UNKNOWN;
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
  printf("test string: %s\ntest ID: %d\n", test->text, test->id);
  test->id += 1;

  char *field_names[3] = {"text", "reference", "id"};
  dig_write("test/test.dig", dig, dig_get_at, field_names, 3);
  dig_destroy(dig);
  return 0;
}