#include "dig.h"

dig_t *dig_initialise(const char *path, size_t(*interface)(char *), void(*set_at)(void *, size_t, void *), size_t type_size)
{
  FILE *fptr = fopen(path, "r");
  if(NULL == fptr)
  {
    printf("[dig err] File %s did not exist.\n", path);
    return NULL;
  }

  fseek(fptr, 0, SEEK_END);
  const size_t fsize = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  printf("[dig info] File %s is %zu bytes long\n", path, fsize);
  
  char *data = (char *)malloc(fsize * sizeof(char) + 1);
  fread(data, fsize, sizeof(char), fptr);
  fclose(fptr);
  data[fsize] = '\0';
  
  dig_t *dig = (dig_t *)malloc(1 * sizeof(dig_t));
  dig->_type_size = type_size;
  token_t *tokens = _dig_tokenise(data);

  size_t token_count = 0;
  token_t *curr_token = tokens;
  while((*curr_token++).type != _DIG_TOKEN_TERMINATE) token_count++;
  printf("[dig info] Generated %zu tokens from %s\n", token_count, path);

  curr_token = tokens;
  while(_DIG_TOKEN_PRAGMA == (*curr_token).type)
  {
    if(0 == strcmp((*curr_token).str, "size"))
    {
      curr_token++;
      if(_DIG_TOKEN_NUMBER != (*curr_token).type)
      {
        printf("[dig error] Size pragma was not followed by a number\n");
        continue;
      }
      dig->_size = (*curr_token).num;
      dig->_type_size = type_size;
      dig->_entries = (void *)malloc(dig->_size * dig->_type_size);
      
      for(size_t i = 0; dig->_size > i; ++i)
      {
        *((char **)((char *)dig->_entries + i * dig->_type_size)) = NULL;
      }
      printf("[dig info] Pragma set size to %zu entries\n", dig->_size);
    } else 
    {
      printf("[dig error] Unrecognized pragma command \"#%s\"\n", (*curr_token).str);
    }
    curr_token++;
  }
  
  while(_DIG_TOKEN_ENTRY == (*curr_token).type)
  {
    char *entry = (*curr_token).str;
    size_t i = dig_hash(entry) % dig->_size;
    
    while(NULL != *((char **) ((char *)dig->_entries + i * dig->_type_size)))
    {
      i = (i + 1) % dig->_size;
    }
    
    set_at((char *)dig->_entries + i * dig->_type_size, 0, strdup(entry));
    
    curr_token++;
    
    while(_DIG_TOKEN_FIELD_NAME == (*curr_token).type)
    {
      size_t j = interface((*curr_token).str);
      curr_token++;
      
      if(_DIG_TOKEN_COLON != (*curr_token).type)
      {
        printf("[dig error] Colon did not follow field name\n");
        continue;
      }
      curr_token++;
      
      token_e type = (*curr_token).type;
      if(_DIG_TOKEN_STRING == type || _DIG_TOKEN_ENTRY == type)
      {
        set_at((char *)dig->_entries + i * dig->_type_size, j, strdup((*curr_token).str));
        printf("[dig info] Set %s:%s to %s\n", entry, (*(curr_token - 2)).str, (*curr_token).str);
        curr_token++;
      } else if(_DIG_TOKEN_NUMBER == type)
      {
        set_at((char *)dig->_entries + i * dig->_type_size, j, &(*curr_token).num);
        printf("[dig info] Set %s:%s to %d\n", entry, (*(curr_token - 2)).str, (*curr_token).num);
        curr_token++;
      } else
      {
        printf("[dig error] (String | Number | Entry) value did not follow field name and colon\n");
        continue;
      }
    }
  }

  _dig_destroy_tokens(tokens);
  free(data);
  printf("[dig info] Successfully loaded %s\n", path);
  return dig;
}

void *dig_retrieve(dig_t *dig, char *entry)
{
  size_t i = dig_hash(entry) % dig->_size;
  
  while(NULL != *((char **) ((char *)dig->_entries + i * dig->_type_size)) && 0 != strcmp(*((char **) ((char *)dig->_entries + i * dig->_type_size)), entry)) 
  {
    i = (i + 1) % dig->_size;
  }
  
  if(NULL == *((char **)((char *)dig->_entries + i * dig->_type_size)))
  {
    return NULL;
  }
  
  return (char *)dig->_entries + i * dig->_type_size;
}

void dig_destroy(dig_t *dig)
{
  for(size_t i = 0; dig->_size > i; ++i)
  {
    free(*((char **)((char *)dig->_entries + i * dig->_type_size)));
  }
  free(dig->_entries);
  free(dig);
}