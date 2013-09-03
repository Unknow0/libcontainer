#ifndef _LINKNED_LIST_H
#define _LINKNED_LIST_H

#include <stdlib.h>
#include <container/iterator.h>

typedef struct list_node
	{
	struct list_node *next;
	struct list_node *prev;
	void *data;
	} linked_node_t;

typedef struct
	{
	size_t elem_size;

	struct list_node *head;
	struct list_node *tail;
	} linked_list_t;

linked_list_t *linked_list_create(size_t elem_size);

/**
 * return 0 if added, 1 if list or e is null, 2 if failed to alloc a node
 */
int linked_list_add_head(linked_list_t *list, void *e);
int linked_list_add_tail(linked_list_t *list, void *e);

iterator_t *linked_list_iterator(linked_list_t *list);

#endif
