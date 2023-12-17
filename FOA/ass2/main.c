/* Program to generate text based on the context provided by input prompts.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
  September 2023, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.
  All included code is (c) Copyright University of Melbourne, 2023.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Tianrui Qi
  Dated:     21/09/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>
#include <string.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message
#define NOSFMT "Number of statements: %d\n"                 // no. of statements
#define NOCFMT "Number of characters: %d\n"                 // no. of chars
#define NPSFMT "Number of states: %d\n"                     // no. of states
#define TFQFMT "Total frequency: %d\n"                      // total frequency

// ellipsis
#define ELLIPSIS ("...")

// the stage1 and stage2 output line limit
#define REPLAY_LIMIT (37)

// whether a character is end-of-line
#define IS_EOL(c) ((c) == EOF || (c) == '\n' || (c) == '\r')

/* TYPE DEFINITIONS ----------------------------------------------------------*/

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

// represents the edges/arcs a state can have
struct edge_t
{
    char *prefix;    // edge character(s)
    state_t *child;  // corresponding child
    edge_t *next;    // next edge in the state
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

int automata_statements(const automata_t *automata);

static int automata_states_recursive(const state_t *state);

int automata_states(const automata_t *automata);

static int automata_chars_recursive(const state_t *state);

int automata_chars(const automata_t *automata);

static int automata_compress_recurse(state_t *state, int times);

void automata_compress(automata_t *automata, int times);

static void buf_add(const char *c, char *buf, int n, int *ptr);

static bool str_partial(const char *a, const char *b, int *matches);

void stage0();

void stage1(automata_t *automata);

void stage2(automata_t *automata);


/* USEFUL FUNCTIONS ----------------------------------------------------------*/

/**
 * Returns a new reader object at the current stdin state
 *
 * @return
 */
reader_t make_reader()
{
    reader_t reader = {'\0', false};
    return reader;
}

/**
 * Returns whether the reader is at an EOL position
 *
 * @param reader Reader object
 * @return
 */
bool reader_eol(reader_t *reader)
{
    // always buffer read the next character
    if (!reader->buffered)
    {
        reader->buffered = true;
        reader->buffer = getchar();
    }

    return IS_EOL(reader->buffer);
}

/**
 * Skips all characters til the next non-EOL character
 *
 * @param reader Reader object
 */
void reader_nextline(reader_t *reader)
{
    while (true)
    {
        // put one item into buffer
        if (!reader->buffered)
        {
            reader->buffered = true;
            reader->buffer = getchar();
        }

        // return if EOL is reached
        if (IS_EOL(reader->buffer))
        {
            reader->buffered = false;

            // check for '\r\n'
            if (reader->buffer == '\r')
            {
                reader->buffered = true;
                reader->buffer = getchar();

                // then discard the '\n' too
                if (reader->buffer == '\n')
                {
                    reader->buffered = false;
                }
            }
            break;
        }

        // otherwise just discard the buffer and keep parsing
        reader->buffered = false;
    }
}

/**
 * Retrieves a character from the reader
 *
 * @param reader Reader object
 * @return
 */
int reader_get(reader_t *reader)
{
    if (reader->buffered)
    {
        reader->buffered = false;
        return reader->buffer;
    }

    return getchar();
}

/**
 * Read a maximum number of characters from stdin, storing it into a buffer
 *
 * @param reader Reader object
 * @param max The maximum number of characters to read before EOL
 * @param buf Buffer pointer
 * @return The number of characters read before EOL
 */
int reader_read(reader_t *reader, int max, char **buf)
{
    // allocate buffer
    *buf = malloc((max + 1) * sizeof(char));
    assert(*buf != NULL);

    // read characters til EOL
    int i = 0;
    while (i < max && !reader_eol(reader))
    {
        (*buf)[i] = (char) reader_get(reader);
        i += 1;
    }

    // terminate string buffer
    (*buf)[i] = '\0';
    return i;
}

/**
 * Returns a new allocated state_t
 *
 * @return
 */
state_t *make_state()
{
    state_t *item = malloc(sizeof(state_t));
    assert(item != NULL);

    item->edges = NULL;
    item->frequency = 0;

    return item;
}

/**
 * Frees an allocated state_t, including all child states and edges
 *
 * @param state
 */
void free_state(state_t *state)
{
    if (state == NULL)
    {
        return;
    }

    // free edges & child states
    free_edge(state->edges);

    // then free state
    free(state);
}

/**
 * Frees an allocated state, not freeing any internal pointers
 *
 * @param state
 */
void free_state_only(state_t *state)
{
    free(state);
}

// Sort a linked list of edges by prefix, alphabetically, returns
// the start of the new linked list. Does not allocate!
static edge_t *merge_sort(edge_t *root, int n)
{
    // base case with less than 2 elements
    if (n <= 1)
    {
        return root;
    }

    // find the halfway point
    int half = n / 2;
    edge_t *half_root = root;
    for (int i = 0; i < half; ++i)
    {
        half_root = half_root->next;
    }

    // sort left and right
    int left = half, right = n - half;
    root = merge_sort(root, left);
    half_root = merge_sort(half_root, right);

    // merge the two parts
    edge_t *newroot = NULL, *current = NULL;
    int i = 0, j = 0;
    while (i < left || j < right)
    {
        bool leftless = true;  // whether the left part is merged

        if (i < left && j < right)
        {
            // when both parts are non-empty
            // compare on the prefix
            leftless = strcmp(root->prefix, half_root->prefix) <= 0;
        }
        else if (i < left)  // merge left
        {
            leftless = true;
        }
        else if (j < right)  // merge right
        {
            leftless = false;
        }

        // merge left or right part
        edge_t *new_edge;
        if (leftless)
        {
            // left part
            new_edge = root;
            root = root->next;
            i += 1;
        }
        else
        {
            // right part
            new_edge = half_root;
            half_root = half_root->next;
            j += 1;
        }

        // append to new list
        if (newroot == NULL)
        {
            current = newroot = new_edge;
        }
        else
        {
            current = current->next = new_edge;
        }
    }

    // cleanup the end edge pointer
    current->next = NULL;
    return newroot;
}

/**
 * Returns the number of edges a state has
 *
 * @param state The state
 * @return
 */
int state_edges(const state_t *state)
{
    int count = 0;
    edge_t *edge = state->edges;
    while (edge != NULL)
    {
        count += 1;
        edge = edge->next;
    }

    return count;
}

/**
 * Sort in place the edges of a state, alphabetically
 *
 * @param state
 */
void state_sorted_edges(state_t *state)
{
    state->edges = merge_sort(state->edges, state_edges(state));
}


/**
 * Returns a new allocated edge
 *
 * @param c Edge prefix character
 * @return
 */
edge_t *make_edge(char c)
{
    edge_t *newedge = malloc(sizeof(edge_t));
    assert(newedge != NULL);

    newedge->next = NULL;

    // add prefix to edge
    newedge->prefix = malloc(2 * sizeof(char));
    assert(newedge->prefix != NULL);
    newedge->prefix[0] = c;
    newedge->prefix[1] = '\0';

    newedge->child = NULL;

    return newedge;
}

/**
 * Append a prefix to the edge's current prefix
 *
 * @param edge Edge object
 * @param prefix The new null terminated prefix
 */
void update_edge_prefix(edge_t *edge, char *prefix)
{
    // allocate new prefix
    char *newprefix = malloc(
            (strlen(edge->prefix) + strlen(prefix) + 1) * sizeof(char)
    );
    assert(newprefix != NULL);

    // format is 'prefix'+'old-prefix'
    strcpy(newprefix, prefix);
    strcat(newprefix, edge->prefix);

    free(edge->prefix);
    edge->prefix = newprefix;
}

/**
 * Compares the two edges based on frequency and prefix
 *
 * @param edge1 Edge 1
 * @param edge2 Edge 2
 * @return >0 if edge1 > edge2, <0 if edge1 < edge2, =0 if edge1 = edge2
 */
int edge_cmp(const edge_t *edge1, const edge_t *edge2)
{
    if (edge1->child->frequency != edge2->child->frequency)
    {
        return edge1->child->frequency - edge2->child->frequency;
    }

    return strcmp(edge1->prefix, edge2->prefix);
}

/**
 * Frees an edge, its state, and its subsequent edges
 *
 * @param edge
 */
void free_edge(edge_t *edge)
{
    if (edge == NULL)
    {
        return;
    }

    // free child state
    free_state(edge->child);

    // free next edge
    free_edge(edge->next);

    // free prefix
    free(edge->prefix);
    free(edge);
}

/**
 * Frees an edge, without freeing any internal pointers
 *
 * @param edge
 */
void free_edge_only(edge_t *edge)
{
    if (edge == NULL)
    {
        return;
    }

    free(edge->prefix);
    free(edge);
}

/**
 * Returns an empty automata
 *
 * @return
 */
automata_t *make_automata()
{
    automata_t *item = malloc(sizeof(automata_t));
    assert(item != NULL);

    item->root = make_state();

    return item;
}

/**
 * Frees an automata and all its states'n edges
 *
 * @param automata
 */
void free_automata(automata_t *automata)
{
    if (automata == NULL)
    {
        return;
    }

    free_state(automata->root);
    free(automata);
}

/**
 * Use the reader to add a statement to the automata
 *
 * @param automata Automata object
 * @param reader Reader object
 */
void automata_add(automata_t *automata, reader_t *reader)
{
    // assume that reader is on the next line (ie, not eol already)
    state_t *state = automata->root;

    // keep reading til eol
    while (!reader_eol(reader))
    {
        char c = (char) reader_get(reader);

        state->frequency += 1;

        // search for the edge c
        edge_t *edge = state->edges;
        bool matched = false;
        while (edge != NULL)
        {
            // we are only comparing for the first index, as it is a character
            if (edge->prefix[0] == c)
            {
                // matched
                matched = true;
                // increase frequency and move on
                state = edge->child;
                break;
            }

            edge = edge->next;
        }

        // we are done if we've matched an edge
        if (matched)
        {
            continue;
        }

        // make a new edge & create a new state to attach
        edge_t *newedge = make_edge(c);
        newedge->child = make_state();

        // add this edge to the state
        if (state->edges == NULL)
        {
            state->edges = newedge;
        }
        else
        {
            edge = state->edges;
            while (edge->next != NULL)
            {
                edge = edge->next;
            }

            edge->next = newedge;
        }

        // move on to the next state
        state = newedge->child;
    }
}

/**
 * Returns the number of statements in the automata
 *
 * @param automata Automata object
 * @return
 */
int automata_statements(const automata_t *automata)
{
    return automata->root->frequency;
}

// Recursion to count the number of states in this subtree
static int automata_states_recursive(const state_t *state)
{
    int total = 1;

    // count child states
    edge_t *edge = state->edges;
    while (edge != NULL)
    {
        total += automata_states_recursive(edge->child);
        edge = edge->next;
    }

    return total;
}

/**
 * Returns the number of unique states in the automata
 *
 * @param automata Automata object
 * @return
 */
int automata_states(const automata_t *automata)
{
    return automata_states_recursive(automata->root);
}

// Recursion to count the number of characters in the subtree
static int automata_chars_recursive(const state_t *state)
{
    int total = state->frequency;

    // count child characters
    edge_t *edge = state->edges;
    while (edge != NULL)
    {
        total += automata_chars_recursive(edge->child);
        edge = edge->next;
    }

    return total;
}

/**
 * Returns the total frequencies (total characters) in the automata
 *
 * @param automata Automata object
 * @return
 */
int automata_chars(const automata_t *automata)
{
    return automata_chars_recursive(automata->root);
}

// Appends a null-terminated string into a buffer, limited by the buffer size
static void buf_add(const char *c, char *buf, int n, int *ptr)
{
    // while not exceeding the buffer and there are more characters
    while (*ptr < n && *c != '\0')
    {
        buf[*ptr] = *c;

        *ptr += 1;
        c += 1;
    }
}

// Partially matches two strings, puts the most matched length into
// the pointer *matches
static bool str_partial(const char *a, const char *b, int *matches)
{
    *matches = 0;
    // keep comparing both strings
    while (*a != '\0' && *b != '\0')
    {
        if (*a != *b)
        {
            return false;
        }

        *matches += 1;
        a += 1;
        b += 1;
    }

    return true;
}


/**
 * Use the automata to replay a statement from the reader,
 * outputs the replay into a buffer that is length limited
 *
 * @param automata Automata object
 * @param reader Reader object
 * @param buffer Output buffer
 * @param n Buffer length limit
 */
void automata_replay(
        const automata_t *automata, reader_t *reader, char *buffer, int n
)
{
    int bufptr = 0;  // buffer position
    state_t *state = automata->root;  // current state

    bool failed = false, partial = false;
    while (!reader_eol(reader))
    {
        edge_t *edge = state->edges;
        // handle empty edges
        if (edge == NULL)
        {
            // add the none-matching character and exit
            char buf[2] = {(char) reader_get(reader), '\0'};
            buf_add(buf, buffer, n, &bufptr);
            failed = true;
            break;
        }

        // read that prefix length of input
        char *buf;
        int prefix_length = (int) strlen(edge->prefix);
        int read = reader_read(reader, prefix_length, &buf);

        edge_t *max_edge = NULL; // greatest edge that partially matches
        int matches = 0;  // highest partial matches
        while (edge != NULL)
        {
            if (str_partial(edge->prefix, buf, &matches))
            {
                // if partially matching, determine maximum edge
                if (max_edge == NULL
                    || edge_cmp(edge, max_edge) > 0)
                {
                    max_edge = edge;
                }
            }
            edge = edge->next;
        }

        // failed to match anything
        if (max_edge == NULL)
        {
            // print partial matches, +1 to show that last non-matching
            buf[matches + 1] = '\0';
            buf_add(buf, buffer, n, &bufptr);
            free(buf);
            failed = true;
            break;
        }

        buf_add(buf, buffer, n, &bufptr);
        free(buf);
        state = max_edge->child;

        if (read < prefix_length) // when the input is shorter than the prefix
        {
            // generate the rest of the edge prefix
            buf_add(ELLIPSIS, buffer, n, &bufptr);
            buf_add(max_edge->prefix + read, buffer, n, &bufptr);
            partial = true;
            break;
        }
    }

    // add ellipse, only when not partially already added
    if (!partial)
    {
        buf_add(ELLIPSIS, buffer, n, &bufptr);
    }

    // failing early return
    if (failed)
    {
        buffer[bufptr] = '\0';
        return;
    }

    // replay feature starting from state
    while (state->edges != NULL)
    {
        // find the greatest edge
        edge_t *edge = state->edges, *max_edge = NULL;
        while (edge != NULL)
        {
            if (max_edge == NULL || edge_cmp(edge, max_edge) > 0)
            {
                max_edge = edge;
            }
            edge = edge->next;
        }

        // write prefix
        buf_add(max_edge->prefix, buffer, n, &bufptr);
        state = max_edge->child;
    }
    buffer[bufptr] = '\0';
}

// Recursively compress the state and its subtree n times,
// returns the available compressions left
static int automata_compress_recurse(state_t *state, int times)
{
    // base case with no more compressions
    if (times <= 0)
    {
        return 0;
    }

    // while compressible
    while (
            times > 0
            && state_edges(state) == 1
            && state_edges(state->edges->child) >= 1
            )
    {
        // compress: x = state, y = child
        state_t *child = state->edges->child;

        // update edges by looping through the edges of y
        edge_t *edge = child->edges;
        while (edge != NULL)
        {
            update_edge_prefix(edge, state->edges->prefix);
            edge = edge->next;
        }

        // free x->y
        free_edge_only(state->edges);
        // update x edges with the new edges of y
        state->edges = child->edges;
        // free y
        free_state_only(child);

        times -= 1;
    }

    // compress children, prioritize smaller ascii edges
    state_sorted_edges(state);
    edge_t *edge = state->edges;
    while (edge != NULL)
    {
        times = automata_compress_recurse(edge->child, times);
        edge = edge->next;
    }

    return times;
}

/**
 * Compress an automata n times
 *
 * @param automata Automata object
 * @param times The number of compressions
 */
void automata_compress(automata_t *automata, int times)
{
    automata_compress_recurse(automata->root, times);
}

// performs stage0
void stage0()
{
    printf(SDELIM, 0);

    reader_t reader = make_reader();
    automata_t *automata = make_automata();

    // keep adding statements til a line starting with newline
    while (!reader_eol(&reader))
    {
        automata_add(automata, &reader);
        reader_nextline(&reader);
    }

    // clean newline for stage1
    reader_nextline(&reader);

    printf(NOSFMT, automata_statements(automata));
    printf(NOCFMT, automata_chars(automata));
    printf(NPSFMT, automata_states(automata));
    fflush(stdout);

    stage1(automata);
}

// perform stage1
void stage1(automata_t *automata)
{
    printf(SDELIM, 1);

    reader_t reader = make_reader();

    // output buffer
    char output[REPLAY_LIMIT + 1];

    // keep proceeding prompts
    while (!reader_eol(&reader))
    {
        automata_replay(automata, &reader, output, REPLAY_LIMIT);
        printf("%s\n", output);

        // skip remaining unmatched inputs on the line
        reader_nextline(&reader);
    }
    fflush(stdout);

    // clear newline for stage2
    reader_nextline(&reader);

    stage2(automata);
}

// perform stage2
void stage2(automata_t *automata)
{
    printf(SDELIM, 2);

    reader_t reader = make_reader();

    // read compression steps
    int times = 0;
    while (!reader_eol(&reader))
    {
        char c = (char) reader_get(&reader);
        // to parse a decimal number in ascii
        // 10 is the base, and '0' is the ascii zero digit
        times = 10 * times + (c - '0');
    }
    reader_nextline(&reader);

    // compress the automata
    automata_compress(automata, times);

    printf(NPSFMT, automata_states(automata));
    printf(TFQFMT, automata_chars(automata));
    printf(MDELIM);

    // process each prompt
    char output[REPLAY_LIMIT + 1];
    while (!reader_eol(&reader))
    {
        automata_replay(automata, &reader, output, REPLAY_LIMIT);
        printf("%s\n", output);
        reader_nextline(&reader);
    }
    fflush(stdout);

    printf(THEEND);
    free_automata(automata);
}

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main()
{
    stage0();
    return EXIT_SUCCESS;
}

/* THE END -------------------------------------------------------------------*/



// algorithms are fun!!!