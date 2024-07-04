# include <stdio.h>
# include <stdlib.h>
# include <math.h>

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

double* k_means()
void is_valid_input(int k, int iter);

int main(int argc, char** argv){
    int k, iter;
    char* filename;
    k = //argv[0].to_int()
    iter =//argv[1] check if it is an integer if not. we take iter=200
    filename = //argv[2]
    is_valid_input(k, iter, filename);


    return k_means(k, iter, filename)
}

double* k_means(int k, int iter, char* filename){
    FILE *fp = fopen(filename, "r");
    if (fp ==NULL){
        printf("Error: could not open file %s", filename);
        return 1;
    }

}

