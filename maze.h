int get_rand(int);

char get_node_char(int);

int is_node(node_t *, node_t *);

int is_end_path(node_t *, node_t *);

int have_connection(node_t *, node_t *);

int make_double_connection(node_t **, int, int, node_t *);

void generate_maze(node_t **, int, int, node_t *, int, node_t *, node_t *, stack_t *);

void print_maze(node_t **, int, int, node_t *, node_t *);

node_t get_next_node(node_t **, node_t *, int, node_t *, node_t *);

