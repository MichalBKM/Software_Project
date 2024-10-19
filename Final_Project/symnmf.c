#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int compute_d(FILE* file);
int compute_n(FILE* file);
double** compute_data_matrix(FILE* file, int n, int d);
double** create_matrix(int rows, int columns);
void free_matrix(double** matrix, int n);
void print_matrix(double** matrix, int rows, int columns);
double vector_distance(double *x, double *y, int d);
double** sym(double** mat, int n, int d);
double** ddg(double** A, int n);
double** norm(double** D, double** A, int n);

/**
 * @brief calculating d the vector size
 * 
 * @param file set of n datapoints in .txt file format
 * @return int d The vector size
 */
int compute_d(FILE* file){
    int d = 0;
    char ch;
    while((ch = fgetc(file)) != '\n'){
       if (ch == ',') d++; 
    }
    rewind(file);
    return d + 1; 
}

/**
 * @brief Calculating n the number of lines
 * 
 * @param file Set of n datapoints in .txt file format
 * @return int n The number of lines
 */
int compute_n(FILE* file){
    int n = 0;
    char line[255];
     while (fgets(line, sizeof(line), file) != NULL) {
        n++;
    }
    rewind(file);
    return n;
}

/**
 * @brief Reads a .txt file and stores datapoints in a matrix
 * 
 * @param file Set of n datapoints in .txt file format
 * @param n The number of rows
 * @param d The number of columns
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
 * @brief Allocates memory for a matrix and returns a pointer to it
 * 
 * @param rows The number of rows in the matrix
 * @param columns The number of columns in the matrix
 * @return double** matrix A pointer to the allocated matrix
 */
double** create_matrix(int rows, int columns){
    int i;
    double **matrix = malloc(sizeof(double*) * rows);
    if (matrix == NULL){
        fprintf(stderr, "An Error Has Occurred\n");
        exit(1);
    }
    for (i=0; i<rows; i++){
        matrix[i] = malloc(sizeof(double) * columns);
        if (matrix[i] == NULL){
            fprintf(stderr, "An Error Has Occurred\n");
            exit(1);
        }
    }
    return matrix;
}

/**
 * @brief Free the allocated memory of a given matrix
 * 
 * @param matrix A 2d array
 * @param n The number of lines in the matrix
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
 * @param matrix A 2d array
 * @param rows The number of rows in the matrix
 * @param columns The number of colums in the matrix
 */
