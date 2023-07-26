#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "stack.h"


/* Polaczenia wezlow sa okreslane w nastepujacy sposob */
/* NULL - nie ma przjescia lub jeszcze nie sprawdzono */
/* 0 - gora */
/* 1 - prawo */
/* 2 - dol */
/* 3 - lewo */


int get_rand(int n)
/* Zwraca losowa loiczbe z przedzialu <0;n-1> */
{
	return rand() % n;
}


int is_node(node_t *a, node_t *b)
/* Porownuje dwa wezly. 1 - sa takie same, 0 - sa inne */
{
	if (a->x == b->x && a->y == b->y)
		return 1;
	else
		return 0;
}


int have_connection(node_t *a, node_t *b)
/* Sprawdza czy podane wezly maja ze soba polaczenie. 1- maja, 0 - nie maja */
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
/* Sprawdza czy z podanego punktu jest mozliwe przejscie do konca labiryntu */
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
/* Sprawdza czy moze zostac utworzone polaczenie. Jesli tak - zwraca nastepny wezel. Jesli nie - NULL */
{
	int x = head->x;
	int y = head->y;

	int nx = x;
	int ny = y;

	node_t *next_node = NULL;

	/* Zakladamy, ze przypadki "wypadniecia" z labiryntu zostaly sprawdzone poza funckja */
	switch (dir)
	{
		case 0: /* Gora */
			nx--;
			break;
		case 1: /* Prawo */
			ny++;
			break;
		case 2: /* Dol */
			nx++;
			break;
		case 3: /* Lewo */
			ny--;
			break;
		default:
			return NULL;
			break;
	}
	
	next_node = &(nodes[nx][ny]);
	
	/* Wezel koncowy moze miec wiele sciezek do niego prowadzacych */
	if (is_node(next_node, end_node) == 1 && !have_connection(head, next_node))
	{
		if (end_node->prev_c > 0 && get_rand(2) == 0)
			return next_node;
		else if (end_node->prev_c == 0)
			return next_node;
		else
			return NULL;
	}

	/* Jesli wezel nie jest koncowym i ma poprzednikow to odpada */
	if (next_node->prev_c > 0)
	{
		return NULL;
	}

	return next_node;
}


int double_connect(node_t *a, node_t *b, node_t *end_node)
/* Tworzy podwojne polaczenie wezlow */
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
/* Szuka wezlow ktore mozna ze soba podwojnie polaczyc. To takie, ktore prowadza do konca labiryntu */
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
/* Rekurencyjna funkcja tworzaca odnoge labiryntu */
{	
	int x = head->x;
	int y = head->y;
	
	int dir;
	int checked[4] = {0, 0, 0, 0};
	
	node_t *next_node = NULL;

	/* Jesli wezel koncowy to wychodzimy */
	if (is_node(head, end_node))
	{
		return head;
	}	

	/* Petla dopoki nie zostana sprawdzone wszystkie kierunki dla jednego wezla */
	while (checked[0] == 0 || checked[1] == 0 || checked[2] == 0 || checked[3] == 0) 
	{
		/* Pobieramy nowy kierunek <0;3> */
		dir = get_rand(4);
		switch(dir)
		{
			case 0:
				/* Gora */
				if (x > 0 && prev != 2 && checked[dir] == 0) /* Czy nie wychodzi poza labirynt */
				{
					/* Jesli nowy wezel nie spelnia warunkow to nie tworzymy polaczenia */
					if ((next_node = get_next_node(nodes, head, dir, start_node, end_node)) != NULL)
					{
						/* Inkrementujemy licznik poprzednikow by nie polaczyc sie z tym wezlem ponownie */
						next_node->prev_c++;

						stack_add(stack, head);

						/* Wywolujemy funkcje i gdy kiedys dostaniemy wskaznik na nastepny wezel,
						 * zapisujemy go jako nastepce aktualnego */
						head->next[head->next_c++] = generate_maze(nodes, rows, cols, next_node,dir,start_node,end_node, stack);
						return head;
					}
				}
				checked[dir] = 1;
				break;
			case 1:
				/* Prawo */
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
				/* Dol */
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
				/* Lewo */
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
/* Jesli liczba jest wieksza od 9 to zamieniamy ja na kolejne litery alfabetu */
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
/* Wyswietla wygenerowany labirynt, wezly poczatkowe i koncowe */
{
	int i, j, k;

	printf("\n");
	printf("Wezel startowy: %d%d\n", start_node->x, start_node->y);
	printf("Wezel koncowy: %d%d\n", end_node->x, end_node->y);

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

	printf("Przejscia wezlow:\n");
	for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			if (nodes[i][j].next_c == 0)
				printf("%c%c: brak   ", get_node_char(nodes[i][j].x), get_node_char(nodes[i][j].y));

			for (int k = 0; k < ((nodes[i][j]).next_c); k++)
			{
				printf("%c%c -> %c%c   ", get_node_char(i), get_node_char(j),
				get_node_char(((nodes[i][j]).next[k])->x), get_node_char(((nodes[i][j]).next[k])->y));
			}
		}
		printf("\n");
	}

	printf("\nWagi wezlow:\n");
	for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			printf("%c%c = %lf\t", get_node_char(i), get_node_char(j), (nodes[i][j]).value);
		}
		printf("\n");
	}
}
