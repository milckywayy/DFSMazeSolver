#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "node.h"
#include "stack.h"
#include "maze.h"
#include "search.h"

double get_random_value()
/* Returns a number in the range 0.01 - 10.00 */
{
    return ((rand() % 100) + 1) / 10.0;
}

double count_path_value(stack_t *stack)
/* Calculates the length of a given path */
{
    int i;
    double sum = 0;

    for (i = 0; i < stack->n - 1; i++)
    {
        sum += ((stack->nodes[i])->value + (stack->nodes[i + 1])->value) / 2;
    }

    return sum;
}

void help()
{
    printf("\n");
    printf("main.exe <height> <width> <start x> <end x>\n");
    printf("\theight - height of the maze. Range <2; inf>\n");
    printf("\twidth - width of the maze. Range <2; inf>\n");
    printf("\tstart x - range <1; width>\n");
    printf("\tstart y - range <1; width>\n");
}

int main(int argc, char **argv)
{
    int i, j;

    int paths_c = 0;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0)
        {
            help();
            return 0;
        }
    }

    if (argc < 5)
    {
        fprintf(stderr, "%s: Not enough arguments.\n", argv[0]);
        help();
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    int start_x = atoi(argv[3]);
    int end_x = atoi(argv[4]);

    if (rows < 2 || cols < 2)
    {
        fprintf(stderr, "%s: x/y out of range.\n", argv[0]);
        help();
        return 1;
    }

    if (start_x > cols || end_x > cols || start_x < 1 || end_x < 1)
    {
        fprintf(stderr, "%s: Start/End x out of range.\n", argv[0]);
        help();
        return 1;
    }

    start_x--;
    end_x--;

    srand(time(NULL));

    stack_t *paths = malloc(sizeof(stack_t) * 20);
    for (i = 0; i < 10; i++)
    {
        init_stack(&(paths[i]));
    }

    /* Stack storing the order of "digging" the maze */
    stack_t stack;
    init_stack(&stack);

    /* 2D array storing nodes */
    node_t **nodes = malloc(sizeof(node_t *) * rows);
    for (i = 0; i < rows; i++)
    {
        nodes[i] = malloc(sizeof(node_t) * cols);
    }

    /* Initialize nodes */
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            node_init(&(nodes[i][j]), i, j, get_random_value());
        }
    }

    node_t *start_node = &(nodes[0][start_x]);
    node_t *end_node = &(nodes[rows - 1][end_x]);

    /* Add a predecessor for the start node to prevent paths from connecting back to its beginning */
    start_node->prev_c++;

    /* Loop to create the maze.
       The recursive function is called for the start node, "digging" one tunnel and pushing the "dug" nodes onto the stack.
       Then the same function is called for the next nodes on the stack until the maze is completed. */
    generate_maze(nodes, rows, cols, start_node, 2, start_node, end_node, &stack);
    for (i = 0; i < stack.n; i++)
    {
        generate_maze(nodes, rows, cols, stack.nodes[i], 2, start_node, end_node, &stack);
    }

    /* Creates a double connection between nodes if possible */
    make_double_connection(nodes, rows, cols, end_node);

    print_maze(nodes, rows, cols, start_node, end_node);

    /* Searching for paths and pushing them onto the stacks */
    stack_t search_stack;
    init_stack(&search_stack);
    search_maze(start_node, start_node, end_node, &search_stack, paths, &paths_c);
    free_stack(&search_stack);

    double tmp;
    int max_path_i = 0;
    double max_path_v = count_path_value(&(paths[0]));

    printf("\nFound paths:\n");
    for (i = 0; i < paths_c; i++)
    {
        tmp = count_path_value(&(paths[i]));

        if (tmp < max_path_v)
        {
            max_path_i = i;
            max_path_v = tmp;
        }

        printf("S%d: ", i + 1);
        print_stack(&(paths[i]));

        printf(" with weight: %g", tmp);
        printf("\n");
    }

    printf("\nShortest path found:\nS%d: ", max_path_i + 1);
    print_stack(&(paths[max_path_i]));
    printf(" with weight: %g\n", max_path_v);

    for (i = 0; i < 20; i++)
    {
        free_stack(&(paths[i]));
    }
    free(paths);

    for (i = 0; i < rows; i++)
    {
        free(nodes[i]);
    }
    free(nodes);
    free_stack(&stack);

    return 0;
}

