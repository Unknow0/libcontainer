
typedef bloc
	{
	int len;
	void *data;
	} bloc_t;

typedef struct
	{
	int bloc_size;
	int(void*,void*) *cmp;
	node_t *root;
	chunked_list_t *list;
	} btree_t;


btree_t *btree_create(int, int(void*,void*) *cmp);

void btree_add(btree_t *, void*
