#include "hash.h"

// https://stackoverflow.com/a/21001712
uint32_t dig_hash(char *key)
{
  unsigned int byte, crc, mask;
  int i = 0, j;
  crc = 0xffffffff;
  while(key[i] != 0)
  {
    byte = key[i];
    crc = crc ^ byte;
    for (j = 7; j >= 0; j--)
    {
      mask = -(crc & 1);
      crc = (crc >> 1) ^ (0xedb88320 & mask);
    }
    i = i + 1;
  }
  return ~crc;
}