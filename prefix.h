#pragma once

struct node 
{
    struct node *next;
    double value;
};

struct queue
{
    struct node *head;
    struct node *tail;
};

struct operator 
{
    bool is_valid;
    int num_operands; /* if invalid, -1 */
    double (*fn)(double x, double y); /* if invalid, NULL */
};

struct subexpression
{
    char *subexpr;
    char *end_of_subexpr;
};

typedef struct node node_t;
typedef struct queue queue_t;
typedef struct operator operator_t;
typedef struct subexpression subexpression_t;

#define new_operator_t(...) (operator_t) { __VA_ARGS__ }
#define new_subexpression_t(...) (subexpression_t) { __VA_ARGS__ }

bool should_quit(char *input);
operator_t get_operator(char *operator_str);
double calculate(char *str_to_parse);
double unroll_queue(queue_t *queue, operator_t operator);
subexpression_t get_subexpression(char *subexpression);
queue_t* create_queue(void);
node_t* create_node(node_t *next, double value);
void push(queue_t *queue, double value);
double pop(queue_t *queue);

double add(double x, double y) ;
double subtract(double x, double y) ;
double multiply(double x, double y) ;
double divide(double x, double y) ;
