#include "hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// http://www.hackersdelight.org/hdcodetxt/crc.c.txt
unsigned int crc32b(unsigned char *key) 
{
   int i, j;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (key[i] != 0) {
      byte = key[i];
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) 
      {
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i += 1;
   }
   return ~crc;
}

int hashmap_get_index(hashmap *hashmap, char *key)
{
  if (hashmap->size >= (hashmap->table_size / 2))
    return MAP_FULL;

  unsigned int hash_val = crc32b((unsigned char*)key);
  unsigned int index = hash_val % hashmap->table_size;

  for (int i = 0; i < HASHMAP_RETRY_SIZE; i++)
  {
    if ( hashmap->nodes[index].used == 0 )
      return index;
    if ( hashmap->nodes[index].used == 1 && strcmp(hashmap->nodes[index].key, key) == 0 )
      return index;

    index = (index + 1) % hashmap->table_size;
  }
  return MAP_FULL;
}

hashmap* hashmap_init()
{
  hashmap *m = (hashmap *) malloc(sizeof(hashmap));
  if (!m) goto err;

  m->size = 0;
  m->table_size = HASHMAP_TABLE_SIZE;
  m->nodes = (hashmap_node *) malloc(sizeof(hashmap_node)*m->table_size);
  memset(m->nodes, 0, sizeof(hashmap_node)*m->table_size); // setup empty

  return m;
  err:
    if( m )
    {
      hashmap_release(m);
    }
    return NULL;
}

int hashmap_rehash(hashmap *hashmap)
{
  int status;
  int original_table_size = hashmap->table_size;
  hashmap->table_size = 2 * hashmap->table_size;
  hashmap_node *old_node_list = hashmap->nodes;
  hashmap_node *new_node_list = (hashmap_node *) malloc(sizeof(hashmap_node)*hashmap->table_size);
  memset(new_node_list, 0, sizeof(hashmap_node)*hashmap->table_size);
  hashmap->nodes = new_node_list;
  hashmap->size = 0;
  for (int i = 0; i < original_table_size; i++)
  {
    if (old_node_list[i].used == 0)
      continue;
    hashmap_put(hashmap, old_node_list[i].key, old_node_list[i].data);

  }
  free(old_node_list);

  return MAP_OK;
}

void hashmap_release(hashmap *hashmap)
{
  free(hashmap->nodes);
  free(hashmap);
}

int hashmap_put(hashmap *hashmap, char *key, void *anydata)
{
  int index = hashmap_get_index(hashmap, key);
  while( index == MAP_FULL )
  {
    hashmap_rehash(hashmap);
    index = hashmap_get_index(hashmap, key);
  }

  hashmap->nodes[index].key = key;
  hashmap->nodes[index].used = 1;
  hashmap->nodes[index].data = anydata;
  hashmap->size++;
  return MAP_OK;
}

int hashmap_get_node(hashmap *hashmap, char *key, hashmap_node *dst_node)
{
  int index = hashmap_get_index(hashmap, key);

  if (hashmap->nodes[index].used == 0) return MAP_MISS;
  memcpy(dst_node, &hashmap->nodes[index], sizeof(hashmap_node));
  return MAP_OK;
}

void* hashmap_get(hashmap *hashmap, char *key)
{
  int index = hashmap_get_index(hashmap, key);
  if (hashmap->nodes[index].used == 0) return NULL;
  return hashmap->nodes[index].data;
}

int hashmap_size(hashmap *hashmap)
{
  return hashmap == NULL ? 0 : hashmap->size;
}

int hashmap_del(hashmap *hashmap, char *key)
{
  int index = hashmap_get_index(key);
  if (index == MAP_FULL) return MAP_MISS;
  return 0;
}
