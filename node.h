typedef struct node
{
        /* Node position */
        int x;
        int y;

        /* Weight */
        double value;

        /* Number of nodes connected to this one */
        int prev_c;

        /* Nodes connected to this one and on which side */
        int next_c;
        struct node *next[4];
} node_t;


void node_init(node_t *node, int x, int y, double value);

