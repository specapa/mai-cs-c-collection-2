#ifndef PARSER_H
#define PARSER_H

#include "../../include/utils_2.h"
#include "vectors.h"

u_status_t parse_arg_vector(int argc, char *argv[], usgshrt_vec_vec_t *vec, unsigned long long *source_base, unsigned long long *target_base, unsigned long long *max_deep);
u_status_t parse_number_to_vector(char *string, usgshrt_vec_t **vec);

#endif // PARSER_H