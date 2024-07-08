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
double** initial_centroids(double** mat, int k, int d);
// for testings:
void print_matrix(double** matrix, int n, int d);

/* Calculating d - vector size */
int compute_d(char *filename){
    FILE *fp = fopen(filename, "r");
    int d = 0;
    char ch;
     if (fp ==NULL){
        perror("Failed to read line (line 35 in the code)");
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
        perror("Failed to read line (line 53 in the code)");
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
 * This function reads a file containing `n` rows and `d` columns of data points,
 * separated by commas. It allocates memory for an n*d matrix and fills it with 
 * the data points. If memory allocation fails or the file can't be opened, it prints an 
 * error and exits.
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
    double** mat = malloc(sizeof(double*) * n);
    char line[255];
    char buffer[50];
    char ch;
    if (mat == NULL){
        perror("Failed to allocate memory (line 73)");
        exit(1);
    }
    for (i=0; i<n; i++){
        mat[i] = malloc(sizeof(double) * d);
        if (mat[i] == NULL){
            perror("Failed to allocate memory (line 79)");
            exit(1);
        }
    }
    fp = fopen(filename, "r");
    if (fp ==NULL){
        perror("Failed to read line (line 85)");
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

double** initial_centroids(double** mat, int k, int d){
    int i, j;
    double** centroids;
    printf("&&\n");
    centroids = malloc(sizeof(double*) * k);

    if (centroids == NULL){
        perror("Failed to allocate memory (line 73)");
        exit(1);
    }
    printf("allocated memory 1 OK\n");

    for (i=0; i<k; i++){ //failed here - why????
        centroids[i] = malloc(sizeof(double) * d);
        if (centroids[i] == NULL){
            perror("Failed to allocate memory (line 79)");
            exit(1);
            }
        printf("allocated memory 2 OK\n");

        for (j=0; j<d; j++){
            printf("$");
            centroids[i][j] = mat[i][j];
            printf("centroids: %f",centroids[i][j]);
        }
    }
    return centroids;
}
    

//Print matrix - for testings - works OK
void print_matrix(double** matrix, int rows, int cols){
    int i, j;
    for(i=0; i<rows; i++){
        for(j=0;j<cols;j++){
            printf("%.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int k_means(int k, int iter, char* filename){
    int i, j;
    int d = compute_d(filename);
    int n = compute_n(filename);
    double **data_matrix, **centroids, **clusters;
    int *vectors_per_clusters;

    data_matrix = compute_data_matrix(filename, n, d);
    centroids = initial_centroids(data_matrix, k, d); //FAILING

    //printf("matrix:\n");
    //print_matrix(data_matrix, n, d);
    printf("centroids:\n");
    print_matrix(centroids, k, d);

    //Free the allocated memory
    /*
    for (i = 0; i < n; i++) {
        free(data_matrix[i]);
    }
    free(data_matrix);
    for (i = 0; i < k; i++){
        free(centroids[i]);
    }
    free(centroids);*/

    return 0;
}


/*
Missing:
* valid inputs of extreme cases */
int main(int argc, char** argv){
    int k, iter; //later check validity
    char *filename;
    if (argc == 4){
        int k = atoi(argv[1]); 
        int iter = atoi(argv[2]); 
        filename = argv[3];
    }
    else if (argc == 3){
        k = atoi(argv[1]); 
        iter = 200;
        filename = argv[2];
    }
    else{
        perror("Invalid Input!");
        return 1;
    }
    k_means(k, iter, filename);
    return 0;
}
