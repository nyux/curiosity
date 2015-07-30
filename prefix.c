#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <editline/readline.h>

#include "prefix.h"

/* TODO: the functions should be organized more clearly */


bool should_quit(char *input)
{
    return!strcmp(input, "q") || !strcmp(input, "quit") || !strcmp(input, "Q");
}

/* num_operands is an attempt at building scaffolding for the future, if i
 * want to expand the calculator into using functions that take only one
 * operand (like factorial or exp), or more than two operands. for the moment,
 * though, i'm not touching that. */
operator_t get_operator(char *operator_str)
{
    if (strcmp(operator_str, "+") == 0)
    {
        return new_operator_t(.fn = add, .num_operands = 2, .is_valid = true);
    }
    else if (strcmp(operator_str, "-") == 0)
    {
        return new_operator_t(.fn = subtract, .num_operands = 2, .is_valid = true);
    }
    else if (strcmp(operator_str, "*") == 0)
    {
        return new_operator_t(.fn = multiply, .num_operands = 2, .is_valid = true);
    }
    else if (strcmp(operator_str, "/") == 0)
    {
        return new_operator_t(.fn = divide, .num_operands = 2, .is_valid = true);
    }
    else return new_operator_t(.is_valid = false);
}

double add(double x, double y) { return x + y; }
double subtract(double x, double y) { return x - y; }
double multiply(double x, double y) { return x * y; }
double divide(double x, double y) { return x / y; }

double unroll_queue(queue_t *queue, operator_t operator) 
{
    /* assumption: the queue will have at least two items in it */
    double x = pop(queue), y, temp;
    do
    {
        y = pop(queue);
        temp = (*operator.fn)(x, y);
        x = temp;
    } while (queue->head);

    free(queue);
    return x;
}

subexpression_t get_subexpression(char *str)
{
    int level = 1;
    char *str_copy = str;
    char *subexpression;

    str_copy++; /* move the pointer away from the leading '(' */

    while (level > 0)
    {
        if (*str_copy == '(') level++;
        else if (*str_copy == ')') level--;
        str_copy++;
    }

    /* assumes the character after the final ) is a space */
    *str_copy = '\0';
    subexpression = strdup(str);
    *str_copy = ' ';

    return new_subexpression_t(.subexpr = subexpression, .end_of_subexpr = str_copy);
}

queue_t* create_queue()
{
    queue_t *queue = malloc(sizeof(queue_t));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

node_t* create_node(node_t *next, double value)
{
    node_t *node = malloc(sizeof(node_t));
    node->next = next;
    node->value = value;
    return node;
}

void push(queue_t *queue, double value)
{
    node_t *temp = create_node(NULL, value);
    
    if (!queue->head) queue->head = temp;
   
    if (queue->tail) queue->tail->next = temp;
    else queue->tail = temp;
  
    queue->tail = temp;
}

double pop(queue_t *queue)
{
    node_t *current_head = queue->head;
    double value = current_head->value;

    queue->head = current_head->next;
    free(current_head);

    return value;
}

/* general approach this takes:
 *  - parse the operator
 *  - skip ahead to the next token
 *  - test if is a parenthesis
 *      - if so, duplicate the string up until the end of the closing paren
 *      - run calculate again
 *  - if is a number, add to stack
 *  - once you reach the end of the string, do calculation and return result
 */
double calculate(char *expression)
{
    if (*expression == '(') expression++;
    
    char *saveptr;
    char *token = strtok_r(expression, " ", &saveptr);
    queue_t *queue = create_queue();

    operator_t operator = get_operator(token);

    if (!operator.is_valid)
    {
        free(saveptr);
        free(queue);
        puts("invalid input; terminating program");
        exit(-1);
    }

    while ((token = strtok_r(NULL, " ", &saveptr)))
    {
        begin:
        if (token == NULL) break;
        else if (*token == '(')
        {
            token++;
            char *undo_nullterminator = token;
            do { undo_nullterminator++; } while (*undo_nullterminator);
            *undo_nullterminator = ' ';

            subexpression_t subexpression = get_subexpression(token);
            push(queue, calculate(subexpression.subexpr));

            expression = subexpression.end_of_subexpr;
            saveptr = NULL;
            
            free(subexpression.subexpr);

            token = strtok_r(expression, " ", &saveptr);
            goto begin;
        }
        else
        {
            char *end;
            double value = strtod(token, &end);

            if (value == 0.0 && end == token)
            {
                puts("invalid input, exiting program");
                exit(-1);
            }
            else push(queue, value);

            if (*end == ')') break;
        }
    }

    return unroll_queue(queue, operator);
}

int main(void)
{
    char *input = "";

    puts("This program's a bit fragile, and only does okay with well-formatted input.");

    while (true)
    {
        input = readline("calc> ");

        if (!input)
        {
            puts("");
            break;
        }
        else if (should_quit(input))
        {
            free(input);
            break;
        }

        add_history(input);
        printf("%.3lf\n", calculate(input));
        free(input);
    }
} 
