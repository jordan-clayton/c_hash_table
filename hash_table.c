#include <stdlib.h>
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
	int occupied;
} hash_entry_t;

typedef struct hash_table{
	hash_entry_t* hash_table;
	size_t num_entries;
	size_t size;
	int (*equal)(const void*, const void*);
	unsigned long (*hash)(const void*, size_t);
	unsigned long (*co_hash)(const void*, size_t);
} hash_table_t;


// TODO: modify probe -> get should search for equality
// -> add should search for NULL/UNOCCUPIED

static unsigned long probe(hash_table_t* ht, void* key, size_t key_size)
{
	unsigned long i;
	unsigned long first_hash;
	unsigned long j;

	if(NULL != ht ->hash)
	{
		i = ht->hash(key, key_size);
	}
	else
	{
		i = djb2(key, key_size); 
	}

	first_hash = i;

	if(NULL != ht -> co_hash)
	{
		j = ht->co_hash(key, key_size);	
	}
	else
	{
		j = djb2(key, key_size);
	}

	hash_entry_t* entries = ht->hash_table;

	while(NULL != (entries + i) || !(ht ->equal(key, (entries + i) ->key)))
	{
		
		i = (i + j) % ht->size;
		if(i == first_hash)
		{
			return -1;
		}
	}
	

	return i;
}


static int add_entry(hash_table_t* ht, void* key, size_t key_size, void* value)
{
	
	ht ->num_entries++;
	return 0;
}

static int remove_entry(hash_table_t* ht, void* key)
{

	ht ->num_entries--;
	return 0;
}

static hash_entry_t* get_entry(hash_table_t* ht, void* key)
{

	return NULL;	
}

static void rehash(hash_table_t* old, hash_table_t* new_ht)
{
	for(int i = 0; i < old->size; i++)
	{
		hash_entry_t entry = *(old->hash_table + i);
		add_entry(new_ht, entry.key, entry.key_size, entry.value);
	}
};


static hash_table_t* increase_size(hash_table_t* ht)
{
	hash_entry_t* new_table;
	size_t new_size = pow(2, (log2(ht->size)) + 1); 
	new_table = malloc(sizeof(hash_entry_t) * new_size);

	if(NULL == new_table)
	{
		printf("Out of memory");
		exit(1);
	}

	hash_table_t* new_ht = malloc(sizeof(hash_table_t));

	if(NULL == new_ht)
	{
		printf("Out of memory");
		exit(1);
	}

	new_ht -> equal = ht -> equal;
	new_ht -> hash = ht -> hash;
	new_ht ->size = new_size;
	new_ht ->hash_table = new_table;
	new_ht ->num_entries = 0;


	rehash(ht, new_ht);	
	free(ht);

	return new_ht;
}


static hash_table_t* decrease_size(hash_table_t* ht)
{
	hash_entry_t* new_table;
	size_t new_size = pow(2, (log2(ht->size)) - 1); 
	new_table = malloc(sizeof(hash_entry_t) * new_size);

	if(NULL == new_table)
	{
		printf("Out of memory");
		exit(1);
	}

	hash_table_t* new_ht = malloc(sizeof(hash_table_t));

	if(NULL == new_ht)
	{
		printf("Out of memory");
		exit(1);
	}

	new_ht -> equal = ht -> equal;
	new_ht -> hash = ht -> hash;
	new_ht ->size = new_size;
	new_ht ->hash_table = new_table;
	new_ht ->num_entries = 0;


	rehash(ht, new_ht);	
	free(ht);

	return new_ht;
}


hash_table_t* create_ht(size_t size, int(*equal)(const void*, const void*), unsigned long (*hash)(const void*, size_t), unsigned long (*co_hash)(const void*, size_t))
{
	hash_table_t* new_ht = malloc(sizeof(hash_table_t));
	if(NULL == new_ht)
	{
		printf("Out of memory");
		exit(1);
	}

	new_ht ->hash_table = malloc(sizeof(hash_entry_t) * size);
	
	if(NULL == new_ht ->hash_table)
	{
		printf("Out of memory");
		exit(1);
	}

	new_ht ->size = size;
	new_ht ->equal = equal;
	new_ht ->hash = hash;
	new_ht ->num_entries = 0;

	return new_ht;
}

// This is the default hashing function, implementing modified djb2
unsigned long djb2 (void* key, size_t size)
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



int ht_add(hash_table_t* ht, void* key, size_t key_size, void* value)
{
	if(((float) ht->num_entries * 100 / ht->size) > INC_THRESHOLD)
	{
		ht = increase_size(ht);	
	}

	return 0;
}


int ht_remove(hash_table_t* ht, void* key)
{
	remove_entry(ht, key);
	if(((float) ht -> num_entries * 100 / ht->size) < DEC_THRESHOLD)
	{
		ht = decrease_size(ht);
	}	
	return 0;
}

void* ht_get(hash_table_t* ht, void* key)
{
	return NULL;
}


