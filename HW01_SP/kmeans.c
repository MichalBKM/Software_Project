#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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



void getMatrix(){ //THATS A NIGHTMARE
    FILE *file = fopen("numbers.txt", "r");
    int **matrix = NULL;
    int rows = 0, cols = 0;
    char line[1024];
    int tmp;

    //read the first line to know #cols
    if (fgets(line, sizeof(line), file)){
        char *ptr = line;
        while((*ptr!='\n')){
            cols++;
            if (*ptr==',') ptr++;
        }
    }
    printf("%d", cols);
}

int main(){
    getMatrix();
}

/*
argc = number of command-line arguments
argv = list of command-line arguments
*/

/*
int main(int argc, char** argv){
    
    int k = atoi(argv[1]); //later check validity
    int iter = atoi(argv[2]); //later check validity
    printf("%d", k);
    printf("\n%d\n", iter);
    char* filename = argv[3]; //assume validity
    getMatrix(); //read txt to matrix
    return 0;
}
*/

/*
double* k_means(int k, int iter, char* filename){
    FILE *fp = fopen(filename, "r");
    if (fp ==NULL){
        printf("Error: could not open file %s", filename);
        return 1;
    }

}
*/