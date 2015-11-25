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
#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

#include <utils/iterator.h>

typedef struct arraylist
	{
	size_t elem_size;
	size_t alloc_size;
	void *data;
	size_t size;
	float grow_factor;
	float sink_factor;
	float sink_threshold;
	void (*destructor)(void *);
	} arraylist_t;

struct arraylist_it
	{
	iterator_t it;
	arraylist_t *list;
	size_t off;
	};

/**
 * allocate a new array list
 * @parm initial_cap : number of element before needing realloc
 * @param elem_size : size of an element in this list
 * @param grow_factor : by how much the arraylist will grow on realloc in % of allocated space (range ]0., inf])
 * @param sink_threshold : if len/alloc_size < alloc_size*sink_threshold then sink list, if 0 then never sink if >=1 always sink
 * @param sink_factor : by how much % sink the list
 * @return the list or NULL on error (can't alloc list or invalid param)	
 */
arraylist_t *arraylist_create(unsigned int initial_cap, size_t elem_size, float grow_factor, float sink_threshold, float sink_factor, void (*destructor)(void*));

/**
 * add element to the list, it will be copied in the list.
 * @return 0 if element is added, 1 if failed to grow, 2 if list == NULL or e == NULL.
 */
int arraylist_add(arraylist_t *list, void *e);

/**
 * return the nth element or NULL if list == NULL or i> list_size.
 * /!\ you got access directly on the list do no free returned element!
 */
void *arraylist_get(arraylist_t *list, unsigned int i);

/**
 * remove and return element from the list.
 * return NULL if list == NULL or i> list_size
 * /!\ you should free the returned element.
 */
void *arraylist_remove_return(arraylist_t *list, unsigned int i);

/** return 0 on success */
int arraylist_remove(arraylist_t *list, unsigned int i);

/**
 * free the list and all element returned by arraylist_get
 */
void arraylist_destroy(arraylist_t *list);

/**
 * return an iterator on a array_list.
 * /!\ if you remove an element before the current element iterator will miss the next element.
 */
iterator_t *arraylist_iterator(arraylist_t *list);

#endif
