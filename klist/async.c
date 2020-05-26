
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"

typedef struct chassis_board_msg {
	int board_id;
	int board_mask;
} chassis_board_msg_t;

typedef struct chassis_board_node {
	struct list_head list;
	chassis_board_msg_t board;
} chassis_board_node_t;

typedef struct chassis_board_head {
	struct list_head head;
	int sending;
	void *curnode;
} chassis_board_head_t;

static chassis_board_head_t board_head;

/**
 *
*/
int chassis_board_kqueue_init(void)
{
	INIT_LIST_HEAD(&board_head.head);
	board_head.sending = 0;
	board_head.curnode = NULL;

	return 0;
}


int add_newboard_to_kqueue(chassis_board_msg_t *bmsg)
{
	chassis_board_node_t *node;

	node = malloc(sizeof(*node));
	if (!node) {
		return -1;
	}

	node->board = *bmsg;
	INIT_LIST_HEAD(&node->list);

	list_add_tail(&node->list, &board_head.head);
	return 0;
}

/**
 *
*/
chassis_board_msg_t *get_newboard_from_kqueue(void)
{
	chassis_board_node_t *board = NULL;

	if (list_empty(&board_head.head)) {
		return NULL;
	}

	board = list_first_entry(&board_head.head, chassis_board_node_t, list);
	if (!board) {
		return NULL;
	}

	board_head.curnode = board;

	return &board->board;
}

int remove_newboard_from_kqueue(int board_id)
{
	if (board_head.curnode) {
		chassis_board_node_t *node = board_head.curnode;
		if (node->board.board_id == board_id) {
			list_del(&node->list);
			free(node);
			board_head.curnode = NULL;
		}
	}

	return 0;
}

int main(int argc, char const *argv[])
{

	chassis_board_kqueue_init();

	chassis_board_msg_t b;

	b.board_id = 10;
	b.board_mask = 24;

	add_newboard_to_kqueue(&b);

	b.board_id = 20;
	b.board_mask = 8;
	add_newboard_to_kqueue(&b);

	chassis_board_msg_t *msg = get_newboard_from_kqueue();
	if (msg) {
		printf("new board: %d/%d\n", msg->board_id, msg->board_mask);
		remove_newboard_from_kqueue(msg->board_id);
	}

	msg = get_newboard_from_kqueue();
	if (msg) {
		printf("new board: %d/%d\n", msg->board_id, msg->board_mask);
		remove_newboard_from_kqueue(msg->board_id);
	}

	msg = get_newboard_from_kqueue();
	if (!msg) {
		printf("no register !!\n");
	}

	return 0;
}
