#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "stack.h"


/* Node connections are determined as follows */
/* NULL - no passage or not checked yet */
/* 0 - up */
/* 1 - right */
/* 2 - down */
/* 3 - left */


int get_rand(int n)
/* Returns a random number in the range <0;n-1> */
{
	return rand() % n;
}


int is_node(node_t *a, node_t *b)
/* Compares two nodes. 1 - they are the same, 0 - they are different */
{
	if (a->x == b->x && a->y == b->y)
		return 1;
	else
		return 0;
}


int have_connection(node_t *a, node_t *b)
/* Checks if the given nodes have a connection. 1 - they have, 0 - they don't */
{
	int i;

	for (i = 0; i < b->next_c; i++)
	{
		if (a->x == (b->next[i])->x && a->y == (b->next[i])->y)
			return 1;
	}

	for (i = 0; i < a->next_c; i++)
	{
		if (b->x == (a->next[i])->x && b->y == (a->next[i])->y)
			return 1;
	}

	return 0;
}


int is_end_path(node_t *head, node_t *end_node)
/* Checks if there is a possible path to the end of the maze from the given point */
{
	int i;
	int v;

	if (is_node(head, end_node))
	{
		return 1;
	}
	
	for (i = 0; i < head->next_c; i++)
	{
		v = is_end_path(head->next[i], end_node); 
		if (v == 1)
		{
			return 1;
		}
	}

	return 0;
}


node_t *get_next_node(node_t **nodes, node_t *head, int dir, node_t *start_node, node_t *end_node)
/* Checks if a connection can be made. If so, returns the next node. Otherwise, returns NULL */
{
	int x = head->x;
	int y = head->y;

	int nx = x;
	int ny = y;

	node_t *next_node = NULL;

	/* Assuming cases of going out of the maze have been checked outside the function */
	switch (dir)
	{
		case 0: /* Up */
			nx--;
			break;
		case 1: /* Right */
			ny++;
			break;
		case 2: /* Down */
			nx++;
			break;
		case 3: /* Left */
			ny--;
			break;
		default:
			return NULL;
			break;
	}
	
	next_node = &(nodes[nx][ny]);
	
	/* The end node can have multiple paths leading to it */
	if (is_node(next_node, end_node) == 1 && !have_connection(head, next_node))
	{
		if (end_node->prev_c > 0 && get_rand(2) == 0)
			return next_node;
		else if (end_node->prev_c == 0)
			return next_node;
		else
			return NULL;
	}

	/* If the node is not the end node and already has predecessors, it's not valid */
	if (next_node->prev_c > 0)
	{
		return NULL;
	}

	return next_node;
}


int double_connect(node_t *a, node_t *b, node_t *end_node)
/* Creates a double connection between nodes */
{
	if (have_connection(a, b) == 0 && 
	   is_end_path(a, end_node) == 1 && is_end_path(b, end_node) == 1)
	{
		a->next[a->next_c++] = b;
		b->prev_c++;
			
		b->next[b->next_c++] = a;
		a->prev_c++;
	
		return 1;
	}

	return 0;
}


int make_double_connection(node_t **nodes, int rows, int cols, node_t *end_node)
/* Looks for nodes that can be doubly connected. These are nodes that lead to the end of the maze */
{
	int i;
	int j;
	int x;
	int y;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (i > 0)
			{
				x = i - 1;
				y = j;
				if (double_connect(&(nodes[i][j]), &(nodes[x][y]), end_node) == 1)
					return 1;
			}
			if (i < rows - 1)
			{
				x = i + 1;
				y = j;
				if (double_connect(&(nodes[i][j]), &(nodes[x][y]), end_node) == 1)
					return 1;
			}
			if (j > 0)
			{
				x = i;
				y = j - 1;
				if (double_connect(&(nodes[i][j]), &(nodes[x][y]), end_node) == 1)
					return 1;
			}
			if (j < cols - 1)
			{
				x = i;
				y = j + 1;
				if (double_connect(&(nodes[i][j]), &(nodes[x][y]), end_node) == 1)
					return 1;
			}
		}
	}

	return 0;
}


