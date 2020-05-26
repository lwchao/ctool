
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include "list.h"
#include "chassisd_hotplug.h"


typedef struct chassis_board {
	struct list_head list;
	struct chassis_msg_new_board board;
} chassis_board_t;

typedef struct register_board_list {
	struct list_head head;
	int sending;
	void *cur;
} rbl;

rbl regboardlist;

int register_board_init(void)
{
	INIT_LIST_HEAD(&regboardlist.head);

	regboardlist.sending = 0;
	regboardlist.cur = NULL;

	return 0;
}


int add_register_board(struct chassis_msg_new_board *new_board)
{
	chassis_board_t *board;

	board = malloc(sizeof(*board));
	if (!board) {
		return -1;
	}

	board->board = *new_board;

	INIT_LIST_HEAD(&board->list);

	list_add(&board->list, &regboardlist.head);

	return 0;
}


chassis_board_t *get_register_board(void)
{
	chassis_board_t *board = NULL;
	struct list_head *head = &regboardlist.head;

	if (!list_empty(head)) {
		board = list_first_entry(head, chassis_board_t, list);
		if (!board)
			return NULL;

		if (regboardlist.cur) {
			chassis_board_t *b = regboardlist.cur;
			list_del(&b->list);
			free(regboardlist.cur);
			regboardlist.cur = NULL;
		}

		regboardlist.cur = board;
	}

	return board;
}


int remove_registered_board(int board_id)
{
	if (!regboardlist.cur)
		return -1;

	chassis_board_t *b = (chassis_board_t *)regboardlist.cur;
	if (b->board.new_board == board_id) {
		list_del(&b->list);
		free(b);
		b = NULL;
	}

	return 0;
}


int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}
