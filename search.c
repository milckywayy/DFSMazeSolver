#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "stack.h"
#include "maze.h"


int search_maze(node_t *head, node_t *start_node, node_t *end_node, stack_t *stack, stack_t *paths, int *n)
/* Searches the maze by traversing through consecutive nodes and putting them on the stack.
 * When it reaches the end node, it saves the stack and backtracks to find other paths.
 * During backtracking, the last node is removed from the stack. */
{
	int i;
	
	stack_add(stack, head);

	/* If it's the end node, save the path to a different stack */
	if (is_node(head, end_node) == 1)
	{
		for (i = 0; i < stack->n; i++)
		{
			stack_add(&(paths[*n]), (stack->nodes)[i]);
		}

		(*n)++;

		stack_pop(stack);
		
		return 1;
	}
	
	/* Move to the next nodes */
	for (i = 0; i < head->next_c; i++)
	{	
		/* If the node we want to move to is already on the stack, it means we have already visited it */
		if (is_on_stack(stack, head->next[i]) == 0)
		{
			search_maze(head->next[i], start_node, end_node, stack, paths, n);
		}
	}
	
	stack_pop(stack);
	return 0;
}

