#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

typedef struct arraylist
	{
	size_t elem_size;
	size_t alloc_size;
	void *data;
	int len;
	float grow_factor;
	float sink_factor;
	float sink_threshold;
	} arraylist_t;

/**
 * allocate a new array list
 * @parm initial_cap : number of element before needing realloc
 * @param elem_size : size of an element in this list
 * @param grow_factor : by how much the arraylist will grow on realloc in % of allocated space (range ]0., inf])
 * @param sink_threshold : if len/alloc_size < alloc_size*sink_threshold then sink list, if 0 then never sink if >=1 always sink
 * @param sink_factor : by how much % sink the list
 * @return the list or NULL on error (can't alloc list or invalid param)	
 */
arraylist_t *arraylist_create(unsigned int initial_cap, size_t elem_size, float grow_factor, float sink_threshold, float sink_factor);

/**
 * add element to the list, it will be copied in the list.
 * @return 0 if element is added, 1 if failed to grow, 2 if list == NULL or e == NULL.
 */
int arraylist_add(arraylist_t *list, void *e);
/**
 * retrun the nth element or NULL if list == NULL or i> list_size.
 * /!\ you got access directly on the list do no free returned element!
 */
void *arraylist_get(arraylist_t *list, unsigned int i);
/**
 * remove and return element from the list.
 * return NULL if list == NULL or i> list_size
 * /!\ you should free the returned element.
 */
void *arraylist_remove(arraylist_t *list, unsigned int i);

/**
 * free the list and all element returned by arraylist_get
 */
void arraylist_destroy(arraylist_t *list);

#endif
