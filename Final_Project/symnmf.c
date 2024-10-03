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


int compute_d(FILE* file){
    int d = 0;
    char ch;
    while((ch = fgetc(file)) != '\n'){
       if (ch == ',') d++; 
    }
    rewind(file);
    return d + 1; 
}

int compute_n(FILE* file){
    int n = 0;
    char line[255];
     while (fgets(line, sizeof(line), file) != NULL) {
        n++;
    }
    rewind(file);
    return n;
}

double** compute_data_matrix(FILE* file, int n, int d){
    int i,j, buffer_index, k;
    double** mat = create_matrix(n, d);
    char line[255];
    char buffer[50];
    char ch;

    i=0;
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

void free_matrix(double** matrix, int n){
    int i;
    for (i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    
}

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