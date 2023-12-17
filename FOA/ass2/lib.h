#include <stdbool.h>

// represents a reader object for line inouts
typedef struct
{
    int buffer;  // the next character buffer
    bool buffered;  // whether the reader is buffered
} reader_t;

reader_t make_reader();

bool reader_eol(reader_t *reader);

void reader_nextline(reader_t *reader);

int reader_get(reader_t *reader);

int reader_read(reader_t *reader, int max, char **);

typedef struct state_t state_t;
typedef struct edge_t edge_t;

// represents the edges a state can have
struct edge_t
{
    char *prefix;  // edge character(s)
    state_t *child;  // corresponding child
    edge_t *next;  // next edge in the state
};

edge_t *make_edge(char c);

void update_edge_prefix(edge_t *edge, char *prefix);

int edge_cmp(const edge_t *edge1, const edge_t *edge2);

void free_edge(edge_t *edge);

void free_edge_only(edge_t *edge);

// represents a state node
struct state_t
{
    int frequency;  // node frequency
    edge_t *edges;  // child edges
};

state_t *make_state();

void free_state(state_t *state);

void free_state_only(state_t *state);

int state_edges(const state_t *state);

static edge_t *merge_sort(edge_t *root, int n);

void state_sorted_edges(state_t *state);

// represents an automata
typedef struct
{
    state_t *root;  // root state
} automata_t;

automata_t *make_automata();

void free_automata(automata_t *automata);

void automata_add(automata_t *automata, reader_t *reader);

void automata_replay(
        const automata_t *automata, reader_t *reader,
        char *buf, int n
);

void automata_compress(automata_t *automata, int times);

int automata_statements(const automata_t *automata);

int automata_states(const automata_t *automata);

int automata_chars(const automata_t *automata);

static int automata_states_recursive(const state_t *state);

static int automata_chars_recursive(const state_t *state);

static int automata_compress_recurse(state_t *state, int times);

static void buf_add(const char *c, char *buf, int n, int *ptr);

static bool str_partial(const char *a, const char *b, int *matches);

void stage0();

void stage1(automata_t *automata);

void stage2(automata_t *automata);