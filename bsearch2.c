#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _ip_pool_msg {
	int saddr;
	int daddr;
} ip_pool_msg;

static ip_pool_msg ip_pool[1024];

int compare_ip_pool(const void *p1, const void *p2)
{
	ip_pool_msg *ip1 = (ip_pool_msg *)p1;
	ip_pool_msg *ip2 = (ip_pool_msg *)p2;
	
	printf("ip1: %d->%d  ip2: %d->%d\n", ip1->saddr, ip1->daddr, ip2->saddr, ip2->daddr);

	return ip1->saddr - ip2->saddr;
}


void init_ip_pool(void)
{
	int i;
	ip_pool_msg *msg;

	for (i = 0; i < 1024; i++) {
		msg = &ip_pool[i];

		msg->saddr = i;
		msg->daddr = i + 1;
	}
}

int main(void) 
{
	ip_pool_msg key; 
	ip_pool_msg *result = NULL;
	
	init_ip_pool();

	key.saddr = 100;
	key.daddr = 2;

	result = (ip_pool_msg *)bsearch(&key, ip_pool, 1024, sizeof(ip_pool_msg), compare_ip_pool);
	if (!result) {
		printf("cann`t find by key\n");
		return -1;
	}

	printf("result saddr: %d, daddr:%d\n", result->saddr, result->daddr);
	return 0;
}
