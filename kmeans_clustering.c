#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* PLAN:
float* K_means(K, iter=200, input_data.txt){
    make_sure_input_is_valid()  
    compute_d(f) #number of columms (V)
    compute n (V)
    put_in_matrix size N*d #use %4f to make sure inputs/outputs are at most 4 decimal places. (V)

    init_initial_centroids(matrix) (inital centroids(matrix of size k*d) as first k data points

    While (convergence(centroids) OR iteration_number > iter:
        for each data_point:
            find_closest_centroid(x,centroids)
        
        update_centroids (u_k= mean(u))
    
    return centroids!!
 */

int k_means(int k, int iter, char* filename);
void is_valid_input(int k, int iter);
double** compute_data_matrix(char *filename, int n, int d);
int compute_d(char *filename);
int compute_n(char *filename);
double** copy_first_k_elements(double** read_matrix, double**write_matrix int k, int d);
void free_matrix(double** matrix, int n);
int convergence(double** centroids, double** before, int curr_iter, int max_iter, int k, int d);
double** alloc_memory_to_matrix(double** matrix, int rows, int columns, const char *error_msg);
// for testings:
void print_matrix(double** matrix, int n, int d);

/* Calculating d - vector size */
int compute_d(char *filename){
    FILE *fp = fopen(filename, "r");
    int d = 0;
    char ch;
     if (fp ==NULL){
        perror("Failed to read line (line 39 in the code)");
        exit(1);
    }
    while((ch = fgetc(fp)) != '\n'){
       if (ch == ',')
       d++; 
    }
    fclose(fp);
    printf("vector size:%d\n", d+1);
    return d + 1; // Number of ","
}

/* Calculating n - number of lines */
int compute_n(char *filename){
    FILE *fp = fopen(filename, "r");
    int n = 0;
    char line[255];
    if (fp ==NULL){
        perror("Failed to read line (line 57 in the code)");
        exit(1);
    }
     while (fgets(line, 255, fp) != NULL) {
        n++;
    }
    fclose(fp);
    printf("number of lines:%d\n", n);
    return n;
}

/**
 * @brief Reads a file and stores data points in a matrix.
 *
 * This function reads an input file containing `n` rows and `d` columns of data points,
 * separated by commas. It allocates memory for an n*d matrix and fills it with 
 * the data points.
 *
 * @param filename The name of the file to read.
 * @param n The number of rows.
 * @param d The number of columns.
 * 
 * @return A pointer to the allocated matrix.
 */
double** compute_data_matrix(char *filename, int n, int d){
    FILE *fp;
    int i,j, buffer_index, k;
    double** mat = alloc_memory_to_matrix(mat, n, d, "Failed to allocate memory to data matrix (line 85)");
    char line[255];
    char buffer[50];
    char ch;
    fp = fopen(filename, "r");
    if (fp ==NULL){
        perror("Failed to read line (line 101)");
        exit(1);
    }
    i=0;
    while (fgets(line, sizeof(line), fp) != NULL){
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

    fclose(fp);
    return mat;
}

/**
 * @brief Copies the first k rows from a matrix to a new k*d matrix.
 *
 * This function copies the first `k` rows from the given matrix
 * with `d` columns into another matrix of size `k*d` at least.
 * It returns a pointer to the new matrix.
 *
 * @param read_matrix The matrix from which you read.
 * @param write_matrix The matrix to which you write.
 * @param k The number of rows to copy.
 * @param d The number of columns per row.
 * 
 * @return A pointer to the new k*d matrix.
 */
double** copy_first_k_elements(double** read_matrix, double**write_matrix, int k, int d){
    int i, j;
    for (i =0; i< k; i++){
        for (int j=0; j<d; j++){
            write_matrix[i][j] = read_matrix[i][j];
        }
    }
    return write_matrix;
}
   
//Print matrix - just to check correctness of code  
void print_matrix(double** matrix, int n, int d){
    int i, j;
    for(i=0; i<n; i++){
        for(j=0;j<d;j++){
            printf("%.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

//Free the allocated memory
void free_matrix(double** matrix, int n){
    int i;
    for (i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    
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

void find_closest_point(double* vector, double** centroids, int* vectors_per_cluster, double **clusters, int d, int k){
    int i, j, min_index;
    double dist;
    double min = INFINITY;
    for (i=0; i<k ; i++){
        dist = vector_distance(vector, centroids[i], d);
        if (dist < min){
            min_index = i;
            min = dist;
        }
    }
    vectors_per_cluster[min_index]++;
    for(j=0; j<d; j++){
      clusters[min_index][j]+= vector[j];
    }
}

double** alloc_memory_to_matrix(double** matrix, int rows, int columns, const char *error_msg){
    int i;
    double **matrix = malloc(sizeof(double*) * rows);
    if (matrix == NULL){
        perror(error_msg);
        exit(1);
    }
    for (i=0; i<rows; i++){
        matrix[i] = malloc(sizeof(double) * columns);
        if (matrix[i] == NULL){
            perror(error_msg);
            exit(1);
        }
    }
    return matrix;
}

int k_means(int k, int iter, char* filename){
    int i, j;
    int d = compute_d(filename);
    int n = compute_n(filename);
    double **data_matrix, **centroids, **clusters;
    int *vectors_per_cluster;

    //allocate memory for the centroids matrix
    centroids = alloc_memory_to_matrix(centroids, k, d, "Failed to allocate memory to centroids matrix");

    //allocate memory for the cluster matrix
    clusters = alloc_memory_to_matrix(clusters, k, d), "Failed to allocate memory to clusters matrix";

    //allocate memory for the vectors_per_cluster matrix
    vectors_per_cluster = malloc(sizeof(int) * k);
    if (vectors_per_cluster == NULL) {
        perror("Failed to allocate memory to vectors_per_cluster matrix")
        exit(1);
    }



    data_matrix = compute_data_matrix(filename, n, d);
    centroids = copy_first_k_elements(data_matrix, centroids, k, d);


    free_matrix(data_matrix, n);

    print_matrix(centroids);

    return 0;
}

/*
Missing:
* valid inputs of extreme cases */
int main(int argc, char** argv){
    int k, iter; //later check validity
    char *filename;
    if ((argc != 3) && (argc != 4)){
        perror("Invalid Input!");
        return 1;
    }
    else if (argc == 4){
        k = atoi(argv[1]); 
        iter = atoi(argv[2]); 
        filename = argv[3];
    }
    else if (argc == 3){
        k = atoi(argv[1]); 
        iter = 200;
        filename = argv[2];
    }
    k_means(k, iter, filename);
    return 0;
}