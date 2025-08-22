#ifndef TOKEN_H
#define TOKEN_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define _DIG_TOKEN_BUFFER_SIZE 1024

typedef enum
{
  _DIG_TOKEN_PRAGMA,
  _DIG_TOKEN_NUMBER,
  _DIG_TOKEN_STRING,
  _DIG_TOKEN_ENTRY,
  _DIG_TOKEN_FIELD_NAME,
  _DIG_TOKEN_COLON,
  _DIG_TOKEN_COMMA,
  _DIG_TOKEN_ERROR,
  _DIG_TOKEN_TERMINATE,
} token_e;

typedef struct 
{
  token_e type;
  uint8_t has_allocated_value;
  union
  {
    char _c;
    char *str;
    uint16_t num;
  };
  
} token_t;

token_t *_dig_tokenise(char *data);
void _dig_destroy_tokens(token_t *tokens);

#endif