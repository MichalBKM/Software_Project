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

int k_means(int k, int iter, char* filename){
    int d = compute_d(filename);
    int n = compute_n(filename);
    double **data_matrix, **centroids;

    FILE *fp = fopen(filename, "r");

    if (fp ==NULL){
        perror("Failed to read line (line 57 in the code)");
        fclose(fp);
        return 1;
    }

    data_matrix = compute_data_matrix(filename, n, d);
   

    //Print matrix - just to check correctness of code  
    for(i=0; i<n; i++){
        for(j=0;j<d;j++){
            printf("%.4f ", data_matrix[i][j]);
        }
        printf("\n");
    }
    // Print matrix - just to check correctness of code  

    //Free the allocated memory
    for (i = 0; i < n; i++) {
        free(data_matrix[i]);
    }
    free(data_matrix);

    fclose(fp);
    return 0;
}
/* Calculating d - vector size */
int compute_d(char *filename){
    FILE *fp = fopen(filename, "r");
    int d = 0;
    char ch;
     if (fp ==NULL){
        perror("Failed to read line (line 57 in the code)");
        fclose(fp);
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
        fclose(fp);
        exit(1);
    }
     while (fgets(line, sizeof(line), fp) != NULL) {
        n++;
    }
    fclose(fp);
    printf("number of lines:%d\n", n+1);
    return n + 1; // Number of lines
}

double** compute_data_matrix(char *filename, int n, int d){
    FILE *fp;
    int i=0,j=0;
    double** mat = malloc(sizeof(double*) * n);
    char *line[255];
    char *point_buffer;
    char ch;
    if (mat == NULL){
        perror("Failed to allocate memory (line 105)");
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
    while (fgets(line, sizeof(line), fp) != NULL){
        while((ch = fgetc(fp)) != '\n'){
       if (ch == ','){

       }
    }


}


int main(int argc, char** argv){
    int k = atoi(argv[1]); //later check validity
    int iter = atoi(argv[2]); //later check validity
    char* filename = argv[3]; //assume validity
    k_means(k, iter, filename);

    return 0;
}