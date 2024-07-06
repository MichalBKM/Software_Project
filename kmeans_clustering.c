#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* 
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
    
    return centroids

 */

int int k_means(int k, int iter, char* filename);
void is_valid_input(int k, int iter);
int compute_dimension(char *fp);

int main(int argc, char** argv){
    char *filename = "input_data.txt"; //argv[2]
    int k, iter;
    k = 3; //argv[0].to_int()
    iter = 200; //argv[1] check if it is an integer if not. we take iter=200
    //is_valid_input(k, iter, filename);

    return k_means(k, iter, filename);
}


int k_means(int k, int iter, char* filename){
    int n;
    int d = compute_dimension(filename);
    FILE *fp = fopen(filename, "r");
    if (fp ==NULL){
        perror("Failed to read line (line 57 in the code)");
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}

/* calculating d which is the size of each data point */
int compute_dimension(char *filename){
    FILE *fp = fopen(filename, "r");
     if (fp ==NULL){
        perror("Failed to read line (line 57 in the code)");
        fclose(fp);
        exit(1);
    }
    int cnt = 0;
    char ch;
    while((ch = fgetc(fp)) != '\n'){
       if (ch == ',')
       cnt++; 
    }
    fclose(fp);
    return cnt + 1; // Number of "," + 1
}



