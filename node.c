#include <stdlib.h>
#include "node.h"


void node_init(node_t *node, int x, int y, double value)
/* Inicjalizacja wezla */
{
	node->x = x;
	node->y = y;

	node->next_c = 0;
	node->prev_c = 0;

	node->value = value;

	for (int i = 0; i < 4; i++)
		node->next[i] = NULL;
}
