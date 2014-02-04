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
#ifndef _CONTAINER_STRING_H
#define _CONTAINER_STRING_H

#include <stdlib.h>

typedef struct
	{
	const char *str;

	unsigned int ref_count;
	} string_t;

size_t string_hash(void *str);

/** create a new string_t 
 * return NULL on error
 */
string_t *string_create(const char *str);

/** get or create a unique string_t */
string_t *string_create_unique(const char *str);

/** destroy a string_t */
void string_destroy(string_t *str);

#endif
