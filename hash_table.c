#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "hash_table.h"

#define INC_THRESHOLD 70
#define DEC_THRESHOLD 40
#define INITIAL_BUCKET_SIZE 16 


typedef struct hash_entry{
	void* key;		
	void* value;
	size_t key_size;
	size_t value_size;
	int occupied;
} HASH_ENTRY;

typedef struct hash_table{
	HASH_ENTRY* hash_table;
	size_t num_entries;
	size_t size;
	int (*equal)(const void*, const size_t, const void*, const size_t);
	unsigned long (*hash)(const void*, const size_t);
	unsigned long (*co_hash)(const void*, const size_t);
} HASH_TABLE;


/// Default functions

// Modified djb2.
unsigned long djb2 (const void* key, const size_t size)
{
	unsigned long hash = 5381;
	int c;
	char* k = (char*) key;
	for(int i = 0; i < size; i++)
	{
		c = k[i];
		hash = ((hash << 5) + hash) ^ c; 
	}	

	return hash;
}

// Modified sdbm - always odd.
unsigned long sdbm(const void*key, const size_t size)
{
	unsigned long hash = 0;
	int c;
	char* k = (char*) k;
	for(int i = 0 ; i < size; i++)
	{
		c = k[i];
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash | 0x1;

}

int default_equal(const void* key1, const size_t size1, const void* key2, const size_t size2)
{
	if(size1 != size2)
	{
		return 0;
	}

	return !memcmp(key1, key2, size1);
}



static unsigned long probe_get(HASH_TABLE* ht, const void* key, const size_t key_size)
{
	unsigned long i;
	unsigned long first_hash;
	unsigned long j;
	int (*equal)(const void*, const size_t, const void*, const size_t);
	unsigned long (*hash)(const void*, size_t);
	unsigned long (*co_hash)(const void*, size_t);

	if(NULL != ht->equal)
	{
		equal = ht->equal;
	}
	else
	{
		equal = default_equal;
	}

	if(NULL != ht->hash)
	{
		hash = ht->hash;
	}
	else
	{
		hash = djb2;
	}


	if(NULL != ht->co_hash)
	{
		co_hash = ht->co_hash;
	}
	else
	{
		co_hash = sdbm;
	}
	
	i = hash(key, key_size) % ht->size;
	j = co_hash(key, key_size);
	first_hash = i;

	HASH_ENTRY* entries = ht->hash_table;

	while(!(equal(key, key_size, (entries + i)->key, (entries + i)->key_size)) && (entries + i)->occupied != 1)
	{
		printf("searching");	
		i = (i + j) % ht->size;
		if(i == first_hash)
		{
			return -1;
		}
	}
	

	return i;
}

static unsigned long probe_add(HASH_TABLE* ht, const void* key, const size_t key_size)
{
	unsigned long i;
	unsigned long first_hash;
	unsigned long j;
	int (*equal)(const void*, const size_t, const void*, const size_t);
	unsigned long (*hash)(const void*, size_t);
	unsigned long (*co_hash)(const void*, size_t);

	if(NULL != ht->equal)
	{
		equal = ht->equal;
	}
	else
	{
		equal = default_equal;
	}

	if(NULL != ht->hash)
	{
		hash = ht->hash;
	}
	else
	{
		hash = djb2;
	}


	if(NULL != ht-> co_hash)
	{
		co_hash = ht->co_hash;
	}
	else
	{
		co_hash = sdbm;
	}

	
	
	i = hash(key, key_size) % ht->size;
	j = co_hash(key, key_size);
	first_hash = i;


	HASH_ENTRY* entries = ht->hash_table;

	while(NULL != (entries + i)->key && (entries + i)->occupied != 1)
	{
		
		printf("searching add");	
		i = (i + j) % ht->size;
		if(i == first_hash)
		{
			return ht->size;
		}

		// Might use a different code? I dunno
		if(equal(key, key_size, (entries + i)->key, (entries + i)->key_size))
		{
			return ht->size + 1; 
		}
	}

	return i;
}

static int add_entry(HASH_TABLE* ht, const void* key, const size_t key_size, void* value, size_t value_size)
{
	
	unsigned long i = probe_add(ht, key, key_size);

	if(i == ht->size)
	{
		return -1;
	}

	if(i == ht->size + 1)
	{
		return -2;
	}

	ht->hash_table[i].key = malloc(sizeof(key_size));
	if(NULL == ht->hash_table[i].key)
	{
		printf("Out of memory for key");
		exit(1);
	}
	ht->hash_table[i].value = malloc(sizeof(value_size));
	if(NULL == ht->hash_table[i].value)
	{
		printf("Out of memory for value");
		exit(1);
	}

	memcpy(ht->hash_table[i].key, key, key_size);	
	memcpy(ht->hash_table[i].value, value, value_size);
	ht->hash_table[i].key_size = key_size;
	ht->hash_table[i].occupied = 1;
	ht->num_entries++;
	return 0;
}

static HASH_ENTRY* get_entry(HASH_TABLE* ht, const void* key, size_t key_size)
{
	unsigned long i = probe_get(ht, key, key_size);
	if(i == -1)
	{
		return NULL;	
	}

	return &ht->hash_table[i];
}

static int remove_entry(HASH_TABLE* ht, const void* key, size_t key_size)
{

	HASH_ENTRY* entry = get_entry(ht, key, key_size);
	if(NULL == entry)
	{
		return -1;
	}

	entry->occupied = 0;
	ht->num_entries--;
	return 0;
}


static void rehash(HASH_TABLE* old_table, HASH_ENTRY* new_ht)
{
	for(int i = 0; i < old_table ->size; i++)
	{
		HASH_ENTRY* entry = old_table->hash_table + i;
		if(NULL != entry -> key)
		{
			unsigned long new_index = probe_add(old_table, entry->key, entry->key_size);
			if(new_index == -1)
			{
				printf("collision/size error");
				exit(1);
			}

			new_ht[i] = *entry;
		}
	}
};


static void increase_size(HASH_TABLE* ht)
{
	size_t new_size = pow(2, (log2(ht->size)) + 1); 
	HASH_ENTRY* new_table = malloc(sizeof(HASH_ENTRY) * new_size);

	if(NULL == new_table)
	{
		printf("Out of memory");
		exit(1);
	}

	for(int i = 0; i < new_size; i++)
	{
		new_table[i].key = NULL;
		new_table[i].key_size = 0;
		new_table[i].occupied = 0;
		new_table[i].value = NULL;
		new_table[i].value_size = 0;
	}

	rehash(ht, new_table);	
	HASH_ENTRY* old_table = ht->hash_table;
	ht->hash_table = new_table;
	free(old_table);
}


static void decrease_size(HASH_TABLE* ht)
{
	size_t new_size = pow(2, (log2(ht->size)) - 1); 
	HASH_ENTRY* new_table = malloc(sizeof(HASH_ENTRY) * new_size);

	if(NULL == new_table)
	{
		printf("Out of memory");
		exit(1);
	}

	for(int i = 0; i < new_size; i++)
	{
		new_table[i].key = NULL;
		new_table[i].key_size = 0;
		new_table[i].occupied = 0;
		new_table[i].value = NULL;
		new_table[i].value_size = 0;
	}

	rehash(ht, new_table);	
	HASH_ENTRY* old_table = ht->hash_table;
	ht->hash_table = new_table;
	free(old_table);
}


HASH_TABLE* create_ht(size_t size, int(*equal)(const void*, const size_t, const void*, const size_t), unsigned long (*hash)(const void*, size_t), unsigned long (*co_hash)(const void*, size_t))
{

	if (size < 1)
	{
		size = INITIAL_BUCKET_SIZE;
	}

	HASH_TABLE* new_ht = malloc(sizeof(HASH_TABLE));
	if(NULL == new_ht)
	{
		printf("Out of memory");
		exit(1);
	}

	new_ht->hash_table = malloc(size * sizeof(HASH_ENTRY));
	for(int i = 0; i < size; i++)
	{
		new_ht->hash_table[i].key = NULL;
		new_ht->hash_table[i].value = NULL;
		new_ht->hash_table[i].key_size = 0;
		new_ht->hash_table[i].occupied = 0;
		new_ht->hash_table[i].value_size = 0;
	}
	
	if(NULL == new_ht->hash_table)
	{
		printf("Out of memory");
		exit(1);
	}

	new_ht->size = size;
	new_ht->equal = equal;
	new_ht->hash = hash;
	new_ht->num_entries = 0;

	return new_ht;
}



int ht_add(HASH_TABLE* ht, const void* key, const size_t key_size, void* value, const size_t value_size)
{
	if(((float) ht->num_entries * 100 / ht->size) > INC_THRESHOLD)
	{
		increase_size(ht);	
	}

	// This returns 0 on success, -1 on table size error, -2 if item already added.
	return add_entry(ht, key, key_size, value, value_size);
}


int ht_remove(HASH_TABLE* ht, const void* key, size_t key_size)
{
	int success = remove_entry(ht, key, key_size);
	if(((float) ht->num_entries * 100 / ht->size) < DEC_THRESHOLD && ht->size > INITIAL_BUCKET_SIZE)
	{
		decrease_size(ht);
	}
	return success;
}

void* ht_get(HASH_TABLE* ht, const void* key, size_t key_size)
{
	HASH_ENTRY* entry = get_entry(ht, key, key_size);
	if(NULL == entry)
	{
		return entry;
	}

	return entry->value;
}

void clear_table(HASH_TABLE* ht, size_t size)
{

	if(size == -1)
	{
		size = INITIAL_BUCKET_SIZE;
	}
	if(NULL == ht)
	{
		return;
	}

	for(int i = 0; i < ht->size; i++)
	{
		free(ht->hash_table[i].value);
		free(ht->hash_table[i].key);
	}

	free(ht->hash_table);
	HASH_ENTRY* new_table = malloc(sizeof(HASH_ENTRY) * size);
	if(NULL == new_table)
	{
		printf("Out of memory");
		exit(1);
	}

	ht->hash_table = new_table;

}

// For convenience
void delete_table(HASH_TABLE* ht)
{
	if(NULL == ht)
	{
		return;
	}

	for(int i = 0; i < ht->size; i++)
	{
		free(ht->hash_table[i].value);
		free(ht->hash_table[i].key);
	}

	free(ht->hash_table);
	free(ht);
}

