#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int compute_d(void);
int compute_n(void);
double** compute_data_matrix(int n, int d);
void copy_matrix(double **read_matrix, double**write_matrix, int k, int d);
void free_matrix(double** matrix, int n);
double vector_distance(double *x, double *y, int d);
void find_closest_point(double* vector, double** centroids, int* vectors_per_cluster, double **clusters, int k, int d);
double** create_matrix(int rows, int columns);
void update_centroids(double **centroids, int *vectors_per_cluster, double **clusters, int k, int d);
int convergence(double** centroids, double** before, int curr_iter, int max_iter, int k, int d);
void clear_matrix(double **clusters, int *vectors_per_cluster, int k, int d);
int k_means(int k, int iter);
void print_matrix(double** matrix, int rows, int columns);

/* Calculating d - vector size */
int compute_d(void){
    int d = 0;
    char ch;
    freopen(NULL, "r", stdin);
    while((ch = getchar()) != '\n'){
       if (ch == ',') d++; 
    }
    return d + 1; 
}

/* Calculating n - number of lines */
int compute_n(void){
    int n = 0;
    char line[255];
    freopen(NULL, "r", stdin);
     while (fgets(line, sizeof(line), stdin) != NULL) {
        n++;
    }
    return n;
}

/**
 * @brief Reads a file and stores data points in a matrix.
 *
 * @param n The number of rows.
 * @param d The number of columns.
 * 
 * @return A pointer to the allocated matrix.
 */
