#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "symnmf.h"

/**
 * @brief Calculating d the vector size
 * 
 * @param file set of n datapoints in .txt file format
 * @return int d The vector size
 */
int compute_d(FILE* fp){
    int d = 0;
    char ch;
    while((ch = fgetc(fp)) != '\n'){
       if (ch == ',') d++; 
    }
    rewind(fp);
    return d + 1; 
}

/**
 * @brief Calculating n the number of lines
 * 
 * @param file Set of n datapoints in .txt file format
 * @return int n The number of lines
 */
int compute_n(FILE* fp){
    int n = 0;
    char line[255];
     while (fgets(line, sizeof(line), fp) != NULL) {
        n++;
    }
    rewind(fp);
    return n;
}

/**
 * @brief Reads a .txt file and stores datapoints in a matrix
 * 
 * @param file Set of n datapoints in .txt file format
 * @param n Number of rows
 * @param d Number of columns
 * @return double** mat A pointer to the allocated matrix
 */
double** compute_data_matrix(FILE* file, int n, int d){
    int i,j, buffer_index, k;
    double** mat = create_matrix(n, d);
    char line[255];
    char buffer[50];
    char ch;

    i = 0;
    while (fgets(line, sizeof(line), file) != NULL){
        buffer_index = 0;
        j = 0;
        for (k = 0; line[k] != '\0'; k++){
            ch = line[k];
            if (ch == ','){
                buffer[buffer_index] = '\0';
                mat[i][j++] = atof(buffer);
                buffer_index = 0;
            } else {
                buffer[buffer_index++] = ch;
            }
        }
        if (buffer_index > 0){
            buffer[buffer_index] = '\0';
            mat[i][j++] = atof(buffer);
        }
        i++;
    }
    return mat;
}

/**
 * @brief Checks if a pointer is NULL, indicating allocation failure.
 * 
 * @param mat Matrix pointer to check.
 * @return int 1 if NULL (error), 0 otherwise.
 */

int check_pointer(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "%s\n", ERROR_MESSAGE);
        return 1;
    }
    return 0;
}

/**
 * @brief Allocates memory for a matrix and returns a pointer to it
 * 
 * @param rows Number of rows in matrix
 * @param columns The number of columns in matrix
 * @return double** matrix A pointer to the allocated matrix
 */
double** create_matrix(int rows, int columns){
    int i;
    double **matrix = malloc(sizeof(double*) * rows);
    if (check_pointer(matrix)) exit(1);
    for (i=0; i<rows; i++){
        matrix[i] = calloc(columns, sizeof(double));
        if (check_pointer(matrix[i])){
            free_matrix(matrix, i);
            exit(1);
        }
    }
    return matrix;
}


/**
 * @brief Free the allocated memory of a given matrix
 * 
 * @param matrix A matrix
 * @param n Number of rows in matrix
 */
