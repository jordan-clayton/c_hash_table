test: hash_table.o hash_test.c
	gcc -o build/hash_test build/hash_table.o hash_test.c -lm

hash_table.o:
	if [ `find . -type d -name "build"` -z ]; then \
		mkdir build; \
		fi; \
	gcc -c -o build/hash_table.o hash_table.c -lm

