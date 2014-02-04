/*******************************************************************************
 * This file is part of libcontainer.
 *
 * libcontainer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcontainer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with libcontainer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ******************************************************************************/
#ifndef _CHUNKED_LIST_H
#define _CHUNKED_LIST_H

#include <stdlib.h>
#include <container/iterator.h>

struct chunk
	{
	unsigned int len;
	struct chunk *next;
	struct chunk *prev;
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
