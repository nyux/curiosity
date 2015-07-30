#pragma once

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

#define new_operator_t(...) (operator_t) { __VA_ARGS__ }

bool should_quit(char *input);
operator_t get_operation(char *operator_str);
double calculate(char *str_to_parse);
double unroll_stack(void);


double add(double x, double y) ;
double subtract(double x, double y) ;
double multiply(double x, double y) ;
double divide(double x, double y) ;
