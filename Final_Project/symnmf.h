#ifndef SYM_NMF_H
#define SYM_NMF_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Macro to check if a matrix is NULL */
#define CHECK_MATRIX(mat) \
    do { \
        if ((mat) == NULL) { \
            fprintf(stderr, "Error: Matrix is NULL\n"); \
            exit(1); \
        } \
    } while (0)

#define ERROR_MSG(id) fprintf(stderr, "Error %d: An Error Has Occurred! Line: %d\n", id, __LINE__)


/* Function declarations from symnmf.c */
int compute_d(FILE* fp);
int compute_n(FILE* fp);
double** compute_data_matrix(FILE* file, int n, int d);
double** create_matrix(int rows, int columns);
void free_matrix(double** matrix, int n);
void print_matrix(double** matrix, int rows, int columns);
double vector_distance(double *x, double *y, int d);
double** sym(double** mat, int n, int d);
double** ddg(double** A, int n);
double** norm(double** D, double** A, int n);
double** transpose_matrix(double** mat, int rows, int cols);
double** multiply_matrices(double** A, double** B, int rows_A, int cols_A, int rows_B, int cols_B);
double** update_H(double** H, double** W, int n, int k);
double frobenius_norm(double** new_H, double** H, int n, int k);
double** optimize_H(double** H, double** W, int n, int k);
double** compute_goals(double **data_matrix, const char *goal, int n, int d);

#endif 
