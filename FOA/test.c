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

   Signed by: Owen Vandegraaff
   Dated:     05/09/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>


/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A    'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */

#define CH_COM   ','    /* character ',' */
#define COM_DIST 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_ONE  1	/* integer 1 */
#define INT_TEN  10	/* integer 10 */

/* My #defines */
#define DO_NOT_INSERT -1 /* A flag to tell expand_longint to not insert a 
			    number 
			 */
#define OVERFLOW 0 /* do_complain() code for Integer overflow error */
#define DIVIDE_BY_ZERO 1 /* do_complain() code for divide by 0 error */

		      /* All 3 are output constants for longint_comparison*/
#define VAR1_BIGGER 1 /* Signals that Var1 > Var2 */
#define VAR2_BIGGER 0 /* Signals that Var2 > Var1 */
#define VARS_EQUAL  2 /* Signals that Var1 == Var2*/

#define ASCII_0_CODE 48 /* the ASCII code for 0*/
#define BUDDY_OFFSET 1 /* how much space we need to leave for the buddy 
			  variable when constructing longints*/
#define BUDDY_POS    0 /* positon in longint that the buddy variable is stored*/
#define UNITS_COLUMN 1 /* the column in which the units are stored*/

#define MAX_EXPONENT_LENGTH 4 /* The maximum exponent length before an overflow 
				 is guaranteed
			      */

typedef int longint_t[INTSIZE+1];
#define LONG_ZERO {1, 0}
#define LONG_ONE  {1, 1}
#define LONG_TEN  {2, 0, 1}
#define EMPTY_LONGINT {0}



/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg,
	longint_t second_value); 
int  to_varnum(char ident);
void do_print(int varnum, longint_t var);
void do_assign(longint_t var1, longint_t var2);
void do_plus(longint_t var1, longint_t var2);
void zero_vars(longint_t vars[]);
void parse_num(char *rhs, longint_t new_longint);

/* My function prototypes */
int longest_longint(longint_t var1, longint_t var2);
void invert_longint(longint_t target);
void expand_longint(longint_t target, int number);
void purge_leading_zeros(longint_t new_longint);
int equate_lengths(longint_t var1, longint_t var2);
void do_complain(int reason);
void do_multiply(longint_t var1, longint_t var2);
void do_exponentiate(longint_t var1, longint_t var2);
void do_divide (longint_t numerator, longint_t denominator);
int longint_comparison(longint_t var1, longint_t var2);
int do_minus(longint_t var1, longint_t var2);
/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {
	char line[LINELEN+1] = {0};
	longint_t vars[NVARS];

	zero_vars(vars);
	print_prompt();
	while (read_line(line, LINELEN)) {
		if (strlen(line) > 0) {
			/* non empty line, so process it */
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
void
print_prompt(void) {
	if (isatty(fileno(stdin))) {
		fprintf(stderr, "> ");
		fflush(stderr);
	}
}

void
print_tadaa() {
	/* all done, so pack up bat and ball and head home,
	   getting the exact final lines right is a bit tedious,
	   because input might be coming from a file and output
	   might be going to a file */
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
		fprintf(stderr, "\n");
	}
}

void
print_error(char *message) {
	/* need to write an error message to the right place(s)
	*/
	if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(fileno(stdout))) {
		printf("%s\n", message);
	}
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	/* then, if the input is coming from a file or the output
	   is going to a file, it is helpful to echo the input line
	   and record what the command was */
	if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
		printf("%s%s\n", PROMPT, line);
	}
	return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t vars[], char *line) {
	int varnum, optype, status;
	longint_t second_value;

	/* determine the LHS variable, it
	   must be first character in compacted line
	*/
	varnum = to_varnum(line[0]);
	if (varnum==ERROR) {
		print_error("invalid LHS variable");
		return;
	}

	/* more testing for validity 
	*/
	if (strlen(line)<2) {
		print_error("no operator supplied");
		return;
	}

	/* determine the operation to be performed, it
	   must be second character of compacted line
	*/
	optype = line[1];
	if (strchr(ALLOPS, optype) == NULL) {
		print_error("unknown operator\n");
		return;
	}

	/* determine the RHS argument (if one is required),
	   it must start in the third character of compacted line
	*/
	if (optype != PRINT) {
		if (strlen(line)<3) {
			print_error("no RHS supplied");
			return;
		}
		status = get_second_value(vars, line+2, second_value);
		if (status==ERROR) {
			print_error("RHS argument is invalid");
			return;
		}
	}

	/* finally, do the actual operation
	*/
	if (optype == PRINT) {
		do_print(varnum, vars[varnum]);
	} else if (optype == ASSIGN) {
		do_assign(vars[varnum], second_value);
	} else if (optype == PLUS) {
		do_plus(vars[varnum], second_value);
	} else if (optype == MULT) {
		do_multiply(vars[varnum], second_value);
	} else if (optype == POWR) {
        	do_exponentiate(vars[varnum], second_value);
	} else {
		do_divide(vars[varnum], second_value);
	}
	return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
	int varnum;
	varnum = ident - CH_A;
	if (0<=varnum && varnum<NVARS) {
		return varnum;
	} else {
		return ERROR;
	}
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg,
			longint_t second_value) {
	char *p;
	int varnum2;
	if (isdigit(*rhsarg)) {
		/* first character is a digit, so RHS is a number
		   now check the rest of RHS for validity */
		for (p=rhsarg+1; *p; p++) {
			if (!isdigit(*p)) {
				/* nope, found an illegal character */
				return ERROR;
			}
		}
		/* nothing wrong, ok to convert */
		parse_num(rhsarg, second_value);
		return !ERROR;
	} else {
		/* argument is not a number, so should be a variable */
		varnum2 = to_varnum(*rhsarg);
		if (varnum2==ERROR || strlen(rhsarg)!=1) {
			/* nope, not a variable either */
			return ERROR;
		}
		/* and finally, get that variable's value */
		do_assign(second_value, vars[varnum2]);
		return !ERROR;
	}
	return ERROR;
}

