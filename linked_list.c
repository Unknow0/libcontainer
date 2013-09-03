

#include "container/linked_list.h"

struct linked_list_it
	{
	iterator_t it;
	linked_list_t *list;
	linked_node_t *cur;
	};

linked_list_t *linked_list_create(size_t elem_size)
	{
	linked_list_t *list;
	if(elem_size==0)
		return NULL;
	list=malloc(sizeof(linked_list_t));
	if(list==NULL)
		return NULL;
	list->elem_size=elem_size;
	list->head=NULL;
	return list;
	}

int linked_list_add_head(linked_list_t *list, void *e)
	{
	struct list_node *node;
	if(list==NULL || e==NULL)
		return 1;
	node=malloc(sizeof(linked_node_t)+list->elem_size);
	if(node==NULL)
		return 2;
	node->prev=NULL;
	node->next=list->head;
	list->head->prev=node;
	list->head=node;
	return 0;
	}

int linked_list_add_tail(linked_list_t *list, void *e)
	{
	struct list_node *node;
	if(list==NULL || e==NULL)
		return 1;
	node=malloc(sizeof(linked_node_t)+list->elem_size);
	if(node==NULL)
		return 2;
	node->prev=NULL;
	node->next=list->head;
	list->head->prev=node;
	list->head=node;
	return 0;
	}

int linked_list_it_has_next(iterator_t *i)
	{
	struct linked_list_it *it=(struct linked_list_it*)i;
	if(it==NULL || it->cur==NULL)
		return 0;
	return it->cur->next!=NULL;
	}
void *linked_list_it_next(iterator_t *i)
	{
	struct linked_list_it *it=(struct linked_list_it*)i;
	void *e;
	if(it==NULL)
		return NULL;
	if(it->cur)
		it->cur=it->cur->next;
	return it->cur->data;
	}
int linked_list_it_remove(iterator_t *i)
	{
	struct linked_list_it *it=(struct linked_list_it*)i;
	linked_node_t *node;
	if(it==NULL || it->cur==NULL)
		return 1;
	node=it->cur->prev;
	node->next=it->cur->next;
	it->cur->next->prev=node;
	return 0;
	}

int linked_list_it_reset(iterator_t *i)
	{
	struct linked_list_it *it=(struct linked_list_it*)i;
	linked_node_t *node;
	if(it==NULL)
		return 1;
	it->cur=it->list->head;
	return 0;
	}

iterator_t *linked_list_iterator(linked_list_t *list)
	{
	struct linked_list_it *it;
	if(list==NULL)
		return NULL;
	
	it=malloc(sizeof(struct linked_list_it));

	it->it.has_next=&linked_list_it_has_next;
	it->it.next=&linked_list_it_next;
	it->it.reset=&linked_list_it_reset;
	it->it.remove=&linked_list_it_remove;

	it->list=list;
	it->cur=list->head;

	return (iterator_t*)it;
	}
