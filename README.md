# C Hashtable
A hashtable implementation in C

I would not recommend this to be used in production software; this is purely for my own learning and use-cases.
This implementation uses open-addressing and double-hashing to resolve collisions.

# Use
Include hash\_table.h and compile with hash\_table.c, link with math.h as necessary.

## Hashtable creation
create\_ht returns a pointer to a hashtable struct. When called with size < 1, the default bucket sized is used.
Supply NULL to \*equal \*hash \*co-hash to use default functions.

## Hashing
Both \*hash and \*co-hash signitures include a size parameter. Feel free to
ignore these and set size to 0.

The default hashing implementations treat a key as a char\* to allow for any
type to be used as a key (see default functions).

Creation uses malloc for the inner table and the hashtable struct. Free appropriately or call delete\_table to handle
memory.

## Default functions
When using the default functions, sizes for arguments must be provided.
Hashing treats the key as a stream of bytes and is computed accordingly.

default\_equal performs comparison using memcmp
djb2 (see references) is used for the primary hash - this implementation uses XOR instead of addition.

sdbm (see references) is used for secondary hash - this has been modified to always return an odd number.


## Insertion
ht\_add returns 0 on success, -1 on a table size error, -2 if the item is already present in the table.
Keys can be of any type when using the default hashing function (see default functions).
Values must be supplied via pointer.

## Removal
ht\_delete returns 0 on success, -1 if the object is not in the table.

## Access
ht\_get returns a void\* to the value on successful retrieval. Cast as appropriate. On failure, this returns NULL.

## Clearing
clear\_table clears the current table and resizes to the supplied size. When called with size -1, the default bucket size is used.

## Deletion
delete\_table is a convenience function to free a hashtable struct in its entirety. Using a pointer after this function
has been called on it will cause a segmentation fault. 

# References
djb2, sdbm - http://www.cse.yorku.ca/~oz/hash.html