/* Set the vars array to all zero values
*/
void
zero_vars(longint_t vars[]) {
	int i;
	longint_t zero = LONG_ZERO;
	for (i=0; i<NVARS; i++) {
		do_assign(vars[i], zero);
	}
	return;
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a longint, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Create an internal-format number out of a string
*/
void
parse_num(char *rhs, longint_t new_longint) {
	int i = 0, length = 0;
	char digit;
	
	/* for each character in the string, convert it into an integer, and 
	   then put said integer into the longint data structure if it isn't a
	   leading zero
	*/
	while ((digit = *(rhs + i)) != '\0') {
		if (length >= INTSIZE) {
			do_complain(OVERFLOW);
		} else {  
			new_longint[length + BUDDY_OFFSET] = digit - CH_ZERO;
			length++; 
		}
		i++;
	}

	new_longint[BUDDY_POS] = length;
	/* Since we read the longint in decending order of magnitude, we'll 
	   need to flip it 
	*/
	invert_longint(new_longint);
	/* And if some rascal tried to put leading zeros in, we'll need to
	   take them out 
	*/
	purge_leading_zeros(new_longint);
}

/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, longint_t var) {
	printf("register %c: ", varnum+CH_A);
	/* For each digit in var, print the digit, and then a comma if needed
	*/
	int numdigits = var[BUDDY_POS];
	int commas_required = (numdigits - 1) / COM_DIST;
	int chars_to_next_comma = (numdigits - 1) % COM_DIST;

	for (int i = numdigits; i >= 1; i--) {
		putchar(var[i] + ASCII_0_CODE);
		if ((commas_required > 0) && (chars_to_next_comma == 0)) {
			putchar(CH_COM);
			chars_to_next_comma = 3;
			commas_required--;
		}
		chars_to_next_comma--;	
	}

	printf("\n");
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2

*/
void
do_assign(longint_t var1, longint_t var2) {
	/* for every integer contained in the original array (including the 
	   buddy variable), copy over a digit to the target array
	*/
	int var2_length = var2[BUDDY_POS]; 
	for (int i = 0; i <= var2_length; i++) {
		var1[i] = var2[i];
	}
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2
*/
void
do_plus(longint_t var1, longint_t var2) {
	/* First better make sure that the variables dont have any junk stored 
	   after their conclusion.
	*/
	int var_length = equate_lengths(var1, var2);
	var1[BUDDY_POS] = var_length;
	int carry = 0;

	/* For every digit in var1 */
	for (int i = 1; i <= var_length; i++) {
		int sum = var1[i] + var2[i] + carry;
		carry = 0;
		/* if the sum is larger than 9, subtract 10 and carry the
		   1 into the next addition
		*/
		if (sum >= INT_TEN) {
			sum -= INT_TEN;
			carry++;
			/* If the addition caused an increase in the order of 
			   magnitude of var1, we must adjust the size 
			*/ 
			if (i == var_length) {
				expand_longint(var1, carry);
			}
		}
		/* Then update the ith digit of var1 to be the sum of var1 and 
		   var2 
		*/
		var1[i] = sum;
	}
	
}

/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/

/* Observes the Buddy variables of two longints, and returns the largest
of the two.
*/
int longest_longint(longint_t var1, longint_t var2) {
	int var1_length = var1[BUDDY_POS];
	int var2_length = var2[BUDDY_POS];
	if (var1_length >= var2_length) {
		return var1_length;
	} else {
		return var2_length;
	}
}

/* Inverts the positions of the digits (NOT including the buddy variable)
   of a longint.
*/
void invert_longint(longint_t target) {
	int length = target[BUDDY_POS];
	/* Swap the ith and the ith last digits until there are no more digits
	   left to be swapped 
	*/
	for (int i = 0; (i + BUDDY_OFFSET) < (length - i); i++) {
		int int1 = target[i + BUDDY_OFFSET];
		target[i + BUDDY_OFFSET] = target[length - i];
		target[length - i] = int1;
	}
}

/* Increases the length of target longint by 1 and puts <number> in the 
   final digit 
*/
void expand_longint(longint_t target, int number) {
	target[BUDDY_POS] += 1;
	int length = target[BUDDY_POS];
	if (length > INTSIZE) {
		do_complain(OVERFLOW);
	}
	if (number != DO_NOT_INSERT) {
		target[length] = number;
	}
}
/* A defensive function to guarantee there are no rogue digits lurking beyond
   the end of the smaller function. There was an issue where instead of all
   digits past var[total_length] being zero, they weren't and I'd like to be
   able to make the assumption that they are. We might as well return the 
   length while we're at it.
*/
int equate_lengths(longint_t var1, longint_t var2) {
	int longest = longest_longint(var1, var2);
	int var1_length = var1[BUDDY_POS];
	int var2_length = var2[BUDDY_POS];
	/* It's computationally wasteful to go through and guarantee that all
	   elements in the range (var_length, INTSIZE) are zero, stopping at
	   the longest of the two variable will be just fine 
	*/
	if (var1_length < longest) {
		for (int i = var1_length + BUDDY_OFFSET; i <= longest; i++) {
			var1[i] = 0;
		}
	} else if (var2_length < longest) {
		for (int i = var2_length + BUDDY_OFFSET; i <= longest; i++) {
			var2[i] = 0;
		}
	}
	return longest;
}

/* Gets rid of any leading zeros that are present in the target longint */
void purge_leading_zeros(longint_t target) {
	int i = target[BUDDY_POS];
	/* Until the digit of the highest order of magnitude is not a zero, or
	   there is only 1 zero left, eraze a zero 
	*/
	while((target[i] == 0) && (i > 1)) {
		target[BUDDY_POS] -= 1;
		i--;
	}
}

/* Exits the program with a failure code. Prints a message dependant on the 
   reason passed
*/
void do_complain(int reason) {
	printf("oi oi oi mate wot's all this then? Have you got yer ");
	if (reason == DIVIDE_BY_ZERO) {  
		printf("dividing by zero ");
	} else if (reason == OVERFLOW) {
		printf("integers of order of magnitude greater than 500 ");
	}
	printf("loisence? Didn't think so! I'm gonna have to write you up!");
	exit(EXIT_FAILURE);
}

/* Update the indicated variable var1 by doing an multiplication
   using var2 to compute var1 = var1 * var2
*/
void do_multiply(longint_t var1, longint_t var2) {
	/* First better make sure that the variables dont have any junk stored. 
	   after their conclusion 
	*/
	equate_lengths(var1, var2);
	int var1_length = var1[BUDDY_POS], var2_length = var2[BUDDY_POS];
	int carry = 0;
	longint_t running_sum = LONG_ZERO, line_total = LONG_ZERO;
	
	/* for every order of magnitude in var2, multiply the entirity of 
	   var1 by it then add it to the running sum 
	*/
	for(int i = 1; i <= var2_length; i++) {
		int position = UNITS_COLUMN;
		carry = 0;
		line_total[BUDDY_POS] = var1_length;
		for(int j = 1; j <= var1_length; j++) {
			/* Integer overflow guard */
			if (position > INTSIZE) {
				do_complain(OVERFLOW);
			}
			/* First insert zeros based on the order of magnitude
			   of i
			*/
			while (position < i) {
				line_total[position] = 0;
				position++;
				expand_longint(line_total, DO_NOT_INSERT);
			}
			
			int product = var1[j] * var2[i] + carry;
			carry = 0;
			/* If the product is more than ten, we'll have to 
			   carry over a number to the next order of magnitude
			*/
			if (product >= INT_TEN) {
				carry = product / INT_TEN;
				product = product % 10;
				if (position == line_total[BUDDY_POS]) {
					expand_longint(line_total, carry);
				}
			}
			/* Finally add the product to the sub-total */
			line_total[position] = product;	
			position++;
		}
		/* And then add the sub total to the total */
		do_plus(running_sum, line_total);
	}
	purge_leading_zeros(running_sum);
	do_assign(var1, running_sum);
}

/* Raises var1 to the power of var2 through repeated multiplication */
void do_exponentiate(longint_t var1, longint_t var2) {
	int var2_length = var2[BUDDY_POS];
	int exponent = 0, exponent_subtotal = 0;

	/* We'll initilize the running total to be 1 so if we get a power of
	   zero, we don't need to do anything to it 
	*/
   	longint_t running_total = LONG_ONE, base;
    	do_assign(base, var1);

	if (var2_length > MAX_EXPONENT_LENGTH) {
		/* This check is necessary to prevent integer overflow in the 
		   loop directly below. Without it, inputing a number with a 
		   sufficient number of trailing zeros would cause it to read 
		   0 for the exponent. Hilarious!
		*/
		do_complain(OVERFLOW);
	}
	/* First we need to extract the integer value from the exponent 
	   longint
	*/
	for(int i = 1; i <= var2_length; i++) {
        	exponent_subtotal = var2[i];
        	for (int j = 1; j < i; j++) {
		    	exponent_subtotal *= 10;
        	}
        	exponent += exponent_subtotal;
	}

    	if (exponent > 0) {
		/* Multiply the total by the base i times, where i is the 
		   number in the exponent 
		*/
       		do_assign(running_total, base);
        	for (int i = 1; i < exponent; i++) {
            		do_multiply(running_total, base);
        	}
    	}
    	do_assign(var1, running_total); 
} 

/* Compares the values of 2 longints and returns 1 if the 1st is greater 
   than the second, 0 if it is less than and 2 if they are equal
*/
int longint_comparison(longint_t var1, longint_t var2) {
	int var1_length = var1[BUDDY_POS];
	int var2_length = var2[BUDDY_POS];
	if (var1_length > var2_length) {
		return 1;
	} else if (var1_length < var2_length) {
		return 0;
	}
	/* If we got to here both variables have the same length :( 
	   We'll have scan through the elements, until the tie is broken
	*/
	
	for (int i = var1_length; i >= 1; i--) {
		if (var1[i] > var2[i]) {
			return 1;
		} else if (var1[i] < var2[i]) {
			return 0;
		}
	}

	/* If we get here var1 == var2 */ 
	return 2;
}

/* Update the indicated variable var1 by doing an addition using var2 to 
   compute var1 = var1 - var2. the Subtraction function only works if 
   var1 > var2, if the subtraction fails, returns 0 and 1 otherwise 
*/
int do_minus (longint_t var1, longint_t var2) {

	int var_length = equate_lengths(var1, var2);
	/* If var2 is bigger we don't really care what the answer is, as it's
	   unhelpful to our division. knowing that var1 > var2 allows us to skip
	   a bunch of defensive code as well! hooray! 
	*/
	if (longint_comparison(var1, var2) == 0) {
		return 0;
	}
	var1[BUDDY_POS] = var_length;

	/* Starting from the units, subtract each digit of var2 from var1 */
	int borrow = 0;
	for (int i = 1; i <= var_length; i++) {
		int difference = var1[i] - var2[i] - borrow;
		borrow = 0;
		/* If the digit in var2 is greater than the digit in var1 
		   (after any borrowing that has occured) we'll need to 
		   "borrow" a 1 from the next order of magnitude.
		*/
		if (difference < 0) {
			difference += 10;
			borrow++;
		}
		var1[i] = difference;	
	}

	purge_leading_zeros(var1);
	return 1;
}

/* Update the indicated variable var1 by doing an integer division
   using var2 to compute var1 = var1 / var2, so long as var2 != 0
*/
void do_divide (longint_t var1, longint_t var2) {
	int var1_length = var1[BUDDY_POS];
	int minus_successes = 0;

	/* Initilise the minus candidate to be the digit in the highest order
	   of magnitude of the numerator 
	*/
	longint_t minus_candidate = {1, var1[var1_length]};
	longint_t carry_over = EMPTY_LONGINT;
	longint_t quotient = {var1_length, 0};
	longint_t ten = LONG_TEN;

	/* Check to see if the denominator is zero */
	if ((var2[BUDDY_POS] == INT_ONE) && (var2[UNITS_COLUMN] == INT_ZERO)) {
		do_complain(DIVIDE_BY_ZERO);
	}
	
	for (int i = var1_length; i >= 1; i--) {
		/* First we're going to subtract the divisor from the largest
		   remaining order of magnitude digit as much as possible 
		*/
		while (do_minus(minus_candidate, var2)) {
			minus_successes++;
		}
		/* Then we're going to want to carry over any remainder to the
		   next lowest order of magnitude. 
		*/
		do_assign(carry_over, minus_candidate);
		do_multiply(carry_over, ten);
		minus_candidate[UNITS_COLUMN] = var1[i - 1];
		minus_candidate[BUDDY_POS] = INT_ONE;
		do_plus(minus_candidate, carry_over);
		/* Now that the next order of manitude is set up as a longint,
		   we can add the number of successful subtractions to the 
		   quotient and repeat 
		*/
		quotient[i] = minus_successes;
		minus_successes = 0;
	}

	purge_leading_zeros(quotient);
	do_assign(var1, quotient);	
}

/* algorithms are fun */