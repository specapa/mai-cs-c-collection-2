#ifndef U_VECTOR_H
#define U_VECTOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "utils_2.h"

#define VECTOR_INIT_CAPACITY 8

#define Z_VECTOR(dtype, dname)                                                                \
                                                                                              \
typedef struct dname##_ {                                                                     \
    size_t size;                                                                              \
    size_t capacity;                                                                          \
    dtype *vec;                                                                               \
} dname##_t;                                                                                  \
                                                                                              \
static inline u_status_t dname##_init(dname##_t **dv) {                                       \
    dname##_t *temp = (dname##_t *)malloc(sizeof(dname##_t));                                 \
    if (temp == NULL) {                                                                       \
        return U_MEMORY_ALLOCATE_ERROR;                                                       \
    }                                                                                         \
    temp->size = 0;                                                                           \
    temp->capacity = VECTOR_INIT_CAPACITY;                                                    \
    temp->vec = (dtype *)calloc(temp->capacity, sizeof(dtype));                               \
    if (temp->vec == NULL) {                                                                  \
        free(temp);                                                                           \
        return U_MEMORY_ALLOCATE_ERROR;                                                       \
    }                                                                                         \
    *dv = temp;                                                                               \
    return U_OK;                                                                              \
}                                                                                             \
                                                                                              \
static inline u_status_t dname##_push_back(dname##_t *dv, dtype value) {                      \
    if (dv->size >= dv->capacity) {                                                           \
        size_t new_capacity = (dv->capacity == 0) ? VECTOR_INIT_CAPACITY : dv->capacity * 2;  \
        dtype *temp = (dtype *)realloc(dv->vec, sizeof(dtype) * new_capacity);                \
        if (temp == NULL) {                                                                   \
            return U_MEMORY_REALLOCATE_ERROR;                                                 \
        }                                                                                     \
        dv->vec = temp;                                                                       \
        dv->capacity = new_capacity;                                                          \
    }                                                                                         \
    dv->vec[dv->size++] = value;                                                              \
    return U_OK;                                                                              \
}                                                                                             \
                                                                                              \
static inline u_status_t dname##_free(dname##_t **dv) {                                       \
    if (dv == NULL || *dv == NULL) return U_OK;                                               \
    free((*dv)->vec);                                                                         \
    free(*dv);                                                                                \
    *dv = NULL;                                                                               \
    return U_OK;                                                                              \
}                                                                                             \
                                                                                              \

#endif // U_VECTOR_H