#include "token.h"

static inline char *_dig_read_until_space(char **curr_char)
{
  char buffer[0xff];
  size_t i = 0;
  while(!isspace(*(++*curr_char)))
  {
    buffer[i++] = **curr_char;
  }
  buffer[i] = '\0';
  return strdup(buffer);
}

static inline char *_dig_read_until(char **curr_char, char delim)
{
  char buffer[0xff];
  size_t i = 0;
  while('\0' != *(++*curr_char) && delim != **curr_char)
  {
    buffer[i++] = **curr_char;
  }
  buffer[i] = '\0';
  (*curr_char)--;
  return strdup(buffer);
}

token_t *_dig_tokenise(char *data)
{
  token_t *tokens = (token_t *)malloc(_DIG_TOKEN_BUFFER_SIZE * sizeof(token_t));
  size_t curr_token = 0;

  char *curr_char = data;
  do
  {
    if(isspace(*curr_char)) continue;
    
    if(isnumber(*curr_char))
    {
      tokens[curr_token].type = _DIG_TOKEN_NUMBER;

      uint16_t temp = 0;
      while(isnumber(*curr_char))
      {
        temp = temp * 10 + *(curr_char++) - '0';
      }
      tokens[curr_token++].num = temp;
      continue;
    }

    if(isalpha(*curr_char))
    {
      tokens[curr_token].type = _DIG_TOKEN_ENTRY;

      tokens[curr_token].has_allocated_value = 1;
      curr_char--; // prevent consumption of initial char
      tokens[curr_token++].str = _dig_read_until_space(&curr_char);
      continue;
    }

    switch(*curr_char)
    {
      case '#':
      {
        tokens[curr_token].type = _DIG_TOKEN_PRAGMA;
        
        tokens[curr_token].has_allocated_value = 1;
        tokens[curr_token++].str = _dig_read_until_space(&curr_char);
        break;
      }
      case '$':
      {
        tokens[curr_token].type = _DIG_TOKEN_FIELD_NAME;

        tokens[curr_token].has_allocated_value = 1;
        tokens[curr_token++].str = _dig_read_until(&curr_char, ':');
        break;
      }
      case ':':
      {
        tokens[curr_token++].type = _DIG_TOKEN_COLON;
        break;
      }
      case ',':
      {
        tokens[curr_token++].type = _DIG_TOKEN_COMMA;
        break;
      }
      case '\"':
      {
        tokens[curr_token].type = _DIG_TOKEN_STRING;

        tokens[curr_token].has_allocated_value = 1;
        tokens[curr_token++].str = _dig_read_until(&curr_char, '\"');
        curr_char++;
        break;
      }
      case '/':
      {
        while(*(++curr_char) != '\0' && *curr_char != '\n');
        break;
      }
      case '\0':
      {
        break;
      }
      default:
      {
        tokens[curr_token++].type = _DIG_TOKEN_ERROR;
        tokens[curr_token]._c = *curr_char;
        printf("[dig error] Could not classify token (%d)\n", (uint8_t)tokens[curr_token]._c);
        break;
      }
    }
  } while('\0' != *(curr_char++));

  tokens[curr_token++].type = _DIG_TOKEN_TERMINATE;
  return tokens;
}

void _dig_destroy_tokens(token_t *tokens)
{
  size_t curr_token = -1;
  while(_DIG_TOKEN_TERMINATE != tokens[++curr_token].type)
  {
    if(tokens[curr_token].has_allocated_value) free(tokens[curr_token].str);
  }
  free(tokens);
}