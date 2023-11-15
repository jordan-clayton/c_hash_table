#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash_table.h"

#define ITERATIONS 1000000

void retrieval_speed_test();

int main(void)
{
	// TODO: 
	// Write a string representaton.
	// Testing default implementation;

	// Insert and retrieve.	
	printf("Insertion test, default implementation: add/get\n");
	HASH_TABLE* hash_table = create_ht(0, NULL, NULL, NULL);
	int v1 = 1;
	ht_add(hash_table, "Test", sizeof("Test"), &v1, sizeof(v1));

	int v2 = *(int*)ht_get(hash_table, "Test", sizeof("Test"));

	printf("v1: %d, v2: %d\n", v1, v2);

	delete_table(hash_table);

	// Removal test.
	printf("Deletion test, default implementation: add/get/remove/get\n");
	hash_table = create_ht(0, NULL, NULL, NULL);
	ht_add(hash_table, "Test", sizeof("Test"), &v1, sizeof(v1));
	v2 = *(int*)ht_get(hash_table, "Test", sizeof("Test"));
	printf("v1: %d, v2: %d\n", v1, v2);
	ht_remove(hash_table, "Test", sizeof("Test"));
	void* test = ht_get(hash_table, "Test", sizeof("Test"));
	if(NULL != test)
	{
		v2 = *(int*)test;
		printf("Unsuccessful removal, key: Test, val: %d\n", v2);
	}
	else
	{
		printf("NULL pointer Successfully removed\n");
	}

	delete_table(hash_table);

	// Retrieval speed
	retrieval_speed_test();
	// Dynamic key test.


	// Resize test.
	
	// Size increase.


	// Size decrease.

	// Clear test.


	// Delete table
	return 0;

}

void retrieval_speed_test()
{
	
	int v1 = 1;
	int v2;
	clock_t start;
	clock_t end;
	double acc = 0.0;
	printf("Hashtable 512 buckets:\n");	
	HASH_TABLE* hash_table = create_ht(512, NULL, NULL, NULL);
	ht_add(hash_table, "Test", sizeof("Test"), &v1, sizeof(v1));
	for(int i = 0; i < ITERATIONS; i++)
	{
	 	start = clock();
		v2 = *(int*)ht_get(hash_table, "Test", sizeof("Test"));
		if(v1 != v2)
		{
			printf("Hash retrieval incorrect\n");
			return;
		}
		end = clock();
		//in milliseconds
		double diff = ((double) end - (double) start) * 1000/ CLOCKS_PER_SEC;
		acc += diff;
	}
	
	delete_table(hash_table);
	printf("Average retrieval speed (in milliseconds): %lf\n", acc / ITERATIONS);	

	int numbers[512];
	numbers[511] = 1;
	acc = 0.0;
	printf("Sequential search 512 numbers, end of array:\n");

	for(int i = 0; i < ITERATIONS; i++)
	{
	 	start = clock();
		for(int j = 0; j < 512; j++)
		{
			if(numbers[j] == v1)
			{
				break;
			}
		}
		end = clock();
		//in seconds
		double diff = ((double) end - (double) start)*1000/ CLOCKS_PER_SEC;
		acc += diff;
	}
	printf("Average retrieval speed (in milliseconds): %lf\n", acc/ITERATIONS);	

	char* strings[512];
	for(int i = 0; i < 511; i++)
	{
		strings[i] = "NULL";
	}
	strings[511] = "Test";
	printf("Sequential search, 512 strings, end of array: \n");

	for(int i = 0; i < ITERATIONS; i++)
	{
	 	start = clock();
		for(int j = 0; j < 512; j++)
		{
			if(strncmp("Test", strings[j], 4) == 0)
			{
				break;
			}
		}
		end = clock();
		//in milliseconds
		double diff = ((double) end - (double) start)*1000/ CLOCKS_PER_SEC;
		acc += diff;
	}
	printf("Average retrieval speed (in milliseconds): %lf\n", acc/ITERATIONS);	
}
