#include <stdio.h>
#include <stdlib.h>

struct item {
        int a;
        int b;
        char c[10];
};

struct item items[5] = {
{.a = 1, .b=1, .c ="hello1"},
{.a = 2, .b=1, .c="hello2"},
{.a = 3, .b=1, .c="hello3"},
{.a = 4,.b= 1, .c="hello4"},
{.a = 5, .b = 1,.c= "hello5"}
};


static int comp_str(const void *a, const void *b)
{
        struct item *aitem = (struct item *)a;
        struct item *bitem = (struct item *)b;

        if (aitem->a == bitem->a)
                return 0;

        return -1;
}


int main(void)
{
        struct item a;
        struct item *res = NULL;

        a.a = 3;
        res = (struct item *)bsearch(&a, items, 5, sizeof(struct item), comp_str);
        if (res) {
                printf("res->c = %s \n", res->c);
        } else {
                printf("not find res \n");
        }

        return 0;
}
