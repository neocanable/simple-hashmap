#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  hashmap *h = hashmap_init();
  if (h == NULL) return -1;

  for (int index=0; index<100; index+=1)
  {
      char *key = (char *)malloc(30);
      char *val = (char *)malloc(30);
      snprintf(key, 100, "%s%d", "key_prefix_", index);
      snprintf(val, 100, "%s%d", "val_prefix_", index);
      hashmap_put(h, key, val);
  }

  char *key_s = (char*) malloc(30);
  snprintf(key_s, 100, "%s%d", "key_prefix_", 23);
  printf("hashmap size: %d, table_size: %d\n", h->size, h->table_size);
  char *val_s = (char *)hashmap_get(h, key_s);
  printf("the value of %s is: %s\n", key_s, val_s);

  hashmap_node *node = (hashmap_node*)malloc(sizeof(hashmap_node));
  int status = hashmap_get_node(h, key_s, node);
  printf("the value of %s is: %s and status is: %d\n", key_s, node->data, status);

  hashmap_release(h);
  return 0;
}
