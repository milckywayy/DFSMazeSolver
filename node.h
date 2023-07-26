typedef struct node
{
	/* Pozycja wezla */
	int x;
	int y;

	/* Waga */
	double value;

	/* Ile wezlow ma polaczenie do tego */
	int prev_c;

	/* Do ilu i jakich wezlow mamy polaczenie */
	int next_c;
	struct node *next[4];
} node_t;


void node_init(node_t *, int, int, double);
