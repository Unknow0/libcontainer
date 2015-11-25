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
#include <string.h>
#include <stdlib.h>

#include "utils/chunked_string.h"

chunked_string_t *chunked_string_create(size_t chunk_size)
	{
	chunked_string_t *list;
	if(chunk_size==0)
		return NULL;
	list=malloc(sizeof(chunked_string_t));
	if(list==NULL)
		return NULL;

	list->chunk_size=chunk_size;
	list->head=NULL;
	list->size=0;
	return list;
	}

int chunked_string_add(chunked_string_t *list, const char *str)
	{
	struct str_chunk *chunk;
	if(list==NULL || str==NULL)
		return 2;
	chunk=list->head;
	while(chunk!=NULL && chunk->len==list->chunk_size && chunk->next!=NULL)
		chunk=chunk->next;
	if(chunk==NULL || chunk->len==list->chunk_size)
		{
		struct str_chunk *c=malloc(sizeof(struct str_chunk)+list->chunk_size);
		if(c==NULL)
			return 1;
		c->len=0;
		c->next=NULL;
		if(chunk==NULL)
			list->head=c;
		else
			chunk->next=c;
		chunk=c;
		}

	unsigned int len=strlen(str);
	unsigned int l=list->chunk_size-chunk->len;
	if(l>len)
		l=len;
	memmove(chunk->data+chunk->len, str, l);
	chunk->len+=l;
	list->size+=l;
	if(l<len)
		return chunked_string_add(list, str+l);
	return 0;
	}

char *chunked_string_get(chunked_string_t *list, unsigned int i)
	{
	struct str_chunk *chunk;
	if(list==NULL)
		return NULL;
	chunk=list->head;
	while(chunk!=NULL && i>=list->chunk_size)
		{
		chunk=chunk->next;
		i-=list->chunk_size;
		}
	return chunk==NULL?NULL:chunk->data+i;
	}

int chunked_string_remove(chunked_string_t *list, unsigned int i, unsigned int len)
	{
	struct str_chunk *chunk, *last;
	if(list==NULL || len==0)
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
	list->size-=len;
	if(chunk->len<i+len)
		{
		len-=chunk->len-i;
		if(i==0)
			{
			last->next=chunk->next;
			free(chunk);
			chunk=last->next;
			}
		else
			{
			chunk->len=i;
			}
		while(len>chunk->len)
			{
			len-=chunk->len;
			last->next=chunk->next;
			free(chunk);
			chunk=last->next;
			}
		}
	if(len>0)
		{
		chunk->len-=len;
		memmove(chunk->data+i, chunk->data+(i+len), chunk->len-i);
		}

	if(chunk->len==0)
		{
		last->next=chunk->next;
		free(chunk);
		}

	return 0;
	}

void chunked_string_destroy(chunked_string_t *list)
	{
	struct str_chunk *chunk;
	if(list==NULL)
		return;
	chunked_string_clear(list);
	free(list);
	}

 void chunked_string_clear(chunked_string_t *list)
 	{
	struct str_chunk *c=list->head;
	while(c!=NULL)
		{
		struct str_chunk *chunk=c->next;
		free(c);
		c=chunk;
		}
	list->size=0;
	list->head=NULL;
	}