node_t *generate_maze(node_t **nodes, int rows, int cols, node_t *head, int prev, node_t *start_node, node_t *end_node,stack_t*stack)
/* Recursive function to create a branch of the maze */
{	
	int x = head->x;
	int y = head->y;
	
	int dir;
	int checked[4] = {0, 0, 0, 0};
	
	node_t *next_node = NULL;

	/* If it's the end node, we return */
	if (is_node(head, end_node))
	{
		return head;
	}	

	/* Loop until all directions for one node have been checked */
	while (checked[0] == 0 || checked[1] == 0 || checked[2] == 0 || checked[3] == 0) 
	{
		/* Get a new direction <0;3> */
		dir = get_rand(4);
		switch(dir)
		{
			case 0:
				/* Up */
				if (x > 0 && prev != 2 && checked[dir] == 0) /* Make sure it doesn't go out of the maze */
				{
					/* If the new node doesn't meet the requirements, don't create a connection */
					if ((next_node = get_next_node(nodes, head, dir, start_node, end_node)) != NULL)
					{
						/* Increment the predecessor counter to prevent connecting back to this node */
						next_node->prev_c++;

						stack_add(stack, head);

						/* Call the function recursively and when we get a pointer to the next node,
						   save it as the successor of the current node */
						head->next[head->next_c++] = generate_maze(nodes, rows, cols, next_node,dir,start_node,end_node, stack);
						return head;
					}
				}
				checked[dir] = 1;
				break;
			case 1:
				/* Right */
				if (y < cols - 1 && prev != 3 && checked[dir] == 0)
				{
					if ((next_node = get_next_node(nodes, head, dir, start_node, end_node)) != NULL)
					{
						next_node->prev_c++;
						
						stack_add(stack, head);
						
						head->next[head->next_c++] = generate_maze(nodes,rows,cols,next_node,dir,start_node,end_node, stack);
						return head;
					}
				}
				checked[dir] = 1;
				break;
			case 2:
				/* Down */
				if (x < rows - 1 && prev != 0 && checked[dir] == 0)
				{
					if ((next_node = get_next_node(nodes, head, dir, start_node, end_node)) != NULL)
					{
						next_node->prev_c++;
						
						stack_add(stack, head);

						head->next[head->next_c++] = generate_maze(nodes,rows,cols,next_node,dir,start_node,end_node, stack);
						return head;
					}
				}
				checked[dir] = 1;
				break;
			case 3:
				/* Left */
				if (y > 0 && prev != 1 && checked[dir] == 0)
				{
					if ((next_node = get_next_node(nodes, head, dir, start_node, end_node)) != NULL)
					{
						next_node->prev_c++;

						stack_add(stack, head);

						head->next[head->next_c++] = generate_maze(nodes,rows,cols,next_node,dir,start_node,end_node, stack);
						return head;
					}
				}
				checked[dir] = 1;
				break;
		}
	}

	return head;
}


char get_node_char(int n)
/* If the number is greater than 9, it is converted to the next letter in the alphabet */
{
	char c;

	if (n < 10)
	{
		c = '0' + n;
		return c;
	}
	c ='0' + n + 7;

	return c;
}


void print_maze(node_t **nodes, int rows, int cols, node_t *start_node, node_t *end_node)
/* Displays the generated maze, start, and end nodes */
{
	int i, j, k;

	printf("\n");
	printf("Start node: %d%d\n", start_node->x, start_node->y);
	printf("End node: %d%d\n", end_node->x, end_node->y);

	for (i = 0; i < rows; i++)
	{
		for (k = 0; k < 3; k++)
		{
			if (k == 0 && i == 0)
			{
				printf("\n|");
				for (j = 0; j < cols; j++)
				{
					if (is_node(&(nodes[i][j]), start_node))
					{
						if (j == cols - 1)
							printf("    ");
						else
							printf("     ");
					}
					else
					{
						if (j == cols - 1)
							printf("----");
						else
							printf("-----");
					}
				}
				printf("|");
			}
			else if (k == 1)
			{
				printf("\n|");
				for (j = 0; j < cols; j++)
				{
					printf(" %c%c ", get_node_char(nodes[i][j].x), get_node_char(nodes[i][j].y));
					//printf("    ");
					if (j < cols - 1 && have_connection(&(nodes[i][j]), &(nodes[i][j + 1])))
						printf(" ");
					else
						printf("|");
				}
			}	
			else if (k == 2)
			{
				printf("\n|");
				for (j = 0; j < cols; j++)
				{
					if (i < rows - 1 && have_connection(&(nodes[i][j]), &(nodes[i + 1][j]))
							|| is_node(&(nodes[i][j]), end_node))
					{
						if (j == cols - 1)
							printf("    ");
						else
							printf("    |");
					}
					else
					{
						if (j == cols - 1)
							printf("----");
						else
							printf("----|");
					}
				}
				printf("|");
			}
		}
	}
	printf("\n\n");

	printf("Node connections:\n");
	for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			if (nodes[i][j].next_c == 0)
				printf("%c%c: none   ", get_node_char(nodes[i][j].x), get_node_char(nodes[i][j].y));

			for (int k = 0; k < ((nodes[i][j]).next_c); k++)
			{
				printf("%c%c -> %c%c   ", get_node_char(i), get_node_char(j),
				get_node_char(((nodes[i][j]).next[k])->x), get_node_char(((nodes[i][j]).next[k])->y));
			}
		}
		printf("\n");
	}

	printf("\nNode weights:\n");
	for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			printf("%c%c = %lf\t", get_node_char(i), get_node_char(j), (nodes[i][j]).value);
		}
		printf("\n");
	}
}

