#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <editline/readline.h>

/* current ideas of what data structures i need:
 * a node_t for the linked list that is the stack
 * a value_t for whether a token is a double or malformed
 */

struct node {
    double value;
    struct node *next;
};

struct value {
    bool is_valid;
    double value; /* if invalid input, NAN */
};

typedef struct node node_t;
typedef struct value value_t ;

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
