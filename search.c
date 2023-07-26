#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "stack.h"
#include "maze.h"


int search_maze(node_t *head, node_t *start_node, node_t *end_node, stack_t *stack, stack_t *paths, int *n)
/* Przeszukuje labirynt przechodzac przez kolejne wezly i odkladajac je na stos.
 * Gdy trafimy na wezel koncowy zapisujemy stos i cofamy sie szukac kolejnych sciezek.
 * Podczas cofania zdejmujemy osttani wezel ze stosu. */
{
	int i;
	
	stack_add(stack, head);

	/* Jesli to wezel koncowy to zapisz sciezke na inny stos */
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
	
	/* Przejscie na kolejne wezly */
	for (i = 0; i < head->next_c; i++)
	{	
		/* Jesli wezel do ktorego chcemy przejsc jest juz na stosie, oznacza to ze zostal juz przez nas odwiedzony */
		if (is_on_stack(stack, head->next[i]) == 0)
		{
			search_maze(head->next[i], start_node, end_node, stack, paths, n);
		}
	}
	
	stack_pop(stack);
	return 0;
}
