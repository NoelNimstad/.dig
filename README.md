## `.dig` – A new data format made for use with C
### Examples
#### Dialogue & Characters
```js
// characters.dig
// characters form Boneworks
#size 2
#unserialisable

HAYES
	$photo: "res/hayes.png"
	$name: "Hayes M."
	$age: 34

ARTHUR
	$photo: "res/arthur.png"
	$name: "Arthur F."
	$age: 33
```
```js
// dialogue.dig
// dialogue from Boneworks
#size 4
#unserialisable

STREETS_HAYES_1
	$text: "Yo, Arthur! Over here!"
	$character: HAYES
	$next: STREETS_HAYES_2

STREETS_HAYES_2
	$text: "How are you still in the city?"
	$character: HAYES
	$next: STREETS_HAYES_3

STREETS_HAYES_3
	$text: "The board's calling an emergency meeting for all Monogon employees."
	$character: HAYES
	$next: STREETS_HAYES_4

STREETS_HAYES_4
	$text: "All of MythOS is on lockdown, it's crazy over here."
	$character: HAYES
```
```c
#include <stdio.h>
#include <dig.h>

typedef struct
{
  char *key;
  char *photo;
  char *name;
  int age;
} character_t;

#define FIELD_PHOTO 1
#define FIELD_NAME  2
#define FIELD_AGE   3
size_t character_interface(char *field)
{
  size_t res = 0;
  // all fields have unique initial letters
  if(field[0] == 'p') res = FIELD_PHOTO;
  if(field[0] == 'n') res = FIELD_NAME;
  if(field[0] == 'a') res = FIELD_AGE;
  return res;
}

void dig_set_character(void *entry, size_t field, void *value)
{
  character_t *character = (character_t *)entry;
  switch(field)
  {
    case 0:
      character->key = (char *)value;
      break;
		case FIELD_PHOTO:
      character->photo = (char *)value;
      break;
		case FIELD_NAME:
      character->name = (char *)value;
      break;
		case FIELD_AGE:
			character->age = *(int *)value;
			break;
  }
}

typedef struct
{
  char *key;
  char *text;
  char *character;
  char *next;
} dialogue_t;

#define FIELD_TEXT      1
#define FIELD_CHARACTER 2
#define FIELD_NEXT      3
size_t dialogue_interface(char *field)
{
  size_t res = 0;
  if(field[0] == 't') res = FIELD_TEXT;
  if(field[0] == 'c') res = FIELD_CHARACTER;
  if(field[0] == 'n') res = FIELD_NEXT;
  return res;
}

void dig_set_dialogue(void *entry, size_t field, void *value)
{
  dialogue_t *dialogue = (dialogue_t *)entry;
  switch(field)
  {
    case 0:
      dialogue->key = (char *)value;
      break;
		case FIELD_TEXT:
      dialogue->text = (char *)value;
      break;
		case FIELD_CHARACTER:
      dialogue->character = (char *)value;
      break;
		case FIELD_NEXT:
			dialogue->next = (char *)value;
			break;
  }
}

int main(void)
{
	dig_t *characters = dig_initialise("characters.dig", character_interface, dig_set_character, sizeof(character_t));
	dig_t *dialogue = dig_initialise("dialogue.dig", dialogue_interface, dig_set_dialogue, sizeof(dialogue_t));

	dialogue_t *current_dialogue = (dialogue_t *)dig_retrieve(dialogue, "STREETS_HAYES_1");
	do
	{
		character_t *current_character = (character_t *)dig_retrieve(characters, current_dialogue->character);
		printf("%s says \"%s\"\n", current_character->name, current_dialogue->text);
		current_dialogue = (dialogue_t *)dig_retrieve(dialogue, current_dialogue->next);
	} while(current_dialogue != NULL);

	dig_destroy(characters);
	dig_destroy(dialogue);
	return 0;
}

/*
  Hayes M. says "Yo, Arthur! Over here!"
  Hayes M. says "How are you still in the city?"
  Hayes M. says "The board's calling an emergency meeting for all Monogon employees."
  Hayes M. says "All of MythOS is on lockdown, it's crazy over here."
*/
``` 