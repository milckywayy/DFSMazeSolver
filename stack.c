#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "stack.h"


void init_stack(stack_t *stack)
/* Inicjalizacja stosu */
{
	stack->nodes = malloc(sizeof(node_t) * 8);
	stack->n = 0;
	stack->capacity=8;
}


void stack_add_memory(stack_t *stack)
/* Podwaja pamiec stosu jesci jest taka potrzeba */
{
	stack->nodes = realloc(stack->nodes, sizeof(node_t) * stack->capacity * 2);
	stack->capacity *= 2;
}


void stack_add(stack_t *stack, node_t *node)
/* Dodaje element na gore stosu */
{
	if (stack->n + 1 > stack->capacity)
		stack_add_memory(stack);

	stack->nodes[stack->n++] = node;
}


void stack_pop(stack_t *stack)
/* Usuwa element z gory stosu */
{
	stack->n--;
}


int is_on_stack(stack_t *stack, node_t *node)
/* Sprawdza czy podany element znajduje sie na stosie */
{
	int i;

	for (i = 0; i < stack->n; i++)
	{
		if (node->x == stack->nodes[i]->x && node->y == stack->nodes[i]->y)
		{
			return 1;
		}
	}
	return 0;
}


void print_stack(stack_t *stack)
/* Wyswietla zawartosc stosu */
{
	int i;

	for (i = 0; i < stack->n; i++)
	{
		if (i == stack->n - 1)
			printf("%d%d", (stack->nodes[i])->x, (stack->nodes[i])->y);
		else
			printf("%d%d, ", (stack->nodes[i])->x, (stack->nodes[i])->y);
	}
}

void free_stack(stack_t *stack)
/* Zwalnia pamiec stosu */
{
	free(stack->nodes);
}
