#include <stdlib.h>

typedef struct hash_entry hash_entry_t;
typedef struct hash_table hash_table_t;

unsigned long djb2 (void* key, const size_t key_size);

hash_table_t* create_ht(size_t size, int(*equal)(const void*, const void*),
	   					unsigned long (*hash)(const void*, size_t),
					   	unsigned long (*co_hash)(const void*, size_t));
int ht_add(hash_table_t* ht, void* key, size_t key_size, void* value);
int ht_remove(hash_table_t* ht, void* key);
void* ht_get(hash_table_t* ht, void* key);
int ht_set(hash_table_t* ht, void* key, void* value);
