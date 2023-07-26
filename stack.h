typedef struct stack{
	node_t **nodes;
	int n;
	int capacity;
} stack_t;


void init_stack(stack_t *);

void stack_add_memory(stack_t *);

void stack_add(stack_t *, node_t *);

void stack_pop(stack_t *);

int is_on_stack(stack_t *, node_t *);

void print_stack(stack_t *);

void free_stack(stack_t *);
