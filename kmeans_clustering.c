#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* PLAN:
float* K_means(K, iter=200, input_data.txt){
    make_sure_input_is_valid() 
    f = open(input_data.txt)
    compute_d(f) #number of columms
    put_in_matrix size N*d #use %4f to make sure inputs/outputs are at most 4 decimal places.

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


int k_means(int k, int iter, char* filename){
    int d = compute_d(filename);
    int n = compute_n(filename);
    double **data_matrix, **centroids;

    data_matrix = compute_data_matrix(filename, n, d);
    
    centroids = initial_centroids(data_matrix, k, d);

    //Print matrix - just to check correctness of code  
    printf("printing whole matrix:\n");
    for(int i=0; i<n; i++){
        for(int j=0;j<d;j++){
            printf("%.4f ", data_matrix[i][j]);
        }
        printf("\n");
    }
    
    //Print k centroids - just to check correctness of code
    printf("printing whole matrix:\n");  
    for(int i=0; i<n; i++){
        for(int j=0;j<d;j++){
            printf("%.4f ", centroids[i][j]);
        }
        printf("\n");
    }
    
    //Free the allocated memory
    for (int i = 0; i < n; i++) {
        free(data_matrix[i]);
    }
    free(data_matrix);
    return 0;
}
/* Calculating d - vector size */
int compute_d(char *filename){
    FILE *fp = fopen(filename, "r");
    int d = 0;
    char ch;
     if (fp ==NULL){
        perror("Failed to read line (line 61 in the code)");
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
        perror("Failed to read line (line 79 in the code)");
        exit(1);
    }
     while (fgets(line, 255, fp) != NULL) {
        n++;
    }
    fclose(fp);
    printf("number of lines:%d\n", n);
    return n;
}

/* reads a text file with n rows and d columns, converts it into a double matrix of the size n*d */
double** compute_data_matrix(char *filename, int n, int d){
    FILE *fp;
    int i,j, buffer_index, k;
    double** mat = malloc(sizeof(double*) * n);
    char line[255];
    char buffer[50];
    char ch;
    if (mat == NULL){
        perror("Failed to allocate memory (line 99)");
        exit(1);
    }
    for (i=0; i<n; i++){
        mat[i] = malloc(sizeof(double) * d);
        if (mat[i] == NULL){
            perror("Failed to allocate memory (line 105)");
            exit(1);
        }
    }
    fp = fopen(filename, "r");
    if (fp ==NULL){
        perror("Failed to read line (line 111)");
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
    double** centroids = malloc(sizeof(double*) * k);
    for (int i=0; i<k; i++){
        centroids[i] = malloc(sizeof(double) * d);
        if (centroids[i] == NULL){
            perror("Failed to allocate memory (line 105)");
            exit(1);
        }
        for (int j=0; j<d; j++){
            centroids[i][j] = mat[i][j];
        }
    }
    return centroids;
}

int main(int argc, char** argv){
    int k = atoi(argv[1]); //later check validity
    int iter = atoi(argv[2]); //later check validity
    char* filename = argv[3]; //assume validity
    printf("iter: %d\n", iter);
    k_means(k, iter, filename);
    
    return 0;
}