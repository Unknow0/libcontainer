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
#ifndef _LINKNED_LIST_H
#define _LINKNED_LIST_H

#include <stdlib.h>
#include <utils/iterator.h>

typedef struct list_node
	{
	struct list_node *next;
	struct list_node *prev;
	void *data;
	} linked_node_t;

typedef struct
	{
	size_t elem_size;
	size_t size;
	struct list_node *head;
	struct list_node *tail;
	} linked_list_t;

struct linked_list_it
	{
	iterator_t it;
	linked_list_t *list;
	linked_node_t *cur;
	};

linked_list_t *linked_list_create(size_t elem_size);

/**
 * return 0 if added, 1 if list or e is null, 2 if failed to alloc a node
 */
int linked_list_add_head(linked_list_t *list, void *e);
int linked_list_add_tail(linked_list_t *list, void *e);

iterator_t *linked_list_iterator(linked_list_t *list);

#endif
