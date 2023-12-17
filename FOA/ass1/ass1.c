/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Tianrui Qi 1473217
   Dated:     30/08/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE 500    /* max number of digits per integer value */
#define LINELEN 999    /* maximum length of any input line */
#define NVARS 26       /* number of different variables */

#define CH_A 'a'       /* character 'a', first variable name */

#define ERROR (-1)     /* error return value from some functions */
#define PROMPT "> "    /* the prompt string for interactive input */

#define PRINT '?'      /* the print operator */
#define ASSIGN '='     /* the assignment operator */
#define PLUS '+'       /* the addition operator */
#define MULT '*'       /* the multiplication operator */
#define POWR '^'       /* the power-of operator */
#define DIVS '/'       /* the division operator */
#define ALLOPS "?=+*^/"

#define CH_ZERO '0'    /* character zero */
#define CH_ONE '1'     /* character one */
#define CH_NINE '9'    /* character nine */

#define CH_COM ','     /* character ',' */
#define PUT_COMMAS 3   /* interval between commas in output values */

#define INT_ZERO 0     /* integer 0 */
#define INT_ONE 1     /* integer 1 */
#define INT_BASE (10)  /* integer base */

// division renormalization constants
#define DIVISOR_PREFIX 5
#define DIVISOR_NORMALIZATION {-1, 5, 3, 2, 2}

// maximum/minimum macros for performance
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/// RULES THAT CHANGE PROGRAM BEHAVIOR ///

// do not crash on overflow
// because the crashing on overflow rule is kinda dumb
//#define RULE_OVERFLOW_CONTINUE
//#undef RULE_OVERFLOW_CONTINUE

// enable the fft algorithm when multiplying
//#define RULE_USE_FFT
//#undef RULE_USE_FFT

/// END OF RULES ///

#ifdef RULE_USE_FFT
// maths types for fft
#include <complex.h>
#include <math.h>

#define PI (3.14159265358979323846)
typedef double complex dc_t;

// multiplication fft constants
#define FFT_LIMIT 80
#endif

// arb precision base integer type
typedef struct
{
    int digits[INTSIZE];
    int length;
} longint_t;

// longint_t zero and ones
#define LONG_ZERO {{INT_ZERO}, 0}
#define LONG_ONE {{INT_ONE, INT_ZERO}, 1}
// highest longint_t value
#define LONG_MOST {{INT_BASE-1}, INTSIZE}

// denotes a computation result
typedef enum
{
    STATUS_OK,
    STATUS_OVERFLOW,
    STATUS_ZERODIV
} COMPUTE_STATUS;

/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);  // NOLINT

void print_prompt(void);

void print_tadaa();

void print_error(char *message);

int read_line(char *line, int maxlen);

void process_line(longint_t vars[], char *line);

int get_second_value(longint_t vars[], char *rhsarg,
                     longint_t *second_value);

int to_varnum(char ident);

/****************************************************************/

void do_print(int varnum, longint_t *var);

void do_assign(longint_t *var1, longint_t *var2);

COMPUTE_STATUS do_plus(longint_t *var1, const longint_t *var2);

void zero_vars(longint_t vars[]);

int parse_num(char *rhs, longint_t *result);

void handle_compute_status(COMPUTE_STATUS status);

/****************************************************************/

COMPUTE_STATUS do_mult(longint_t *var1, const longint_t *var2);

COMPUTE_STATUS do_naivemult(longint_t *var1, const longint_t *var2);

#ifdef RULE_USE_FFT

int should_fft(int len1, int len2);

int upper_pow2(int n);

void poly_mult(
        const int *poly1, const int *poly2, int n1, int n2,
        int *out, int n
);

void fft(dc_t *coefs, int n);

void ifft(dc_t *ys, int n);

#endif

/****************************************************************/

COMPUTE_STATUS do_exp(longint_t *var1, const longint_t *var2);

int do_dec(longint_t *var);

/****************************************************************/

int do_order(const longint_t *var1, const longint_t *var2);

COMPUTE_STATUS do_sub_positive(longint_t *var1, const longint_t *var2);

COMPUTE_STATUS do_div(longint_t *var1, const longint_t *var2);

void do_div10(longint_t *var);

void do_mul10(longint_t *var, int exp);


/****************************************************************/

int long_zero(const longint_t *var);

/****************************************************************/


