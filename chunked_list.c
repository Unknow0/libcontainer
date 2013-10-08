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

#include "container/chunked_list.h"

struct chunked_list_it
	{
	iterator_t it;
	chunked_list_t *list;
	struct chunk *chunk;
	unsigned int off;
	};

chunked_list_t *chunked_list_create(size_t chunk_size, size_t elem_size)
	{
	chunked_list_t *list;
	if(chunk_size==0 || elem_size==0)
		return NULL;
	list=malloc(sizeof(chunked_list_t)+elem_size*chunk_size);
	if(list==NULL)
		return NULL;

	list->chunk_size=chunk_size;
	list->elem_size=elem_size;
	list->head=NULL;
	return list;
	}

int chunked_list_add(chunked_list_t *list, void *e)
	{
	struct chunk *chunk;
	if(list==NULL || e==NULL)
		return 2;
	chunk=list->head;
	while(chunk!=NULL && chunk->len==list->chunk_size && chunk->next!=NULL)
		chunk=chunk->next;
	if(chunk==NULL || chunk->len==list->chunk_size)
		{
		struct chunk *c=malloc(sizeof(struct chunk)+list->elem_size*list->chunk_size);
		if(chunk==NULL)
			return 1;
		c->len=0;
		c->next=NULL;
		if(chunk==NULL)
			list->head=c;
		else
			chunk->next=c;
		}

	memcpy(&chunk->data+list->elem_size*chunk->len++, e, list->elem_size);
	return 0;
	}

void *chunked_list_get(chunked_list_t *list, unsigned int i)
	{
	struct chunk *chunk;
	if(list==NULL)
		return NULL;
	chunk=list->head;
	while(chunk!=NULL && i>list->chunk_size)
		{
		chunk=chunk->next;
		i-=list->chunk_size;
		}
	return chunk==NULL?NULL:chunk->data+list->elem_size*i;
	}
int chunked_list_remove(chunked_list_t *list, unsigned int i)
	{
	struct chunk *chunk, *last;
	if(list==NULL)
		return 1;
	chunk=list->head;
	while(chunk!=NULL && i>list->chunk_size)
		{
		last=chunk;
		chunk=chunk->next;
		i-=list->chunk_size;
		}
	if(chunk==NULL)
		return 2;
	chunk->len--;
	memmove(chunk->data+list->elem_size*i, chunk->data+list->elem_size*(i+1), list->elem_size*(chunk->len-i));
	
	if(chunk->len==0)
		{
		last->next=chunk->next;
		chunk->next->prev=last;
		free(chunk);
		}

	return 0;
	}
void *chunked_list_remove_return(chunked_list_t *list, unsigned int i)
	{
	struct chunk *chunk, *last;
	if(list==NULL)
		return NULL;
	void *e=malloc(list->elem_size);
	if(e==NULL)
		return NULL;
	chunk=list->head;
	while(chunk!=NULL && i>list->chunk_size)
		{
		last=chunk;
		chunk=chunk->next;
		i-=list->chunk_size;
		}
	if(chunk==NULL)
		return NULL;
	memcpy(e, chunk->data+list->elem_size*i, list->elem_size);
	chunk->len--;
	memmove(chunk->data+list->elem_size*i, chunk->data+list->elem_size*(i+1), list->elem_size*(chunk->len-i));
	
	if(chunk->len==0)
		{
		last->next=chunk->next;
		chunk->next->prev=last;
		free(chunk);
		}

	return e;
	}

void chunked_list_destroy(chunked_list_t *list)
	{
	struct chunk *chunk;
	if(list==NULL)
		return;
	chunk=list->head;
	while(chunk!=NULL)
		{
		struct chunk *c=chunk->next;
		free(chunk);
		chunk=c;
		}
	free(list);
	}
int chunked_list_it_reset(iterator_t *i)
	{
	struct chunked_list_it *it=(struct chunked_list_it *)i;
	it->chunk=it->list->head;
	it->off=0;
	return 0;
	}
int chunked_list_it_has_next(iterator_t *i)
	{
	struct chunked_list_it *it=(struct chunked_list_it *)i;
	return it!=NULL && it->chunk!=NULL && it->list!=NULL && (it->chunk->next!=NULL || it->chunk->len<it->off);
	}
void *chunked_list_it_next(iterator_t *i)
	{
	struct chunked_list_it *it=(struct chunked_list_it *)i;
	if(it==NULL || it->list==NULL || it->chunk==NULL || (it->chunk->next==NULL && it->chunk->len>=it->off))
		return NULL;
	void *e=it->chunk->data+it->list->elem_size*it->off++;
	if(it->off>=it->chunk->len)
		it->chunk=it->chunk->next;
	return e;
	}
int chunked_list_it_remove(iterator_t *i)
	{
	struct chunked_list_it *it=(struct chunked_list_it *)i;
	size_t elem_size;
	if(it==NULL)
		return 1;
	it->chunk->len--;
	it->off--;
	elem_size=it->list->elem_size;
	memmove(it->chunk->data+elem_size*it->off, it->chunk->data+elem_size*(it->off+1), elem_size*(it->chunk->len-it->off));
	
	if(it->chunk->len==0)
		{
		struct chunk *prev=it->chunk->prev;
		if(prev==NULL)
			it->list->head=it->chunk->next;
		else
			prev->next=it->chunk->next;
		it->chunk->next->prev=prev;
		free(it->chunk);
		it->chunk=prev->next;
		it->off=0;
		}
	return 0;
	}

iterator_t *chunked_list_iterator(chunked_list_t *list)
	{
	struct chunked_list_it *it=malloc(sizeof(struct chunked_list_it));
	if(it==NULL)
		return NULL;
	it->it.has_next=&chunked_list_it_has_next;
	it->it.next=&chunked_list_it_next;
	it->it.reset=&chunked_list_it_reset;
	it->it.remove=&chunked_list_it_remove;
	it->list=list;
	it->chunk=list->head;
	it->off=0;
	}
