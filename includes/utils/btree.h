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
#ifndef _BTREE_H
#define _BTREE_H

#include <stdlib.h>

typedef struct btree_index
	{
	char leaf;
	unsigned int len;
	struct btree_index **data;
	} btree_index_t;
typedef struct btree_data
	{
	char leaf;
	unsigned int len;
	struct data_bloc *next;
	void *data;
	} btree_data_t;

typedef struct
	{
	size_t bloc_size;
	size_t elem_size;
	int (*cmp)(void *, void*);
	btree_index_t *index;
	btree_data_t *head;
	} btree_t;

btree_t *btree_create(size_t bloc_size, size_t elem_size, int (*cmp)(void*,void*));

int btree_add(btree_t *index, void *e);

#endif
