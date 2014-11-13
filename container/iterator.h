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
#ifndef _ITERATOR_H
#define _ITERATOR_H

/**
 * you can "extend" this struct like this:
 * struct array_it
 *		{
 *		iterator_t it;
 *		int *array;
 *		int off;
 *		int len;
 *		};
 */
typedef struct iterator
	{
	int (*has_next)(struct iterator *self);	// true if next() != NULL
	void *(*next)(struct iterator *self);	// return the next element
	int (*reset)(struct iterator *self);	// reset the iterator to the first element return 0 if ok
	int (*remove)(struct iterator *self);	// remove the last element return by next() return 0 if ok
	void (*dispose)(struct iterator *self);						// dispose of this iterator
	} iterator_t;

// define some utility macro
#define iterator_reset(it)		(it)->reset(it)
#define iterator_has_next(it)	(it)->has_next(it)
#define iterator_next(it)		(it)->next(it)
#define iterator_remove(it)		(it)->remove(it)
#define iterator_dispose(it)	(it)->dispose(it)

#endif