void print_matrix(double** matrix, int rows, int columns){
    int i, j;
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
 * @param x The first vector
 * @param y The second vector
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
 * @param n The number of lines in the matrix
 * @param d The number of rows in the matrix
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
 * @param n The number of lines in the matrix
 * @return double** D the Diagonal Degree Matrix (ddg)
 */
double** ddg(double** A, int n){
    double** D = create_matrix(n,n);
    int i,j;
    for (i=0; i<n; i++){
        D[i][i] = 0;
        for (j=0; j<n; j++){
            D[i][i] += A[i][j];
        }
    }
    return D;
}

/**
 * @brief Gets a diagonal degree matrix and normalizing it
 * 
 * @param D A 2d array resembling a diagonal degree matrix
 * @param A A 2d array resembling a similarity matrix
 * @param n The number of lines in the matrix
 * @return double** W The Normalized Similarity Matrix (norm)
 */
double** norm(double** D, double** A, int n){
    double** W = create_matrix(n,n);
    double* D_inv_sqrt = malloc(n * sizeof(double));
    if (D_inv_sqrt == NULL) {
        fprintf(stderr, "An Error Has Occurred\n");
        exit(1);
    }

    int i, j, k;
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
 * @param mat A 2D array resembling a matrix
 * @param rows The number of rows in the matrix
 * @param cols The number of columns in the matrix
 * @return double** trans_mat the tranposed matrix
 */
double** transpose_matrix(double** mat, int rows, int cols){
    double** trans_mat = create_matrix(rows, cols);
    int i,j;
    for (i=0; i<rows; i++){
        for (j=0; j<cols; j++){
            trans_mat[i][j] = mat[j][i];
        }
    }
    return trans_mat;
}

/**
 * @brief Gets two matrices and multiplies it
 * 
 * @param A the first matrix
 * @param B the second matrix
 * @param rows_A the number of rows in the first matrix
 * @param cols_A the number of columns in the first matrix
 * @param rows_B the number of rows in the second matrix
 * @param cols_B the number of columns in the second matrix
 * @return double** product_mat the product of multiplication
 */
double** multiply_matrices(double** A, double** B, int rows_A, int cols_A, int rows_B, int cols_B){
    if (cols_A != rows_B){
        fprintf(stderr, "An Error Has Occurred\n");
        exit(1);
    }
    double** product_mat = create_matrix(rows_A, cols_B);
    int i,j,k;
    for(i=0;i<rows_A;i++){
        for (j=0;j<cols_B;j++){
            product_mat[i][j] = 0; //initialize all entries to zero
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
 * @param H A 2D array resembling the initialized decomoposition matrix
 * @param W A 2D array resembling the normalized similarity matrix
 * @param n The number of rows in H, and the number of rows and columns in W
 * @param k The number of columns in H
 * @return double** new_H the updated matrix
 */
double** update_H(double** H, double** W, int n, int k){
    double** WxH = multiply_matrices(W,H,n,n,n,k); //dimensions of WxH are n*k
    double** Ht = transpose_matrix(H,n,k); //dimensions of H^t are k*n
    double** HxHt = multiply_matrices(H,Ht,n,k,k,n); //dimensions of HxH^t are n*n
    double** HHH = multiply_matrices(HxHt,H,n,n,n,k); //dimensions of HHH are n*n
    double** new_H = create_matrix(n,k); //dimensions of new_H are n*k
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
 * @param A A 2D array resembling the first matrix (minuend)
 * @param B A 2D array resembling the second matrix (subtahend)
 * @param rows_A The number of rows in matrix A
 * @param cols_A The number of columns in matrix A
 * @param rows_B The number of rows in matrix B
 * @param cols_B The number of columns in matrix B
 * @return double** diff_mat the result of subraction
 */
double** subtract_matrices(double** A, double** B, int rows_A, int cols_A, int rows_B, int cols_B){
    if(rows_A != rows_B || cols_A != cols_B || A == NULL || B == NULL){
        fprintf(stderr, "An Error Has Occurred\n");
        exit(1);
    }
    int i, j;
    double** diff_mat = create_matrix(rows_A, cols_A);
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
 * @param new_H A 2D array resembling the updated H matrix 
 * @param H A 2D array resembling the old H matrix 
 * @param n The number of rows in both matrices
 * @param k The number of columns in both matrices
 * @return double sqrt(norm) the desired value, which is the square root of the sum of the absolute squares of its elements
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
 * @param H A 2D array resembling the initialized decomoposition matrix
 * @param W A 2D array resembling the normalized similarity matrix
 * @param n The number of rows in H, and the number of rows and columns in W
 * @param k The number of columns in H
 * @return double** H the desired updated matrix
 */
double** optimize_H(double** H, double** W, int n, int k){
    int max_iter = 300;
    int iter = 0;
    double eps = 0.0001;
    double** new_H;
    while (iter < max_iter){
        new_H = update_H(H, W, n, k);
        if (new_H == NULL){
            return NULL;
        }
        if (pow(frobenius_norm(new_H, H, n, k),2) < eps){
            break;
        }
        free_matrix(H,n);
        H = new_H;
        iter++;
    }
    return H;
}

int main(int argc, char** argv){
    char* goal;
    char* file_name;
    if (argc != 3){
        fprintf(stderr, "An Error Has Occured\n Invalid number of arguments");
        return 1;
    }
    goal = argv[1];
    file_name = argv[2];

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "An Error Has Occurred (File Reading Error)\n");
        return 1;
    }

    int d = compute_d(file);
    int n = compute_n(file);
    rewind(file);
    double** data_matrix = compute_data_matrix(file,n,d);
    double** A = sym(data_matrix,n,d);
    double** D = ddg(A, n);
    double** W = norm(D, A, n);

    if (strcmp(goal, "sym")==0){
        print_matrix(A,n,n);
    }
    
    else if (strcmp(goal, "ddg")==0){
        print_matrix(D,n,n);
    }

    else if (strcmp(goal, "norm")==0){
        print_matrix(W,n,n);
    }
    else {
        fprintf(stderr, "Invalid goal argument. Use 'sym','ddg' or 'norm'.\n");
    }

    free_matrix(data_matrix,n);
    free_matrix(A,n);
    free_matrix(D,n);
    free_matrix(W,n);
    fclose(file);
    
    return 0;
}