#ifndef PARSER_H
#define PARSER_H

#include "../../include/utils_2.h"
#include "vectors.h"

u_status_t parse_arg_vector(int argc, char *argv[], double_vec_t *dv, unsigned long long *base);

#endif // PARSER_H