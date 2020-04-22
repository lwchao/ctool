#include <stdio.h>

#define HZ (1000)
#define TEST_MAX_TYPE 1
#define TEST_TIMER    (10 * HZ)
#define TEST_HASH_SIZE 1024

struct test_data_key {
	int id;
	int org;
};

struct test_data {
	struct test_data_key key;
	int id;
	char name[64];
	struct hash_lru hl;
};

struct hash_lru_list *test_list;

static int test_equal_to_key(struct hash_lru_list *list, void *_obj, void *key_obj)
{
	struct test_data_key *key = key_obj;
	struct test_data *obj = _obj;

	return !memcpy(&obj->key, key, sizeof(*key));
}

static int test_equal_to_obj(struct hash_lru_list *list, void *_obj, void *key_obj)
{
	struct test_data *obj = _obj;
	struct test_data *_key_obj = key_obj;

	return test_equal_to_key(list, obj, &(_key_obj->key));
}

static unsigned test_hash_index_by_key(struct hash_lru_list *list, void *_key)
{
	struct test_data_key *key = _key;
	return hash_generic(key, sizeof(*key), 0);
}

static unsigned test_hash_index_by_obj(struct hash_lru_list *list, void *_key)
{
	struct test_data *obj = _key;
	return test_hash_index_by_key(list, &(obj->key));
}

static enum hl_select
test_select_obj(struct hash_lru_list *list, void *old_obj, void *new_obj)
{
	return HL_SELECT_NEW;
}

static void test_free_obj(struct hash_lru_list *list, void *_obj)
{
	// free 
}

static int test_lru_index(struct hash_lru_list *list, void *_obj)
{
	return 0;
}

static unsigned test_timeout[TEST_MAX_TYPE] = {
	[0] = 10 * HZ,
};

struct hash_lru_ops test_ops = {
	.equal_to_key = test_equal_to_key,
	.equal_to_obj = test_equal_to_obj,
	.hash_index_by_key = test_hash_index_by_key,
	.hash_index_by_obj = test_hash_index_by_obj,
	.select_obj = test_select_obj,
	.free_obj = test_free_obj,
	.lru_index = test_lru_index,
	.default_timeout = test_timeout,
};

int test_hash_init(void)
{
	test_list = hash_lru_list_init(
		offsetof(struct test_data, hl),
		TEST_HASH_SIZE,
		TEST_MAX_TYPE,
		TEST_TIMER,
		NULL,
		&test_ops
	);

	return 0;
}


//
int test_hash_inf(void)
{
	struct test_data test;
	struct test_data *odata, *ndata;

	odata = hash_lru_search(test_list, &test->key);
	if (odata) {
		//
		return 0;
	}

	ndata = malloc(sizeof(struct test_data));
	hash_lru_init(test_list, ndata);

	odata = hash_lru_add(test_list, ndata);
	if (!odata)
		return -1;

	hash_lru_put(test_list, ndata);
	return 0;
}

int test_apply(struct hash_lru_list *list, void *obj, void *arg)
{
	struct test_data *data;

	data = (struct test_data *)obj;
	
	// hash_lru_del(list, data);
	return 0;
}

int test_walk(void)
{
	hash_lru_walk_all(test_list, test_apply, NULL);
}

int main(void)
{
	test_hash_init();
	return 0;
}
