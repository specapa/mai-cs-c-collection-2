#ifndef STUDENT_H
#define STUDENT_H

#include "stdlib.h"
#include "../../include/u_vector.h"

Z_VECTOR(unsigned char, marks)

typedef struct student_ {
    size_t id;
    char *name;
    char *surname;
    char *group;
    marks_t *makrs;
} student_t;

Z_VECTOR(student_t *, students)

#endif