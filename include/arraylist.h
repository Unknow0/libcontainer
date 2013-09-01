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


arraylist_t *arraylist_create(int initial_cap, size_t elem_size, float grow_factor, float sink_threshold, float sink_factor);

int arraylist_add(arraylist_t *list, void *e);
void *arraylist_get(arraylist_t *list, int i);
void *arraylist_remove(arraylist_t *list, int i);

void arraylist_destroy(arraylist_t *list);

#endif
