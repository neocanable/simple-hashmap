#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#define HASHMAP_TABLE_SIZE (256)
#define HASHMAP_RETRY_SIZE (8)

#define MAP_OK 0
#define MAP_FULL -1
#define MAP_MISS -2
#define MAP_ERROR -3

typedef struct _hashmap_node {
    char *key;
    int used;
    void *data;
} hashmap_node;

typedef struct _hashmap {
    int table_size;
    int size;
    hashmap_node *nodes;
} hashmap;

// init hashmap
extern hashmap* hashmap_init();

// push anydata to hashmap for key
extern int hashmap_put(hashmap *hashmap, char *key, void *anydata);

// remove key
extern int hashmap_del(hashmap *hashmap, char *key);

// free hashmap
extern void hashmap_release(hashmap *hashmap);

// return hashmap dst_node pointer, if key missing, return MAP_MISS
extern int hashmap_get_node(hashmap *hashmap, char *key, hashmap_node *dst_node);

// return hashmap_node->data, if miss, return NULL
extern void* hashmap_get(hashmap *hashmap, char *key);

// return hashmap size
extern int hashmap_size(hashmap *hashmap);

#endif // __HASHMAP_H__
