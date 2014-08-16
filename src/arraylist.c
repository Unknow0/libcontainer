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
#include <string.h>
#include <stdlib.h>

#include "container/arraylist.h"

arraylist_t *arraylist_create(unsigned int initial_cap, size_t elem_size, float grow_factor, float sink_threshold, float sink_factor)
	{
	arraylist_t *list;
	if(grow_factor<=0. || sink_threshold<0. || sink_factor<0.)
		return NULL;
	list=malloc(sizeof(arraylist_t));
	if(list==NULL)
		return NULL;
	list->data=malloc(initial_cap*elem_size);
	if(list->data==NULL)
		{
		free(list);
		return NULL;
		}
	list->alloc_size=initial_cap;
	list->size=0;
	list->elem_size=elem_size;
	list->grow_factor=1.+grow_factor;
	list->sink_factor=sink_factor;
	list->sink_threshold=sink_threshold;
	return list;
	}

int arraylist_add(arraylist_t *list, void *e)
	{
	if(list==NULL || e==NULL)
		return 2;
	if(list->size==list->alloc_size)
		{
		size_t new_size=(size_t)(list->alloc_size*list->grow_factor);
		void *ptr=realloc(list->data, new_size*list->elem_size);
		if(ptr==NULL)
			return 1;
		list->data=ptr;
		list->alloc_size=new_size;
		}

	memcpy(arraylist_get(list, list->size++), e, list->elem_size);
	return 0;
	}

void *arraylist_get(arraylist_t *list, unsigned int i)
	{
	if(list==NULL || i>=list->size)
		return NULL;
	return list->data+list->elem_size*i;
	}
void *arraylist_remove_return(arraylist_t *list, unsigned int i)
	{
	void *e;
	if(list==NULL || i>=list->size)
		return NULL;
	e=malloc(list->elem_size);
	if(e==NULL)
		return NULL;
	memcpy(e, arraylist_get(list, i), list->elem_size);
	arraylist_remove(list, i);
	return e;
	}

int arraylist_remove(arraylist_t *list, unsigned int i)
	{
	if(list==NULL || i>=list->size)
		return 1;
	list->size--;
	memmove(arraylist_get(list, i), arraylist_get(list, i+1), list->elem_size*(list->size-i));

	if((float)list->size/(float)list->alloc_size<list->sink_threshold)
		{
		size_t new_size=(size_t)(list->alloc_size*list->sink_factor);
		if(new_size<list->size)
			new_size=list->size;
		list->data=realloc(list->data, new_size*list->elem_size);
		list->alloc_size=new_size;
		}
	return 0;
	}

void arraylist_destroy(arraylist_t *list)
	{
	free(list->data);
	free(list);
	}

int arraylist_iterator_reset(iterator_t *i)
	{
	struct arraylist_it *it=(struct arraylist_it *)i;
	if(it==NULL)
		return 1;
	it->off=0;
	return 0;
	}

int arraylist_iterator_has_next(iterator_t *i)
	{
	struct arraylist_it *it=(struct arraylist_it *)i;
	return it==NULL?0:it->off<it->list->size;
	}
void *arraylist_iterator_next(iterator_t *i)
	{
	struct arraylist_it *it=(struct arraylist_it *)i;
	if(it==NULL)
		return NULL;
	return arraylist_get(it->list, it->off++);
	}
int arraylist_iterator_remove(iterator_t *i)
	{
	struct arraylist_it *it=(struct arraylist_it *)i;
	if(it==NULL)
		return 1;
	return arraylist_remove(it->list, it->off--);
	}

iterator_t *arraylist_iterator(arraylist_t *list)
	{
	struct arraylist_it *it=malloc(sizeof(struct arraylist_it));
	it->it.reset=&arraylist_iterator_reset;
	it->it.has_next=&arraylist_iterator_has_next;
	it->it.next=&arraylist_iterator_next;
	it->it.remove=&arraylist_iterator_remove;
	it->list=list;
	it->off=0;

	return (iterator_t *)it;
	}
