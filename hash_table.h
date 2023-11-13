#include <stdlib.h>

typedef struct hash_entry HASH_ENTRY;
typedef struct hash_table HASH_TABLE;

unsigned long djb2 (const void* key, const size_t key_size);
unsigned long sdbm(const void*key, const size_t size);


HASH_TABLE* create_ht(size_t size, int(*equal)(const void*, const size_t, const void*, const size_t),
	   					unsigned long (*hash)(const void*, const size_t),
					   	unsigned long (*co_hash)(const void*, const size_t));
int ht_add(HASH_TABLE* ht, const void* key, const size_t key_size, void* value, size_t value_size);
int ht_remove(HASH_TABLE* ht, const void* key, size_t key_size);
void* ht_get(HASH_TABLE* ht, const void* key, size_t key_size);
void clear_table(HASH_TABLE* ht, size_t size);
void delete_table(HASH_TABLE* ht);
