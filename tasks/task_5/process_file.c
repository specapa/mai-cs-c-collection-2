#include "process_file.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define NEED_TO_CROP 0

char *read_line(FILE *in) {
    size_t size = 128;
    size_t len = 0;
    char *buf = malloc(size);
    if (!buf) return NULL;

    int c;
    while (((c = fgetc(in)) != EOF) && (!NEED_TO_CROP || (len <= 80))) {
        if (len + 1 >= size) {
            size *= 2;
            char *tmp = realloc(buf, size);
            if (!tmp) { free(buf); return NULL; }
            buf = tmp;
        }
        buf[len++] = (char)c;
        if (c == '\n') break;
    }

    if (len == 0 && c == EOF) {
        free(buf);
        return NULL;
    }

    buf[len] = '\0';
    return buf;
}

static void justify_and_write(FILE *out, char **words, size_t count, size_t width) {
    if (count == 0) return;

    size_t total_len = 0;
    for (size_t i = 0; i < count; i++) {
        total_len += strlen(words[i]);
    }

    if (count == 1 || total_len >= width) {
        for (size_t i = 0; i < count; i++) {
            fputs(words[i], out);
            if (i + 1 < count)
                fputc(' ', out);
        }
        fputc('\n', out);
        return;
    }

    size_t gaps = count - 1;
    size_t total_spaces = width - total_len;

    if (total_spaces / gaps == 0) {
        for (size_t i = 0; i < count; i++) {
            fputs(words[i], out);
            if (i + 1 < count)
                fputc(' ', out);
        }
        fputc('\n', out);
        return;
    }

    size_t base = total_spaces / gaps;
    size_t extra = total_spaces % gaps;

    for (size_t i = 0; i < count; i++) {
        fputs(words[i], out);
        if (i + 1 < count) {
            size_t n = base + (extra > 0 ? 1 : 0);
            if (extra > 0) extra--;
            for (size_t j = 0; j < n; j++) fputc(' ', out);
        }
    }
    fputc('\n', out);
}

static char **split_words(const char *line, size_t *out_count) {
    *out_count = 0;
    char **words = NULL;

    const char *p = line;
    while (*p) {
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;

        const char *start = p;
        while (*p && !isspace((unsigned char)*p)) p++;

        size_t len = p - start;

        char *word = malloc(len + 1);
        if (!word) {
            for (size_t i = 0; i < *out_count; i++)
                free(words[i]);
            free(words);
            return NULL;
        }

        memcpy(word, start, len);
        word[len] = '\0';

        char **tmp = realloc(words, (*out_count + 1) * sizeof(char *));
        if (!tmp) {
            free(word);
            for (size_t i = 0; i < *out_count; i++)
                free(words[i]);
            free(words);
            return NULL;
        }

        words = tmp;
        words[*out_count] = word;
        (*out_count)++;
    }

    return words;
}

u_status_t process_file(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "r");
    if (!in) {
        fprintf(stderr, "ERROR at input file %s opening: ", input_file);
        return U_INVALID_FORMAT;
    }

    FILE *out = fopen(output_file, "w");
    if (!out) {
        fprintf(stderr, "ERROR at output file %s opening: ", output_file);
        fclose(in);
        return U_INVALID_FORMAT;
    }

    char *line;
    const size_t WIDTH = 80;

    while ((line = read_line(in)) != NULL) {
        char *trimmed = line;
        while (isspace((unsigned char)*trimmed)) trimmed++;

        if (strlen(trimmed) <= 80) {
            fprintf(out, "%s", trimmed);
            free(line);
            continue;
        }

        size_t word_count;
        char **words = split_words(trimmed, &word_count);
        free(line);

        if (word_count == 0) {
            fputc('\n', out);
            free(words);
            continue;
        }

        size_t i = 0;
        while (i < word_count) {
            size_t current_len = 0;
            size_t start = i;
            while (i < word_count) {
                size_t next_len = current_len + strlen(words[i]) + (i > start ? 1 : 0);
                if (next_len > WIDTH) break;
                current_len = next_len;
                i++;
            }
            justify_and_write(out, &words[start], i - start, WIDTH);
        }

        for (size_t j = 0; j < word_count; j++)
            free(words[j]);
        free(words);
    }

    fclose(in);
    fclose(out);
    return U_OK;
}