double** compute_data_matrix(int n, int d){
    int i,j, buffer_index, k;
    double** mat = create_matrix(n, d);
    char line[255];
    char buffer[50];
    char ch;

    i=0;
    freopen(NULL, "r", stdin);
    while (fgets(line, sizeof(line), stdin) != NULL){
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
 * @brief Copies the first k rows from a matrix to a new k*d matrix.
 *
 * @param read_matrix The matrix from which you read.
 * @param write_matrix The matrix to which you write.
 * @param k The number of rows to copy.
 * @param d The number of columns per row.
 * 
 * @return A pointer to the new k*d matrix.
 */
void copy_matrix(double** read_matrix, double**write_matrix, int k, int d){
    int i, j;
    for (i =0; i< k; i++){
        for (j=0; j<d; j++){
            write_matrix[i][j] = read_matrix[i][j];
        }
    }
}
   
/**
 * @brief prints the matrix
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
 * @brief free the allocated memory of a given matrix
 */ 
void free_matrix(double** matrix, int n){
    int i;
    for (i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    
}

/**
 * @brief Calculates the Euclidean distance between two vectors.
 *
 * @param x The first vector.
 * @param y The second vector.
 * @param d The dimension of the vectors.
 * 
 * @return The Euclidean distance between the two vectors.
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
 * @brief Finds the closest centroid to a given vector and updates the cluster.
 *
 * Finds the closest centroid from the given `vector` among `k` centroids,
 * updates the count of vectors per cluster, and accumulates the vector to the relevant cluster.
 *
 * @param vector The vector for which to find the closest centroid.
 * @param centroids The matrix of centroids.
 * @param vectors_per_cluster The array holding the count of vectors in each cluster.
 * @param clusters The matrix to accumulate the vectors in each cluster.
 * @param k The number of centroids/clusters.
 * @param d The dimension of each vector.
 */

void find_closest_point(double* vector, double** centroids, int* vectors_per_cluster, double **clusters, int k, int d){
    int i, j, min_index;
    double dist;
    double min = HUGE_VAL;
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

/**
 * @brief Allocates memory for a matrix and returns a pointer to it.
 *
 * @param matrix The matrix to be created.
 * @param rows The number of rows.
 * @param columns The number of columns.
 * @param error_msg The error message to print if allocation fails.
 * 
 * @return A pointer to the allocated matrix.
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
 * @brief Updates the centroids to be the mean of each corresponding cluster.
 *
 * @param centroids The matrix of centroids to be updated.
 * @param vectors_per_cluster The array holding the amount of vectors in each cluster.
 * @param clusters The matrix containing the sum of vectors in each cluster.
 * @param k The number of centroids/clusters.
 * @param d The dimension of each vector.
 */
void update_centroids(double **centroids, int *vectors_per_cluster, double **clusters, int k, int d){
    int i,j;
    for(i =0; i < k; i++){
        for(j = 0; j < d; j++){
            centroids[i][j] = (clusters[i][j] / vectors_per_cluster[i]);
        }
    }
}

/**
 * @brief Checks if the centroids have converged or the maximum iterations have been reached.
 *
 * @param centroids The current centroids.
 * @param prev_centroids The centroids from the previous iteration.
 * @param curr_iter The current iteration number.
 * @param iter The maximum number of iterations allowed.
 * @param k The number of centroids/clusters.
 * @param d The dimension of each vector.
 * 
 * @return 1 if the centroids have converged or the maximum iterations have been reached, otherwise 0.
 */
int convergence(double **centroids, double **prev_centroids, int curr_iter, int iter, int k, int d){
    int i;
    double dist;
    if (curr_iter >= iter){
        return 1;
    }
    for (i =0; i<k; i++){
        dist = vector_distance(centroids[i], prev_centroids[i], d);
        if (dist >= 0.001){
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Resets the vectors_per_cluster count and the clusters matrix.
 *
 * @param clusters The matrix of clusters to be cleared.
 * @param vectors_per_cluster The array holding the count of vectors in each cluster.
 * @param k The number of clusters.
 * @param d The dimension of each vector.
 */
void clear_matrix(double **clusters, int *vectors_per_cluster, int k, int d){
    int i ,j;
    for (i =0; i < k; i++){
        vectors_per_cluster[i] = 0;
        for (j=0; j<d; j++){
            clusters[i][j] = 0.0;
        }
    }
}

int k_means(int k, int iter){
    int i, curr_iter;
    int d = compute_d();
    int n = compute_n();
    double *vector;
    double **data_matrix, **centroids, **clusters, **prev_centroids;
    int *vectors_per_cluster;


    if (k > n){
        fprintf(stderr, "Invalid number of clusters!");
        exit(1);
    }
    /*allocate memory for the centroids matrix */
    centroids = create_matrix(k, d);

    /*allocate memory for the cluster matrix*/
    clusters = create_matrix(k, d);

    /*allocate memory for the prev_clusters matrix*/
    prev_centroids = create_matrix(k, d);

    /*allocate memory for the vectors_per_cluster matrix*/
    vectors_per_cluster = malloc(sizeof(int) * k);
    if (vectors_per_cluster == NULL) {
        fprintf(stderr, "An Error Has Occurred\n");
        exit(1);
    }



    data_matrix = compute_data_matrix(n, d);
    copy_matrix(data_matrix, centroids, k, d);

    curr_iter = 0;
    while (!convergence(centroids, prev_centroids, curr_iter, iter, k, d)){
        clear_matrix(clusters, vectors_per_cluster, k, d);
        for (i = 0; i < n; i++){
            vector = data_matrix[i];
            find_closest_point(vector, centroids, vectors_per_cluster, clusters, k, d);
        }
        copy_matrix(centroids, prev_centroids, k, d);
        update_centroids(centroids, vectors_per_cluster, clusters, k, d);
        curr_iter++;
    }

    free_matrix(prev_centroids, k);
    free_matrix(clusters, k);
    free_matrix(data_matrix, n);
    free(vectors_per_cluster);

    print_matrix(centroids, k, d);
    free_matrix(centroids, k);

    return 0;
}

/*
Missing:
* valid inputs of extreme cases */
int main(int argc, char** argv){
    int k, iter;
    if ((argc != 2) && (argc != 3)){
        fprintf(stderr, "An Error Has Occurred\n Invalid number of arguments");
        return 1;
    }
    else if (argc == 3){
        iter = atoi(argv[2]);
        if (iter != atof(argv[2]) || iter <= 0 || iter > 1000){
            fprintf(stderr, "Invalid maximum iteration!");
            return 1;
        }
    }
    else if (argc == 2){ 
        iter = 200;
    }
    k = atoi(argv[1]);
    if (k != atof(argv[1]) || k < 1){
        fprintf(stderr, "Invalid number of clusters!");
        return 1;
    }
    k_means(k, iter);
    return 0;
}
