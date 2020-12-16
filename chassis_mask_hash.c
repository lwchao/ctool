/*******************************************************************************
    Copyright (C), 2012-2020, Legendsec Tech. Co., Ltd.
    Filename: test.c
    Author: liuwenchao
    Version: 1.0
    Date: 2020-12-16
    Description:
    Modification:
    History:
        Author: liuwenchao
        Date: 2020-12-16
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#define CHASSIS_MAX_BOARD_ID 16
#define CHASSIS_SPU_MASK 0xffffffe0
#define CHASSIS_MPU_AND_SPU_MASK 0xfffffff8  // 排除接口板

struct position_mapping {
	int count;
	int position[CHASSIS_MAX_BOARD_ID];
};

struct shm_chassis_cfg {
	int physical_id;  /*实际上所处的物理槽位id*/
	int physical_mask;
	int chassis_id;   /*代码中可见的逻辑槽位id, 对于接口板、业务板而言，chassis_id与physical_id相同*/
	int chassis_mask;
	int mpu_mask;
	int master0_phy_id; /*master0的物理id(槽位号小的板物理id)*/
	int master1_phy_id; /*master1的物理id(槽位号大的板物理id)*/
	int self_master;    /*用于标识本板是否为主用主控*/
	int master_physical_id; /*主用主控板的物理id, 业务板往主控发消息时使用此变量，主备切换后，该值被修改*/
	int temp_physical_id;   /*用于临时设置的physical_id，初始化时同physical_id*/
	int board_status; /*本板状态*/

	struct position_mapping pos;  /* 保存当前在线的板和位置的关系 */
};


static struct shm_chassis_cfg *__shm_chassis_cfg = NULL;

static int get_board_id_byhash(int hash)
{
	if (hash >= __shm_chassis_cfg->pos.count)
        return -1;

	return __shm_chassis_cfg->pos.position[hash];
}

static int reset_chassis_position_mapping(int chassis_mask)
{
	int board = 0;
	int i = 0;
	unsigned int mask = chassis_mask;
	struct position_mapping *pos = &__shm_chassis_cfg->pos;

	memset(pos, 0, sizeof(*pos));

	for (board = 1, mask &= CHASSIS_MPU_AND_SPU_MASK; (mask != 0 && board <= CHASSIS_MAX_BOARD_ID); board++, mask = mask >> 1) {
		if (mask & 0x1) {
			pos->position[i++] = board;
			pos->count++;
		}
	}

	return 0;
}

int print_position(void)
{
	struct position_mapping *pos = &__shm_chassis_cfg->pos;
    printf("position count: %d\n", pos->count);

    int i;
    for (i=0; i<pos->count; i++) {
        printf(">> i: %d board: %d\n", i, pos->position[i]);
    }
}


int main(int argc, char const *argv[])
{

    __shm_chassis_cfg = malloc(sizeof(*__shm_chassis_cfg));
    if (!__shm_chassis_cfg)
        return -1;

    int mask = 0x6F;
    reset_chassis_position_mapping(mask);


    print_position();

    printf("get_board_position pos:%d, board:%d\n", 0, get_board_id_byhash(0));
    printf("get_board_position pos:%d, board:%d\n", 1, get_board_id_byhash(1));
    printf("get_board_position pos:%d, board:%d\n", 2, get_board_id_byhash(2));
    printf("get_board_position pos:%d, board:%d\n", 3, get_board_id_byhash(3));

    free(__shm_chassis_cfg);
    return 0;
}

