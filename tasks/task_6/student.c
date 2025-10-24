#include "student.h"
#include <string.h>
#include <stdio.h>

int cmp_by_id(const void *a, const void *b) {
    const student_t *s1 = *(const student_t **)a;
    const student_t *s2 = *(const student_t **)b;
    return (s1->id > s2->id) - (s1->id < s2->id);
}

int cmp_by_surname(const void *a, const void *b) {
    const student_t *s1 = *(const student_t **)a;
    const student_t *s2 = *(const student_t **)b;
    return strcmp(s1->surname, s2->surname);
}

int cmp_by_name(const void *a, const void *b) {
    const student_t *s1 = *(const student_t **)a;
    const student_t *s2 = *(const student_t **)b;
    return strcmp(s1->name, s2->name);
}

int cmp_by_group(const void *a, const void *b) {
    const student_t *s1 = *(const student_t **)a;
    const student_t *s2 = *(const student_t **)b;
    return strcmp(s1->group, s2->group);
}

void students_sort(students_t *students, int (*cmp)(const void *, const void *)) {
    qsort(students->vec, students->size, sizeof(student_t *), cmp);
}

double student_avg(const student_t *s) {
    if (!s || !s->marks || s->marks->size == 0) return 0;
    double sum = 0;
    for (size_t i = 0; i < s->marks->size; ++i)
        sum += s->marks->vec[i];
    return sum / s->marks->size;
}

double students_avg(const students_t *students) {
    double total = 0;
    for (size_t i = 0; i < students->size; ++i)
        total += student_avg(students->vec[i]);
    return total / students->size;
}

u_status_t students_init2(students_t **students) {
    if (!students) return U_NEGATIVE_NOT_ALLOWED;

    *students = malloc(sizeof(students_t));
    if (!*students) return U_NEGATIVE_NOT_ALLOWED;

    (*students)->size = 0;
    (*students)->capacity = 0;
    (*students)->vec = NULL;
    return U_OK;
}

void student_free2(student_t *s) {
    if (!s) return;
    free(s->name);
    free(s->surname);
    free(s->group);
    marks_free(&s->marks);
    free(s);
}

void students_free2(students_t **students) {
    if (!students || !*students) return;
    for (size_t i = 0; i < (*students)->size; ++i)
        student_free2((*students)->vec[i]);
    free((*students)->vec);
    free(*students);
    *students = NULL;
}

u_status_t students_load(students_t *students, const char *filepath) {
    if (!students || !filepath) return U_INVALID_FORMAT;

    FILE *f = fopen(filepath, "r");
    if (!f) return U_NEGATIVE_NOT_ALLOWED;

    size_t id;
    char name[64], surname[64], group[64];
    unsigned char marks_arr[5];

    while (fscanf(f, "%zu %63s %63s %63s %hhu %hhu %hhu %hhu %hhu",
                  &id, name, surname, group,
                  &marks_arr[0], &marks_arr[1], &marks_arr[2],
                  &marks_arr[3], &marks_arr[4]) == 9) {
        student_t *s = malloc(sizeof(student_t));
        s->id = id;
        s->name = strdup(name);
        s->surname = strdup(surname);
        s->group = strdup(group);

        marks_init(&s->marks);
        for (int i = 0; i < 5; ++i)
            marks_push_back(s->marks, marks_arr[i]);

        students_push_back(students, s);
    }

    fclose(f);
    return U_OK;
}

static student_t* student_clone(const student_t* src) {
    if (!src) return NULL;
    
    student_t* clone = malloc(sizeof(student_t));
    if (!clone) return NULL;
    
    clone->id = src->id;
    clone->name = strdup(src->name);
    clone->surname = strdup(src->surname);
    clone->group = strdup(src->group);
    
    marks_init(&clone->marks);
    for (size_t i = 0; i < src->marks->size; ++i) {
        marks_push_back(clone->marks, src->marks->vec[i]);
    }
    
    return clone;
}

students_t *students_find_by_id(const students_t *students, size_t id) {
    if (!students) return NULL;

    students_t *res = NULL;
    if (students_init2(&res) != U_OK) return NULL;

    for (size_t i = 0; i < students->size; ++i) {
        const student_t *s = students->vec[i];
        if (s->id == id) {
            student_t *clone = student_clone(s);
            if (clone) {
                students_push_back(res, clone);
            }
        }
    }

    return res;
}

students_t *students_find_by_name(const students_t *students, const char *name) {
    if (!students || !name) return NULL;

    students_t *res = NULL;
    students_init(&res);

    for (size_t i = 0; i < students->size; ++i) {
        student_t *s = students->vec[i];
        if (strcmp(s->name, name) == 0)
            students_push_back(res, s);
    }

    return res;
}

students_t *students_find_by_surname(const students_t *students, const char *surname) {
    if (!students || !surname) return NULL;

    students_t *res = NULL;
    students_init(&res);

    for (size_t i = 0; i < students->size; ++i) {
        student_t *s = students->vec[i];
        if (strcmp(s->surname, surname) == 0)
            students_push_back(res, s);
    }

    return res;
}

students_t *students_find_by_group(const students_t *students, const char *group) {
    if (!students || !group) return NULL;

    students_t *res = NULL;
    students_init(&res);

    for (size_t i = 0; i < students->size; ++i) {
        student_t *s = students->vec[i];
        if (strcmp(s->group, group) == 0)
            students_push_back(res, s);
    }

    return res;
}

void trace_student(FILE *trace, const student_t *s) {
    fprintf(trace, "%s %s (%s): avg=%.2f\n",
            s->name, s->surname, s->group, student_avg(s));
}

void trace_above_avg(FILE *trace, const students_t *students) {
    double avg_all = students_avg(students);
    for (size_t i = 0; i < students->size; ++i) {
        student_t *s = students->vec[i];
        if (student_avg(s) > avg_all)
            fprintf(trace, "%s %s\n", s->surname, s->name);
    }
}