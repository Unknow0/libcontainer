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
#include "container/hashmap.h"
#include "container/string.h"

hashmap_t *string_unique=NULL;

void string_init()
	{
	if(string_unique==NULL)
		string_unique=hashmap_create(6, 0.66, &string_hash);
	}

size_t string_hash(void *e)
	{
	size_t h=0;
	char *c=(char *)e;
	while(*c!=0)
		h=(h<<2)+*c++;
	return h;
	}

string_t *string_create(const char *str)
	{
	if(str==NULL)
		return NULL;
	string_t *s=malloc(sizeof(string_t));
	if(s==NULL)
		return NULL;

	s->str=strdup(str);
	s->ref_count=1;
	return s;
	}

string_t *string_create_unique(const char *str)
	{
	if(str==NULL)
		return NULL;
	size_t hash=string_hash((void*)str);
	string_t *s=hashmap_get(string_unique, hash);
	if(s==NULL)
		{
		s=string_create(str);
		hashmap_add(string_unique, s);
		}
	return s;
	}

void string_destroy(string_t *s)
	{
	if(--s->ref_count==0)
		{
		free((char*)s->str);
		free(s);
		}
	}
