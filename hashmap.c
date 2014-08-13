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
#include <stdio.h>
#include "container/hashmap.h"

hashmap_t *hashmap_create(unsigned char log2s, float load_factor, size_t (*hash_func)(void*))
	{
	hashmap_t *hash;
	if(hash_func==NULL)
		return NULL;

	size_t initial_cap=1<<log2s;
	hash=malloc(sizeof(hashmap_t));
	hash->map_entries=malloc(sizeof(void*)*initial_cap);
	memset(hash->map_entries, 0, sizeof(void*)*initial_cap);
	hash->map_size=initial_cap;
	hash->hash=hash_func;
	hash->load_factor=load_factor;
	hash->entries_count=0;

	return hash;
	}

#define index(map, e) (map->hash(e)%map->map_size)

int hashmap_grow(hashmap_t *map)
	{
	int i;
	size_t nsize=map->map_size*2;
	void **entries=malloc(sizeof(void*)*nsize);
	if(entries==NULL)
		return 1;
	memset(entries, 0, sizeof(void*)*nsize);
	for(i=0; i<map->map_size; i++)
		{
		if(map->map_entries[i]!=NULL)
			{
			void *e=map->map_entries[i];
			entries[map->hash(e)%nsize]=e;
			}
		}
	free(map->map_entries);
	map->map_entries=entries;
	return 0;
	}

int hashmap_add(hashmap_t *map, void *e)
	{
	size_t i, h;
	void *e2;
	if(map==NULL || e==NULL)
		return 2;
	
	if(map->map_size*map->load_factor<=map->entries_count)
		hashmap_grow(map);

	h=map->hash(e);
	i=h%map->map_size;
	e2=map->map_entries[i];
	while(e2!=NULL && h!=map->hash(e2))
		{
		if(++i>map->map_size)
			i=0;
		e2=map->map_entries[i];
		}
	map->map_entries[i]=e;
	map->entries_count++;
	return 0;
	}

void *hashmap_get(hashmap_t *map, size_t key)
	{
	size_t i;
	void *e;
	if(map==NULL)
		return NULL;
	i=key%map->map_size;
	e=map->map_entries[i];
	while(e!=NULL && map->hash(e)!=key)
		{
		if(++i>map->map_size)
			i=0;
		e=map->map_entries[i];
		}
	return e;
	}

int hashmap_remove(hashmap_t *map, size_t key)
	{
	size_t i;
	void *e;
	if(map==NULL)
		return 1;
	i=key%map->map_size;
	e=map->map_entries[i];
	while(e!=NULL && map->hash(e)!=key)
		{
		if(++i>map->map_size)
			i=0;
		e=map->map_entries[i];
		}
	map->map_entries[i]=NULL;
	map->entries_count--;
	return 0;
	}

void hashmap_destroy(hashmap_t *map)
	{
	int i;
	if(map==NULL)
		return;
	for(i=0; i<map->map_size; i++)
		free(map->map_entries[i]);
	free(map->map_entries);
	free(map);
	}

int hashmap_it_has_next(iterator_t *i)
	{
	struct hashmap_it *it=(struct hashmap_it *)i;
	while(it->off<it->map->map_size && it->map->map_entries[it->off]==NULL)
		it->off++;
	return it->off<it->map->map_size && it->map->map_entries[it->off]!=NULL;
	}
void *hashmap_it_next(iterator_t *i)
	{
	struct hashmap_it *it=(struct hashmap_it *)i;
	return hashmap_it_has_next(i)?it->map->map_entries[it->off++]:NULL;
	}
int hashmap_it_reset(iterator_t *i)
	{
	struct hashmap_it *it=(struct hashmap_it *)i;
	it->off=0;
	return 0;
	}
int hashmap_it_remove(iterator_t *i)
	{
	struct hashmap_it *it=(struct hashmap_it *)i;
	free(it->map->map_entries[it->off]);
	it->map->map_entries[it->off]=NULL;
	return 0;
	}

iterator_t *hashmap_iterator(hashmap_t *map)
	{
	struct hashmap_it *it=malloc(sizeof(struct hashmap_it));

	it->it.has_next=hashmap_it_has_next;
	it->it.next=hashmap_it_next;
	it->it.reset=hashmap_it_reset;
	it->it.remove=hashmap_it_remove;
	it->map=map;
	it->off=0;
	}
