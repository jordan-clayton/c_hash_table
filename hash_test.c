#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

int main(void)
{
	// Testing default implementation;

	// Insert and retrieve.	
	printf("Insertion test, default implementation: add/get\n");
	HASH_TABLE* hash_table = create_ht(0, NULL, NULL, NULL);
	int v1 = 1;
	ht_add(hash_table, "Test", 4, &v1, sizeof(v1));

	int v2 = *(int*)ht_get(hash_table, "Test", 4);

	printf("v1: %d, v2: %d\n", v1, v2);

	// Removal test.


	// Key test.
	

	// Dynamic key test.


	// Resize test.
	
	// Size increase.


	// Size decrease.

	// Clear test.


	// Delete table
	delete_table(hash_table);
	return 0;

}
