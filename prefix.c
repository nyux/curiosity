#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <editline/readline.h>

/* current ideas of what data structures i need:
 * a node_t for the linked list that is the stack
 * a value_t for whether a token is a double or malformed
 * an operator_t for what an operator is, the function that defines it, and
 *      how many operands it takes
 */

struct node {
    double value;
    struct node *next;
};

struct value {
    bool is_valid;
    double value; /* if invalid input, NAN */
};

struct operator {
    bool is_valid;
    int num_operands; /* if invalid, -1 */
    double (*fn)(double x, double y); /* if invalid, NULL */
};

typedef struct node node_t;
typedef struct value value_t ;
typedef struct operator operator_t;

bool should_quit(char *input) {
    return!strcmp(input, "q") || !strcmp(input, "quit") || !strcmp(input, "Q");
}

/* current plan:
 *  - parse the operator
 *  - skip ahead to the next token
 *  - test if is a parenthesis
 *      - if so, duplicate the string up until the end of the closing paren
 *      - run calculate again
 *  - if is a number, add to stack
 *  - once you reach the end of the string, do calculation and return result
 */
double calculate(char *str_to_parse) {
    char *saveptr;
    char *operation_str = strtok_r(str_to_parse, " ", &saveptr);
    char *token;

    operator_t operation = get_operation(operation_str);

    if (!operation.is_valid) {
        free(str_to_parse);
        puts("invalid input");
        exit(-1);
    }

    while ((token = strtok_r(NULL, " ", &saveptr))) {
        break;
    }

    unroll_stack(/* blah blah blah */);


    return 0.0;
}

int main(void) {
    char *input = "";

    while (true) {
        input = readline("calc> ");

        if (should_quit(input)) {
            free(input);
            break;
        }

        add_history(input);
        printf("%lf\n", calculate(input));
        free(input);
    }
} 