void free_matrix(double** matrix, int n){
    int i;
    for (i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    
}

/**
 * @brief Gets a matrix and prints it
 * 
 * @param matrix A matrix
 * @param rows Number of rows in matrix
 * @param columns Number of columns in matrix
 */
void print_matrix(double** matrix, int rows, int columns){
    int i, j;
    if (!check_pointer(matrix)) exit(1);
    for(i=0; i<rows; i++){
        for(j=0;j<columns;j++){
            if(j<columns-1)
                printf("%.4f,", matrix[i][j]);
            else
                printf("%.4f", matrix[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief Calculates the euclidean distance between two vectors of the same dimension
 * 
 * @param x First vector
 * @param y Second vector
 * @param d The dimension of the vectors
 * @return double res The euclidean distance between the two given vectors
 */
double vector_distance(double *x, double *y, int d){
    double res;
    int i;
    res = 0;
    for(i = 0; i < d; i++){
        res += pow((x[i]-y[i]), 2);
    }
    res = pow(res, 0.5);
    return res;
}

/**
 * @brief Gets a matrix and calculates the Similarity Matrix
 * 
 * @param mat Set of n datapoints
 * @param n Number of rows in the matrix
 * @param d Number of columns in the matrix
 * @return double** A The similarity matrix (sym)
 */
double** sym(double** mat, int n, int d){
    double** A = create_matrix(n,n);
    int i,j;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            if (i!=j) { A[i][j] = exp(-(pow(vector_distance(mat[i],mat[j],d),2))/2); }
            else { A[i][j] = 0; }
        }
    }
    return A;
}

/**
 * @brief Calculates the diagonal dregree matrix 
 * 
 * @param A The Similarity Matrix
 * @param n Number of rows in matrix
 * @return double** D the Diagonal Degree Matrix (ddg)
 */
double** ddg(double** A, int n){
    double** D = create_matrix(n,n);
    int i,j;
    for (i=0; i<n; i++){
        D[i][i] = 0.0;
        for (j=0; j<n; j++){
            D[i][i] += A[i][j];
        }
    }
    return D;
}

/**
 * @brief Gets a diagonal degree matrix and normalizing it
 * 
 * @param D Diagonal degree matrix
 * @param A similarity matrix
 * @param n Number of rows in matrix
 * @return double** W The Normalized Similarity Matrix (norm)
 */
double** norm(double** D, double** A, int n){
    double** W = create_matrix(n,n);
    double* D_inv_sqrt = malloc(n * sizeof(double));
    int i, j;

    if (check_pointer(D_inv_sqrt)) {
        free_matrix(W, n);
        return NULL;
    }
    for (i=0; i<n; i++){
        if (D[i][i]!=0) { D_inv_sqrt[i] = (1.0 / sqrt(D[i][i])); }
        else { D_inv_sqrt[i] = 0; }
    }

    for (i=0; i<n; i++){
        for (j=0; j<n; j++){
                W[i][j] = D_inv_sqrt[i] * A[i][j] * D_inv_sqrt[j];
        }
    }
    
    free(D_inv_sqrt);
    return W;
}

/**
 * @brief Gets a matrix and transposes it
 * 
 * @param mat A matrix
 * @param rows Number of rows in matrix
 * @param cols Number of columns in matrix
 * @return double** trans_mat The tranposed matrix
 */
double** transpose_matrix(double** mat, int rows, int cols){
    double** trans_mat = create_matrix(cols, rows);
    int i,j;
    for (i=0; i<rows; i++){
        for (j=0; j<cols; j++){
            trans_mat[j][i] = mat[i][j];
        }
    }
    return trans_mat;
}

/**
 * @brief Gets two matrices and multiplies it
 * 
 * @param A First matrix
 * @param B Second matrix
 * @param rows_A Number of rows in A
 * @param cols_A Number of columns in A
 * @param rows_B Number of rows in B
 * @param cols_B Number of columns in B
 * @return double** product_mat Result of A * B
 */
double** multiply_matrices(double** A, double** B, int rows_A, int cols_A, int rows_B, int cols_B){
    double** product_mat;
    int i,j,k;
    if (cols_A != rows_B){
        fprintf(stderr, "%s\n", ERROR_MESSAGE);
        exit(1);
    }
    product_mat = create_matrix(rows_A, cols_B);
    for(i=0;i<rows_A;i++){
        for (j=0;j<cols_B;j++){
            product_mat[i][j] = 0; 
            for (k=0; k<cols_A; k++){
                product_mat[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return product_mat;
}

/**
 * @brief Gets the initialized matrix H and the normalized similarity matrix W and updates H
 * 
 * @param H Initialized decomoposition matrix
 * @param W Normalized similarity matrix
 * @param n Number of rows in H & number of rows and columns in W
 * @param k Number of columns in H
 * @return double** new_H The updated matrix
 */
double** update_H(double** H, double** W, int n, int k){
    double** WxH = multiply_matrices(W,H,n,n,n,k); 
    double** Ht = transpose_matrix(H,n,k); 
    double** HxHt = multiply_matrices(H,Ht,n,k,k,n); 
    double** HHH = multiply_matrices(HxHt,H,n,n,n,k); 
    double** new_H = create_matrix(n,k); 
    int i,j;
    for (i=0; i<n; i++){
        for (j=0; j<k; j++){
            new_H[i][j] = H[i][j] * (0.5 + 0.5*(WxH[i][j] / HHH[i][j]));
        }
    }
    free_matrix(WxH,n);
    free_matrix(Ht,k);
    free_matrix(HxHt,n);
    free_matrix(HHH,n);
    return new_H;
}

/**
 * @brief Gets two matrices A and B and calculates the difference A-B
 * 
 * @param A First matrix (minuend)
 * @param B Second matrix (subtahend)
 * @param rows_A Number of rows in matrix A
 * @param cols_A Number of columns in matrix A
 * @param rows_B Number of rows in matrix B
 * @param cols_B Number of columns in matrix B
 * @return double** diff_mat The difference
 */
double** subtract_matrices(double** A, double** B, int rows_A, int cols_A, int rows_B, int cols_B){
    int i, j;
    double** diff_mat;
    if(rows_A != rows_B || cols_A != cols_B || A == NULL || B == NULL){
        fprintf(stderr, "%s\n", ERROR_MESSAGE);
        exit(1);
    }
    diff_mat = create_matrix(rows_A, cols_A);
    for (i=0; i<rows_A; i++){
        for (j=0; j<cols_A; j++){
            diff_mat[i][j] = A[i][j] - B[i][j];
        }
    }
    return diff_mat;
}

/**
 * @brief Gets two matrices, calculates the frobenius norm of their difference
 * 
 * @param new_H Updated H matrix 
 * @param H Old H matrix 
 * @param n Number of rows in both matrices
 * @param k Number of columns in both matrices
 * @return double sqrt(norm) The frobenius norm
 */
double frobenius_norm(double** new_H, double** H, int n, int k){
    int i,j;
    double** diff_mat = subtract_matrices(new_H,H,n,k,n,k);
    double norm = 0;
    for (i=0; i<n; i++){
        for (j=0; j<k; j++){
            norm += pow(diff_mat[i][j],2);
        }
    }
    free_matrix(diff_mat, n);
    return sqrt(norm);
}

/**
 * @brief Gets matrix H and matrix W and update H until convergence (or until max iteration number is reached)
 * 
 * @param H Initialized decomoposition matrix
 * @param W Normalized similarity matrix
 * @param n Number of rows in H, and the number of rows and columns in W
 * @param k Number of columns in H
 * @return double** H Updated matrix
 */
double** optimize_H(double** H, double** W, int n, int k){
    int iter = 0;
    double** new_H;
    while (iter < MAX_ITER){
        new_H = update_H(H, W, n, k);
        if (new_H == NULL){
            return NULL;
        }
        if (pow(frobenius_norm(new_H, H, n, k),2) < EPSILON){
            free_matrix(H,n); 
            return new_H; 
        } 
        free_matrix(H,n);
        H = new_H;
        iter++;
    }
    return new_H;
}

/**
 * @brief Gets a data matrix, goal, n and d and returns the desired matrix based on the goal
 * 
 * @param data_matrix a matrix with n datapoints of size d
 * @param goal The type of matrix to be calculated
 * @param n Number of rows
 * @param d Number of columns
 * @return double** The desired matrix based on the given goal
 */
double** compute_goals(double **data_matrix, const char *goal, int n, int d) {
    double **A, **W, **D; 
    
    A = sym(data_matrix, n, d);

    if (strcmp(goal, "sym") == 0) {
        return A;
    }

    D = ddg(A, n);

    if (strcmp(goal, "ddg") == 0) {
        free_matrix(A, n);
        return D;
    }

    W = norm(D, A, n);

    free_matrix(A, n), free_matrix(D, n);
    if (strcmp(goal, "norm") != 0){
        free_matrix(W, n);
        return NULL;
    }
    return W; 
}



int main(int argc, char** argv){
    char *goal, *file_name;
    int d, n;
    double **result_matrix, **data_matrix;
    FILE *file;
    if (argc != 3){
        fprintf(stderr, "An Error Has Occured\n Invalid number of arguments");
        return 1;
    }
    goal = argv[1];
    file_name = argv[2];

    file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "An Error Has Occurred (File Reading Error)\n");
        return 1;
    }

    d = compute_d(file), n = compute_n(file);
    data_matrix = compute_data_matrix(file,n,d);
    fclose(file);

    result_matrix = compute_goals(data_matrix, goal, n, d);
    if(check_pointer(result_matrix)){
        free_matrix(data_matrix,n);
        return 1;
    }

    print_matrix(result_matrix, n, n);
    free_matrix(result_matrix, n), free_matrix(data_matrix, n);
    return 0;
}
