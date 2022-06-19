#ifndef _FUNCS_H_
#define _FUNCS_H_

#include <stdlib.h>
#include <stdarg.h>

int *map(int (*fun)(int), int *arr, int len) {
    int *b = (int*)malloc(len * sizeof(int));
    for (int i = 0; i < len; ++i) {
        b[i] = fun(arr[i]);
    }
    return b;
}

int *filter(int (*fun)(int), int *arr, int len, int *outlen) {
    int *b = (int *)malloc(len * sizeof(int));
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (fun(arr[i])) {
            b[j++] = arr[i];
        }
    }
    b = (int*)realloc(b, j * sizeof(int));
    *outlen = j;
    return b;
}

int reduce(int (*fun)(int, int), int *arr, int len) {
    int *b = (int *)malloc(len * sizeof(int));
    b[0] = arr[0];
    for (int i = 1; i < len; ++i) {
        b[i] = fun(b[i - 1], arr[i]);
    }
    int x = b[len - 1];
    free(b);
    return x;
}

int **zip(int n, ...) {
    va_list ap;
    va_start(ap, n);
    va_arg(ap, int*);
    int smallest_array = va_arg(ap, int);
    for (int i = 1; i < n; ++i) {
        va_arg(ap, int*); /* skip array argument */
        int array_size = va_arg(ap, int);
        if (array_size < smallest_array) {
            smallest_array = array_size;
        }
    }
    va_end(ap);

    va_start(ap, n);
    int **arrays = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        arrays[i] = (int*)malloc(smallest_array * sizeof(int));
        int *array = va_arg(ap, int*);
        for (int j = 0; j < smallest_array; ++j) {
            arrays[i][j] = array[j];
        }
        va_arg(ap, int);
    }
    va_end(ap);
    int **tuples = (int**)malloc(smallest_array * sizeof(int*));
    for (int i = 0; i < smallest_array; ++i) {
        tuples[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            tuples[i][j] = arrays[j][i];
        }
    }
    for (int i = 0; i < n; ++i) {
        free(arrays[i]);
    }
    free(arrays);
    return tuples;
}

int *chain(int n, int *outlen, ...) {
    va_list ap;
    va_start(ap, outlen);
    int *arr = va_arg(ap, int*);
    int len = va_arg(ap, int);
    int total = len;
    int *b = (int *)malloc(len * sizeof(int));
    for (int i = 0; i < len; ++i) {
        b[i] = arr[i];
    }
    for (int i = 1; i < n; ++i) {
        arr = va_arg(ap, int*);
        len = va_arg(ap, int);
        b = (int *)realloc(b, (total += len) * sizeof(int));
        for (int j = 0; j < len; ++j) {
            b[total - len + j] = arr[j];
        }
    }
    va_end(ap);
    *outlen = total;
    return b;
}

int *accumulate(int (*fun)(int, int), int *arr, int len) {
    int *b = (int *)malloc(len * sizeof(int));
    b[0] = arr[0];
    for (int i = 1; i < len; ++i) {
        b[i] = fun(b[i - 1], arr[i]);
    }
    return b;
}

int *compress(int *arr, int len, int *selectors, int *outlen) {
    int *b = (int *)malloc(len * sizeof(int));
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (selectors[i]) {
            b[j++] = arr[i];
        }
    }
    b = (int*)realloc(b, j * sizeof(int));
    *outlen = j;
    return b;
}

int *repeat(int elem, int n) {
    int *b = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        b[i] = elem;
    }
    return b;
}

int *filterfalse(int (*fun)(int), int *arr, int len, int *outlen) {
    int *b = (int *)malloc(len * sizeof(int));
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (!fun(arr[i])) {
            b[j++] = arr[i];
        }
    }
    b = (int*)realloc(b, j * sizeof(int));
    *outlen = j;
    return b;
}

int *dropwhile(int (*fun)(int), int *arr, int len, int *outlen) {
    int *b = (int *)malloc(len * sizeof(int));
    int failed = 0;
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (!fun(arr[i])) failed = 1;
        if (failed) b[j++] = arr[i];
    }
    b = (int*)realloc(b, j * sizeof(int));
    *outlen = j;
    return b;
}

int *takewhile(int (*fun)(int), int *arr, int len, int *outlen) {
    int *b = (int *)malloc(len * sizeof(int));
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (fun(arr[i])) b[j++] = arr[i];
        else break;
    }
    b = (int*)realloc(b, j * sizeof(int));
    *outlen = j;
    return b;
}

int **pairwise(int *arr, int len) {
    int **tuples = (int**)malloc((len - 1) * sizeof(int*));
    for (int i = 0; i < len - 1; ++i) {
        tuples[i] = (int*)malloc(2 * sizeof(int));
        tuples[i][0] = arr[i];
        tuples[i][1] = arr[i + 1];
    }
    return tuples;
}

int *slice(int *arr, int len, int start, int stop, int step, int *outlen) {
    int *b = (int *)malloc(((stop - start) / step) * sizeof(int));
    int j = 0;
    for (int i = start; i < stop; i += step) {
        b[j++] = arr[i];
    }
    b = (int*)realloc(b, j * sizeof(int));
    *outlen = j;
    return b;
}

#endif