/* Main program controls all the action
*/
int main()
{
    char line[LINELEN + 1] = {0};
    longint_t vars[NVARS];

    zero_vars(vars);
    print_prompt();
    while (read_line(line, LINELEN))
    {
        if (strlen(line) > 0)
        {
            /* non-empty line, so process it */
            process_line(vars, line);
        }
        print_prompt();
    }

    print_tadaa();


    return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void print_prompt(void)
{
    if (isatty(fileno(stdin)))
    {
        fprintf(stderr, "> ");
        fflush(stderr);
    }
}

void print_tadaa()
{
    /* all done, so pack up bat and ball and head home,
       getting the exact final lines right is a bit tedious,
       because input might be coming from a file and output
       might be going to a file */
    if (isatty(fileno(stdin)) && isatty(fileno(stdout)))
    {
        printf("\n");
    }
    printf("ta daa!!!\n");
    if (isatty(fileno(stdin)) && !isatty(fileno(stdout)))
    {
        fprintf(stderr, "\n");
    }
}

void print_error(char *message)
{
    /* need to write an error message to the right place(s)
    */
    if (isatty(fileno(stdin)) || isatty(fileno(stdout)))
    {
        fprintf(stderr, "%s\n", message);
        fflush(stderr);
    }
    if (!isatty(fileno(stdout)))
    {
        printf("%s\n", message);
    }
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int read_line(char *line, int maxlen)
{
    int i = 0, c;
    while (((c = getchar()) != EOF) && (c != '\n'))
    {
        if (i < maxlen && !isspace(c))
        {
            line[i++] = c;  // NOLINT
        }
    }
    line[i] = '\0';
    /* then, if the input is coming from a file or the output
       is going to a file, it is helpful to echo the input line
       and record what the command was */
    if (!isatty(fileno(stdin)) || !isatty(fileno(stdout)))
    {
        printf("%s%s\n", PROMPT, line);
    }
    return ((i > 0) || (c != EOF));
}

/* Process a command by parsing the input line into parts
*/
void process_line(longint_t vars[], char *line)
{
    int varnum, optype, status;
    longint_t second_value;

    /* determine the LHS variable, it
       must be first character in compacted line
    */
    varnum = to_varnum(line[0]);
    if (varnum == ERROR)
    {
        print_error("invalid LHS variable");
        return;
    }

    /* more testing for validity
    */
    if (strlen(line) < 2)
    {
        print_error("no operator supplied");
        return;
    }

    /* determine the operation to be performed, it
       must be second character of compacted line
    */
    optype = line[1]; // NOLINT
    if (strchr(ALLOPS, optype) == NULL)
    {
        print_error("unknown operator");
        return;
    }

    /* determine the RHS argument (if one is required),
       it must start in the third character of compacted line
    */
    if (optype != PRINT)
    {
        if (strlen(line) < 3)
        {
            print_error("no RHS supplied");
            return;
        }
        status = get_second_value(vars, line + 2, &second_value);
        if (status == ERROR)
        {
            print_error("RHS argument is invalid");
            return;
        }
    }

    /* finally, do the actual operation
    */
    COMPUTE_STATUS result = STATUS_OK;
    if (optype == PRINT)
    {
        do_print(varnum, &vars[varnum]);
    }
    else if (optype == ASSIGN)
    {
        do_assign(&vars[varnum], &second_value);
    }
    else if (optype == PLUS)
    {
        result = do_plus(&vars[varnum], &second_value);
    }
    else if (optype == MULT)
    {
        result = do_mult(&vars[varnum], &second_value);
    }
    else if (optype == POWR)
    {
        result = do_exp(&vars[varnum], &second_value);
    }
    else if (optype == DIVS)
    {
        result = do_div(&vars[varnum], &second_value);
    }
    else
    {
        print_error("operation not available yet");
        return;
    }

    handle_compute_status(result);
}

void handle_compute_status(COMPUTE_STATUS status)
{
    if (status == STATUS_OVERFLOW)
    {
        print_error("operation overflow");
    }
    else if (status == STATUS_ZERODIV)
    {
        print_error("operation divided by zero");
    }

#ifndef RULE_OVERFLOW_CONTINUE
    if (status != STATUS_OK)
    {
        exit(EXIT_FAILURE);
    }
#endif
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int to_varnum(char ident)
{
    int varnum;
    varnum = ident - CH_A;
    if (0 <= varnum && varnum < NVARS)
    {
        return varnum;
    }
    else
    {
        return ERROR;
    }
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int get_second_value(
        longint_t vars[], char *rhsarg,
        longint_t *second_value
)
{
    char *p;
    int varnum2;
    if (isdigit(*rhsarg))
    {
        /* first character is a digit, so RHS is a number
           now check the rest of RHS for validity */
        for (p = rhsarg + 1; *p; p++)
        {
            if (!isdigit(*p))
            {
                /* nope, found an illegal character */
                return ERROR;
            }
        }
        /* nothing wrong, ok to convert */
        int status = parse_num(rhsarg, second_value);
        return status;
    }
    else
    {
        /* argument is not a number, so should be a variable */
        varnum2 = to_varnum(*rhsarg);
        if (varnum2 == ERROR || strlen(rhsarg) != 1)
        {
            /* nope, not a variable either */
            return ERROR;
        }
        /* and finally, get that variable's value */
        do_assign(second_value, vars + varnum2);
        return !ERROR;
    }
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a long_int, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Set the vars array to all zero values
*/
void zero_vars(longint_t vars[])
{
    int i;
    longint_t zero = LONG_ZERO;
    for (i = 0; i < NVARS; i++)
    {
        do_assign(vars + i, &zero);
    }
}


/* Create an internal-format number out of a string
*/
int parse_num(char *rhs, longint_t *result)
{
    // remove prefix zeros
    while (*rhs == CH_ZERO)
    {
        rhs++;
    }

    int length = (int) strlen(rhs);

    if (length > INTSIZE)
    {

#ifdef RULE_OVERFLOW_CONTINUE
        // graceful return
        return ERROR;
#else
        // force crash
        print_error("integer overflow from input");
        exit(EXIT_FAILURE);
#endif

    }

    // default integer value
    longint_t value = {{INT_ZERO}, length};

    // loop through the characters
    int remaining = length;
    for (int i = 0; i < length; ++i)
    {
        value.digits[--remaining] = rhs[i] - CH_ZERO;
    }

    *result = value;
    return !ERROR;
}

/****************************************************************/

/* Print out a longint value
*/
void do_print(int varnum, longint_t *var)
{
    printf("register %c: ", varnum + CH_A);

    // handle zero
    if (long_zero(var))
    {
        putchar(CH_ZERO);
        putchar('\n');
        return;
    }

    // deal with the commas
    int offset = var->length % PUT_COMMAS;

    // represents the number of digits before another comma
    // when dividing perfectly (ie offset=0),
    // we skip the first comma and set it to 3
    int comma_countdown = (offset == 0) ? PUT_COMMAS : offset;

    // print each digit
    for (int i = var->length - 1; i >= 0; --i)
    {
        // printing comma
        if (comma_countdown == 0)
        {
            putchar(CH_COM);

            // reset countdown
            comma_countdown = PUT_COMMAS;
        }

        printf("%d", var->digits[i]);
        comma_countdown -= 1;
    }
    putchar('\n');
}

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2

*/
void do_assign(longint_t *var1, longint_t *var2)
{
    *var1 = *var2;
}

/****************************************************************/


/**
 * Adds var2 to var1.
 * Runs in O(n), where n is the digit length.
 *
 * @param var1 Output and Input 1
 * @param var2 Input 2
 * @return Computation status
 */
COMPUTE_STATUS do_plus(longint_t *var1, const longint_t *var2)
{

#ifdef RULE_OVERFLOW_CONTINUE
    // saving var1 in case of overflow
    longint_t backup = *var1;
#endif

    // determine new largest length based on the max of the two ints
    int max_len = MAX(var1->length, var2->length);
    var1->length = max_len;

    // we assert that max_len is bounded by INTSIZE,
    // for they are stored in longints,
    // hence no overflow can occur based on length alone

    // digit-wise addition with carry
    int carry = 0;
    for (int i = 0; i < max_len; ++i)
    {
        int result = var1->digits[i] + var2->digits[i] + carry;
        var1->digits[i] = result % INT_BASE;
        carry = result / INT_BASE;
    }

    if (carry > 0)
    {
        // overflow check upon carry
        if (var1->length >= INTSIZE)
        {

#ifdef RULE_OVERFLOW_CONTINUE
            // we will restore the backup
            *var1 = backup;
#endif
            return STATUS_OVERFLOW;
        }

        var1->digits[var1->length++] = carry;
    }

    return STATUS_OK;
}

/**
 * Multiplies var1 and var2, storing the result in var1.
 * This optionally uses the FFT method, with a runtime of O(n log n),
 * where n is the digit length.
 *
 * @param var1 Output and Input 1
 * @param var2 Input 2
 * @return Computation status
 */
COMPUTE_STATUS do_mult(longint_t *var1, const longint_t *var2)
{
#ifdef RULE_USE_FFT
    // fft check, because for smaller numbers the fft has too much overhead
    if (!should_fft(var1->length, var2->length))
    {
#endif
    return do_naivemult(var1, var2);
#ifdef RULE_USE_FFT
    }

    if (long_zero(var1) || long_zero(var2))
    {
        longint_t zero = LONG_ZERO;
        *var1 = zero;
        return STATUS_OK;
    }

    // 999x999 = 998001, 3+3=6
    // 100x100 =  10000
    // which is the new maximum size
    int max_size = var1->length + var2->length;

    // when the minimum size (max size - 1) is overflowing
    if (max_size - 1 > INTSIZE)
    {
        return STATUS_OVERFLOW;
    }


    // allocate polynomial output storage
    int result[max_size];
    for (int i = 0; i < max_size; ++i)
        result[i] = INT_ZERO;

    /*
     * The fft multiplication treatment
     * We can treat a base-10 integer abc as
     *      a + b*10 + c*10^2 = P(10)
     * Hence by performing polynomial multiplication, we get some new P'(x)
     *      a' + b'*x + c'*x^2 + ...
     * of which P'(10) is our answer
     */
    // let n = maxsize
    // perform polynomial multiplication in O(n log n)
    poly_mult(
            var1->digits, var2->digits,
            var1->length, var2->length,
            result, max_size
    );

    // adding each term's contribution
    // performed in O(n)
    int carry = 0;
    longint_t output = LONG_ZERO;
    for (int i = 0; i < max_size; ++i)
    {
        int coef = result[i];

        // compute current digit and carry
        int digit = (carry + coef) % INT_BASE;
        carry = (carry + coef) / INT_BASE;

        // skip loop when there's no digit
        if (digit == INT_ZERO)
            continue;

        // check for overflow
        if (i >= INTSIZE)
        {
            return STATUS_OVERFLOW;
        }

        output.length = i + 1;
        output.digits[i] = digit;
    }

    // carry must be zero, for we've reached the maximum size of multiplication

    // copy integer
    *var1 = output;

    return STATUS_OK;
#endif
}

/**
 * Performs integer multiplication var1 * var2, storing the result in var1.
 * Runs in O(nm), where n,m are the digit lengths.
 *
 * @param var1
 * @param var2
 * @return Compute status
 */
COMPUTE_STATUS do_naivemult(longint_t *var1, const longint_t *var2)
{
    // zero check
    if (long_zero(var1) || long_zero(var2))
    {
        longint_t zero = LONG_ZERO;
        *var1 = zero;
        return STATUS_OK;
    }

    // when the minimum size (max size - 1) is overflowing
    int max_size = var1->length + var2->length;
    if (max_size - 1 > INTSIZE)
    {
        return STATUS_OVERFLOW;
    }

    // loop each single digit partial sums
    longint_t output = LONG_ZERO;
    for (int i = 0; i < var2->length; ++i)
    {
        // we populate this int for single digit operand multiplication
        longint_t working = LONG_ZERO;

        // this is between [0,9]
        int operand = var2->digits[i];

        // carry on the operand multiplication
        int carry = 0;
        for (int j = 0; j < var1->length; ++j)
        {
            int digit = operand * var1->digits[j] + carry;

            working.digits[i + j] = digit % INT_BASE;
            carry = digit / INT_BASE;

            if (digit > 0)
            {
                working.length = i + j + 1;
            }
        }

        // final carry
        if (carry > 0)
        {
            working.length = i + var1->length + 1;
            if (working.length > INTSIZE)
            {
                return STATUS_OVERFLOW;
            }

            working.digits[i + var1->length] = carry % INT_BASE;
        }

        // add the digit partial sum
        COMPUTE_STATUS status = do_plus(&output, &working);
        if (status == STATUS_OVERFLOW)
        {
            return STATUS_OVERFLOW;
        }
    }

    // copy to output
    *var1 = output;

    return STATUS_OK;
}

#ifdef RULE_USE_FFT

/**
 * Returns whether we should use fft for multiplication
 *
 * @param len1 Number 1
 * @param len2 Number 2
 * @return 1 if should use fft, 0 if should not
 */
int should_fft(int len1, int len2)
{
    // we avoid fft for mostly single digit multiplications
    // arb. limit of 80 characters
    if (len1 < FFT_LIMIT || len2 < FFT_LIMIT)
    {
        return 0;
    }

    return 1;
}

/**
 * Returns the first power-of-2 that is greater than n.
 * Runs in O(log n).
 *
 * @param n
 * @return The first power-of-2 greater than n
 */
int upper_pow2(int n)
{
    int i;
    for (i = 1; i < n; i *= 2);
    return i;
}

/**
 * Performs polynomial multiplication on the coefficients
 *
 * @param poly1 Polynomial 1
 * @param poly2 Polynomial 2
 * @param n1 Length of P1
 * @param n2 Length of P2
 * @param out Output buffer
 * @param n Length of output buffer
 */
void poly_mult(
        const int *poly1, const int *poly2, int n1, int n2,
        int *out, int n
)
{
    // we need the polynomials to have 2^n length
    int size = upper_pow2(n);

    // copy and pad each poly to powers of 2 for the fft
    dc_t p1[size], p2[size];
    for (int i = 0; i < size; ++i)
    {
        if (i < n1)
        {
            p1[i] = poly1[i];
        }
        else
        {
            p1[i] = 0;
        }

        if (i < n2)
        {
            p2[i] = poly2[i];
        }
        else
        {
            p2[i] = 0;
        }
    }

    // compute fft coordinates
    fft(p1, size);
    fft(p2, size);

    // multiply piecewise on the coordinates
    for (int i = 0; i < size; ++i)
    {
        p1[i] *= p2[i];
    }

    // recompute coefficients
    ifft(p1, size);

    // copy into output array
    for (int i = 0; i < n; ++i)
    {
        // we round it too
        out[i] = (int) round(creal(p1[i])) / size;
    }
}

/**
 * Forward FFT
 *
 * @param coefs The signal coefficients
 * @param n Length of signal
 */
void fft(dc_t *coefs, int n)  // NOLINT
{
    // cached for the precomputed roots of unity
    static dc_t roots[2 * INTSIZE + 1];
    static int cached = 0;
    if (!cached)
    {
        for (int i = 2; i <= 2 * INTSIZE; i *= 2)
        {
            double w_angle = 2.0 * PI / (double) i;
            roots[i] = cos(w_angle) + I * sin(w_angle);
        }
        cached = 1;
    }


    // base case
    if (n == 1)
    {
        return;
    }

    int halfn = n / 2;

    // partition into odd and even polynomials
    dc_t evens[halfn], odds[halfn];
    for (int i = 0; i < halfn; ++i)
    {
        evens[i] = coefs[2 * i];
        odds[i] = coefs[2 * i + 1];
    }

    // eval subproblem-fft
    fft(evens, halfn);
    fft(odds, halfn);

    // the roots of unity
    dc_t w = roots[n];
    dc_t wi = 1;

    for (int k = 0; k < halfn; ++k)
    {
        coefs[k] = evens[k] + wi * odds[k];
        coefs[halfn + k] = evens[k] - wi * odds[k];
        wi *= w;
    }
}

/**
 * Inverse FFT
 *
 * @param ys The input coordinates
 * @param n Length of coordinates
 */
void ifft(dc_t *ys, int n)  // NOLINT
{
    // cached for the precomputed roots of unity
    static dc_t roots[2 * INTSIZE + 1];
    static int cached = 0;
    if (!cached)
    {
        for (int i = 2; i <= 2 * INTSIZE; i *= 2)
        {
            double w_angle = -2.0 * PI / (double) i;
            roots[i] = cos(w_angle) + I * sin(w_angle);
        }
        cached = 1;
    }

    // base case
    if (n == 1)
    {
        return;
    }

    int halfn = n / 2;

    // partition into odd and even polynomials
    dc_t evens[halfn], odds[halfn];
    for (int i = 0; i < halfn; ++i)
    {
        evens[i] = ys[2 * i];
        odds[i] = ys[2 * i + 1];
    }

    // eval subproblem-fft
    ifft(evens, halfn);
    ifft(odds, halfn);

    // the roots of unity
    dc_t w = roots[n];
    dc_t wi = 1;

    for (int k = 0; k < halfn; ++k)
    {
        ys[k] = evens[k] + wi * odds[k];
        ys[halfn + k] = evens[k] - wi * odds[k];
        wi *= w;
    }
}

#endif

/**
 * Computes the exponentiation base var1 power var2, storing the result in var1.
 * Runs in O(m n log n) or O(mn^2), where n is the base digit length,
 * m is the exponent value.
 *
 * @param var1 Base
 * @param var2 Power/Exponent
 * @return Computation status
 */
COMPUTE_STATUS do_exp(longint_t *var1, const longint_t *var2)
{
    // copy of the exponent iterator
    longint_t iterator = *var2;

    // result storage
    longint_t result = LONG_ONE;

    // repeatedly decrement exponent by exponent times
    while (do_dec(&iterator))
    {
        // perform multiplication on result
        int status = do_mult(&result, var1);
        if (status == STATUS_OVERFLOW)
        {
            return STATUS_OVERFLOW;
        }
    }

    *var1 = result;
    return STATUS_OK;
}

/**
 * Decrements var by 1.
 * Runs in O(1) amortized.
 *
 * @param var Input/Output to be decremented
 * @return If var is none, zero, thus, successfulness
 */
int do_dec(longint_t *var)
{
    // zero cannot be decremented
    if (long_zero(var))
    {
        return 0;
    }

    // decrease the least sig digit
    var->digits[0] -= 1;

    // keep decreasing higher order digits if it needs to carry
    // except the most sig digit
    for (int i = 0; i < var->length - 1; ++i)
    {
        // if needing to carry
        if (var->digits[i] < INT_ZERO)
        {
            // reset current digit to maximum
            var->digits[i] = INT_BASE - 1;
            // decrement the next higher sig digit
            var->digits[i + 1] -= 1;
        }
        else
        {
            break;
        }
    }

    // when the most sig digit is empty (= 0)
    if (var->digits[var->length - 1] == INT_ZERO)
    {
        // we remove it
        var->digits[var->length - 1] = INT_ZERO;
        var->length -= 1;
    }

    return 1;
}

/**
 * Division of var1 by var2, storing result in var1.
 * Uses the long division algorithm, partially using Knuth's Algorithm D.
 * https://ridiculousfish.com/blog/posts/labor-of-division-episode-iv.html
 *
 * Runs in O(n^2) time, where n is the digit length of dividend
 *
 * @param var1 Dividend and output
 * @param var2 Divisor
 * @return Computation status
 */
COMPUTE_STATUS do_div(longint_t *var1, const longint_t *var2)
{
    if (long_zero(var2))
    {
        return STATUS_ZERODIV;
    }

    longint_t divisor = *var2;

    // normalize dividend/divisor so that the leading digit is >= 5
    // *the five is hand chosen due to algorithm D*
    // this is an unavoidable magic number to improve performance
    int leading = divisor.digits[divisor.length - 1];
    if (leading < DIVISOR_PREFIX)
    {
        // magic numbers to scale the divisor leading digit to >= 5
        int scale[DIVISOR_PREFIX] = DIVISOR_NORMALIZATION;
        longint_t mult = {{scale[leading]},
                          1};

        // we only normalize when the dividend doesn't overflow
        COMPUTE_STATUS status = do_mult(var1, &mult);
        if (status != STATUS_OVERFLOW)
        {
            do_mult(&divisor, &mult);  // which will never overflow
        }
    }

    // build division lookup table
    // with table[i] storing divisor*i
    longint_t table[INT_BASE] = {LONG_ZERO};
    for (int i = 1; i < INT_BASE; ++i)
    {
        table[i] = divisor;
        COMPUTE_STATUS status = do_plus(&table[i], &table[i - 1]);

        // we put the maximum when the table overflows
        if (status == STATUS_OVERFLOW)
        {
            longint_t most = LONG_MOST;
            table[i] = most;
        }
    }

    longint_t result = LONG_ZERO, dividend = LONG_ZERO;

    // shift through the digits of the var1 dividend
    for (int i = var1->length - 1; i >= 0; --i)
    {
        // left shift dividend
        do_mul10(&dividend, 1);

        // bring down digit to dividend
        dividend.digits[0] = var1->digits[i];
        if (dividend.length == 0 && dividend.digits[0] != INT_ZERO)
        {
            dividend.length = 1;
        }

        // continue when dividend do not divide divisor
        if (do_order(&dividend, &divisor) < 0)
        {
            continue;
        }

        // guess the upper limit of the quotient
        // which is (first two char of dividend) / (first char of divisor)
        int left = dividend.digits[dividend.length - 1];
        if (dividend.length > divisor.length && dividend.length >= 2)
        {
            left = INT_BASE * left +
                   dividend.digits[dividend.length - 2];
        }
        int guess = MIN(INT_BASE - 1,
                        left / divisor.digits[divisor.length - 1]);

        // if overflows dividend, then iterate lower limits
        longint_t *product = &table[guess];
        while (do_order(product, &dividend) > 0 && guess > 0)
        {
            guess -= 1;
            product = &table[guess];
        }

        // update result and length
        result.digits[i] = guess;
        result.length = MAX(i + 1, result.length);

        // subtract partial from dividend and continue
        do_sub_positive(&dividend, product);
    }

    *var1 = result;

    return STATUS_OK;
}

/**
 * Returns the ordering between var1 and var2
 *
 * @param var1
 * @param var2
 * @return Positive if var1 is greater, negative if var1 is smaller,
 * and zero if equal
 */
int do_order(const longint_t *var1, const longint_t *var2)
{
    // simple length check
    if (var1->length != var2->length)
    {
        return var1->length - var2->length;
    }

    // check from most to least sig
    for (int i = var1->length - 1; i >= 0; --i)
    {
        // if digit mismatch, return the ordering
        if (var1->digits[i] != var2->digits[i])
        {
            return var1->digits[i] - var2->digits[i];
        }
    }

    // the two are equal, return 0
    return 0;
}


/**
 * Subtract var2 from var1, storing the result in var1.
 * Requires var1 to be larger than var2.
 * Runs in O(n), where n is the digit length.
 *
 * @param var1
 * @param var2
 * @return Computation status
 */
COMPUTE_STATUS do_sub_positive(longint_t *var1, const longint_t *var2)
{
    // zero check
    if (long_zero(var2))
    {
        return STATUS_OK;
    }

    // if var2 > var1, return zero
    if (do_order(var1, var2) <= 0)
    {
        longint_t zero = LONG_ZERO;
        *var1 = zero;
        return STATUS_OK;
    }

    for (int i = 0; i < var1->length; ++i)
    {
        // process negative carries
        if (i >= var2->length)
        {
            if (var1->digits[i] < 0)
            {
                // carry
                var1->digits[i + 1] -= 1;
                var1->digits[i] += INT_BASE;
            }
            else
            {
                // or we are finished
                break;
            }
        }

        // subtraction carry
        if (var1->digits[i] < var2->digits[i])
        {
            var1->digits[i + 1] -= 1;
            var1->digits[i] += INT_BASE;
        }

        var1->digits[i] -= var2->digits[i];
    }

    // and shrink length when needed
    while (var1->digits[var1->length - 1] == INT_ZERO && var1->length > 0)
    {
        var1->length -= 1;
    }

    return STATUS_OK;
}

/**
 * Computes the truncated division by 10. This is equivalent to a right shift.
 * Runs in O(n), where n is the digit length.
 *
 * @param var
 */
void do_div10(longint_t *var)
{
    // remains zero when var is zero
    if (long_zero(var))
    {
        return;
    }

    // slide everything over by 1
    for (int i = 1; i < var->length; ++i)
    {
        var->digits[i - 1] = var->digits[i];
    }

    // zero last and adjust length
    var->digits[var->length - 1] = INT_ZERO;
    var->length -= 1;
}

/**
 * Computes the 10^exp multiplication. This is equivalent to a left shift.
 * Runs in O(n), where n is the digit length.
 *
 * @param var
 * @param exp How much to shift by
 */
void do_mul10(longint_t *var, int exp)
{
    if (long_zero(var))
    {
        return;
    }

    // slide everything over by 1
    for (int i = var->length - 1; i >= 0; --i)
    {
        var->digits[i + exp] = var->digits[i];
    }

    // zero first exp digits
    for (int i = 0; i < exp; ++i)
    {
        var->digits[i] = INT_ZERO;
    }

    var->length += exp;
}

/**
 * Returns whether the integer is zero.
 *
 * @param var
 * @return Is zero
 */
int long_zero(const longint_t *var)
{
    return var->length == 0;
}


// algorithms are fun