#include <stdio.h>
#include <string.h>

#include "cache.h"



int cache_constructor_fn(void* obj, void* notused1, int notused2)
{
    return 0;
}

void cache_destructor_fn(void* obj, void* notused)
{
    return ;
}

#define GPTR_MAX 10
char *gptr[GPTR_MAX];

int main(void)
{
    int i;
    cache_t *cache = cache_create("testcache", 4 * 1024, 0, cache_constructor_fn, cache_destructor_fn);
    if (!cache)
        return -1;

    for (i=0; i< GPTR_MAX; i++) {
        gptr[i] = cache_alloc(cache);
        printf("alloc gptr: %p\n", gptr[i]);
    }

    for (i=0; i< GPTR_MAX; i++) {
        printf("free gptr: %p\n", gptr[i]);
        cache_free(cache, gptr[i]);
    }

    cache_destroy(cache);
    return 0;
}
