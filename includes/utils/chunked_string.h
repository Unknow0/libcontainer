/*******************************************************************************
 * This file is part of libutils.
 *
 * libutils is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with libutils; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ******************************************************************************/
#ifndef _CHUNKED_STRING_H
#define _CHUNKED_STRING_H

#include <stdlib.h>
#include <utils/iterator.h>

struct str_chunk
	{
	unsigned int len;
	struct str_chunk *next;
	char data[0];
	};

typedef struct chunked_string
	{
	size_t chunk_size;
	size_t size;
	struct str_chunk *head;
	} chunked_string_t;

/**
 * allocate a new chunked list
 */
chunked_string_t *chunked_string_create(size_t chunk_size);

/**
 * add element to the list, it will be copied in the list.
 * @return 0 if element is added, 1 if failed to allocate a new chunk, 2 if list or e == NULL.
 */
int chunked_string_add(chunked_string_t *list, const char *str);

/**
 * retrun the nth element or NULL if i > list size or list == NULL.
 * /!\ you got access directly on the list do no free returned element!
 */
char *chunked_string_get(chunked_string_t *list, unsigned int i);

/**
 * remove and return element from the list.
 * return NULL if list == NULL or i > list_size
 * /!\ you should free the returned element.
 */
//char *chunked_string_remove_return(chunked_string_t *list, unsigned int i, unsigned int len);

/** return 0 on success */
int chunked_string_remove(chunked_string_t *list, unsigned int i, unsigned int len);

/**
 * free the list and all element returned by chunked_string_get
 */
void chunked_string_destroy(chunked_string_t *list);

/**
 * remove all data from this list.
 * /!\ dont remove unused chunk.
 */
 void chunked_string_clear(chunked_string_t *list);

#endif
