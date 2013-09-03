#ifndef _CHUNCKED_LIST_H
#define _CHUNKED_LIST_H

#include <stdlib.h>
#include <container/iterator.h>

struct chunk
	{
	unsigned int len;
	struct chunk *next;
	struct chunk *prev;
	void *data;
	};

typedef struct chunked_list
	{
	size_t elem_size;
	size_t chunk_size;
	struct chunk *head;
	} chunked_list_t;

/**
 * allocate a new chunked list
 */
chunked_list_t *chunked_list_create(size_t chunk_size, size_t elem_size);

/**
 * add element to the list, it will be copied in the list.
 * @return 0 if element is added, 1 if failed to allocate a new chunk, 2 if list or e == NULL.
 */
int chunked_list_add(chunked_list_t *list, void *e);

/**
 * retrun the nth element or NULL if i > list size or list == NULL.
 * /!\ you got access directly on the list do no free returned element!
 */
void *chunked_list_get(chunked_list_t *list, unsigned int i);

/**
 * remove and return element from the list.
 * return NULL if list == NULL or i > list_size
 * /!\ you should free the returned element.
 */
void *chunked_list_remove_return(chunked_list_t *list, unsigned int i);

/** return 0 on success */
int chunked_list_remove(chunked_list_t *list, unsigned int i);

/**
 * free the list and all element returned by chunked_list_get
 */
void chunked_list_destroy(chunked_list_t *list);

/**
 * get iterator on chunked list.
 * /!\ removing element in the current chunk before the next element will make you missing it.
 */
iterator_t *chunked_list_iterator(chunked_list_t *list);

#endif
