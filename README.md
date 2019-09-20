# simple hashmap


### Usage
```
#include "hashmap.h"
int main(int argc, char **argv)
{
  hashmap *map = hashmap_init(); // init hashmap
  if (h == NULL) return -1;

  for (int index=0; index<100; index+=1)
  {
      char *key = (char *)malloc(30);
      char *val = (char *)malloc(30);
      snprintf(key, 100, "%s%d", "key_prefix_", index);
      snprintf(val, 100, "%s%d", "val_prefix_", index);
      hashmap_put(h, key, val);
  }
  hashmap_release(map); // free hashmap and nodes

}
```
