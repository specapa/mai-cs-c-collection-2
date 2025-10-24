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
    marks_t *marks;
} student_t;

Z_VECTOR(student_t *, students)

u_status_t students_init2(students_t **students);
void student_free2(student_t *s);
void students_free2(students_t **students);

u_status_t students_load(students_t *students, const char *filepath);

students_t *students_find_by_id(const students_t *, size_t id);
students_t *students_find_by_name(const students_t *, const char *name);
students_t *students_find_by_surname(const students_t *, const char *surname);
students_t *students_find_by_group(const students_t *, const char *group);

void students_sort(students_t *, int (*cmp)(const void *, const void *));
int cmp_by_id(const void *, const void *);
int cmp_by_name(const void *, const void *);
int cmp_by_surname(const void *, const void *);
int cmp_by_group(const void *, const void *);

double student_avg(const student_t *);
double students_avg(const students_t *);

void trace_student(FILE *, const student_t *);
void trace_above_avg(FILE *, const students_t *);

#endif