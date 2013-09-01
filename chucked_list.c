#include <string.h>
#include <stdlib.h>

#include "chunked_list.h"


chunked_list_t *chunked_list_create(size_t chunk_size, size_t elem_size)
	{
	chunked_list_t *list;
	if(chunk_size==0 || elem_size==0)
		return NULL;
	list=malloc(sizeof(chunked_list_t)+elem_size*chunk_size);
	if(list==NULL)
		return NULL;

	list->chunk_size=chunk_size;
	list->head.len=0;
	list->head.next=NULL;
	list->last=&list->head;
	list->elem_size=elem_size;
	return list;
	}

int chunked_list_add(chunked_list_t *list, void *e)
	{
	struct chunk *chunk;
	if(list==NULL || e==NULL)
		return 2;
	chunk=&list->head;
	while(chunk->len==list->chunk_size && chunk->next!=NULL)
		chunk=chunk->next;
	if(chunk->len==list->chunk_size)
		{
		struct chunk *c=malloc(sizeof(struct chunk)+list->elem_size*list->chunk_size);
		if(chunk==NULL)
			return 1;
		c->len=0;
		c->next=NULL;
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
	chunk=&list->head;
	while(i>list->chunk_size)
		{
		chunk=chunk->next;
		i-=list->chunk_size;
		}
	return chunk->data+list->elem_size*i;
	}
void *chunked_list_remove(chunked_list_t *list, unsigned int i)
	{
	struct chunk *chunk, *last;
	if(list==NULL)
		return NULL;
	void *e=malloc(list->elem_size);
	if(e==NULL)
		return NULL;
	chunk=&list->head;
	while(i>list->chunk_size)
		{
		last=chunk;
		chunk=chunk->next;
		i-=list->chunk_size;
		}
	memcpy(e, chunk->data+list->elem_size*i, list->elem_size);
	chunk->len--;
	memmove(chunk->data+list->elem_size*i, chunk->data+list->elem_size*(i+1), list->elem_size*(chunk->len-i));
	
	if(chunk->len==0)
		last->next=chunk->next;

	return e;
	}

void chunked_list_destroy(chunked_list_t *list)
	{
	struct chunk *chunk;
	if(list==NULL)
		return;
	chunk=&list->head;
	while(chunk!=NULL)
		{
		struct chunk *c=chunk->next;
		free(chunk);
		chunk=c;
		}
	free(list);
	}
