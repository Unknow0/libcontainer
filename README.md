libcontainer
============

simple c container library

#### arraylist
a growing array

#### chunked_list
a linked array list.
removing element will only move value in the chunk, adding somthing back will take the first free space.

	chuncked_list_t *list=chunked_list_create(3, 1);
	chunkcked_list_add(list, 'a');
	chunkcked_list_add(list, 'b');
	// we got {a, b, ... }
	chunkcked_list_add(list, 'c');
	chunkcked_list_add(list, 'd');
	chunkcked_list_add(list, 'e');
	// we got {a, b, c } -> {d, e, ...}
	
	void *e=chuncked_list_remove(list, 1);
	free(e);
	// we got {a, c, ...} -> {d, e, ...}
	
	chunkcked_list_add(list, 'f');
	// we got {a, c, f} -> {d, e, ...}
	
