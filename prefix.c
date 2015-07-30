#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <editline/readline.h>

bool should_quit(char *input) {
    return!strcmp(input, "q") || !strcmp(input, "quit") || !strcmp(input, "Q");
}

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
