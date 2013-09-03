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
	} iterator_t;

// define some utility macro
#define iterator_reset(it)		it->reset(it)
#define iterator_has_next(it)	it->has_next(it)
#define iterator_next(it)		it->inext(it)
#define iterator_remove(it)		it->remove(it)

#endif
