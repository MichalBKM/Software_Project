#define PY_SSIZE_T_CLEAN

#include <stdio.h>
#include <Python.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


void copy_matrix(double **read_matrix, double**write_matrix, int k, int d);
void free_matrix(double** matrix, int rows);
double vector_distance(double *x, double *y, int d);
void find_closest_point(double* vector, double** centroids, int* vectors_per_cluster, double **clusters, int k, int d);
double** create_matrix(int rows, int columns);
void update_centroids(double **centroids, int *vectors_per_cluster, double **clusters, int k, int d);
int convergence(double** centroids, double** before, int curr_iter, int max_iter, int k, int d, double eps);
void clear_matrix(double **clusters, int *vectors_per_cluster, int k, int d);
double** k_means(int k, int maxIter, int n, int d, double **data_matrix, double **centroids, double eps);
void print_matrix(double** matrix, int rows, int columns);



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
int convergence(double **centroids, double **prev_centroids, int curr_iter, int iter, int k, int d, double eps){
    int i;
    double dist;
    if (curr_iter >= iter){
        return 1;
    }
    for (i =0; i<k; i++){
        dist = vector_distance(centroids[i], prev_centroids[i], d);
        if (dist >= eps){
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

double** k_means(int k, int maxIter, int n, int d, double **data_matrix, double **centroids, double eps){
    int i, curr_iter;
    double *vector;
    double **clusters, **prev_centroids;
    int *vectors_per_cluster;

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

    curr_iter = 0;
    while (!convergence(centroids, prev_centroids, curr_iter, maxIter, k, d, eps)){
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
    free(vectors_per_cluster);

    return centroids;
}

static void convert_PyObj_To_cMatrix(PyObject* pyMat, double **cMat, int rows, int columns)
{
    int i,j;
    PyObject *matRow, *matValue;

    for (i = 0; i< rows ; i++){
        matRow = PyList_GetItem(pyMat, i);
        if (!PyList_Check(matRow)){
            continue;
        }
        for (j = 0; j < columns; j++){
            matValue = PyList_GetItem(matRow, j);
            cMat[i][j] = PyFloat_AsDouble(matValue);
        }
    }
}

static PyObject* fit(PyObject *self, PyObject *args)
{
    int n, d, k, maxIter, i, j;
    double eps;
    PyObject *PyCentroids, *PyDataPoints, *finalCentroids_py, *single_centroid;
    double** dataPoints;
    double** centroids;
    double** finalCentroids_c;
    /* This parses the Python arguments into a double (d)  variable named z and int (i) variable named n*/
    if(!PyArg_ParseTuple(args, "iiiidOO", &d, &n, &k, &maxIter, &eps, &PyCentroids, &PyDataPoints)) {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }

    // Ensure the objects are lists of lists
    if (!PyList_Check(PyCentroids) || !PyList_Check(PyDataPoints)) {
        PyErr_SetString(PyExc_TypeError, "Both arguments must be matrices.");
        return NULL;
    }
    dataPoints = create_matrix(n, d);
    convert_PyObj_To_cMatrix(PyDataPoints, dataPoints, n, d);

    centroids = create_matrix(k, d);
    convert_PyObj_To_cMatrix(PyCentroids, centroids, k, d);


    finalCentroids_c = k_means(k, maxIter, n, d, dataPoints, centroids, eps);
    finalCentroids_py = PyTuple_New(k);
    for (i = 0; i < k; i++) {
        single_centroid = PyTuple_New(d);
        for (j = 0; j < d; j++){
            PyTuple_SET_ITEM(single_centroid, j, PyFloat_FromDouble(finalCentroids_c[i][j]));
        } 
    PyTuple_SET_ITEM(finalCentroids_py, i, single_centroid);
    }

    free_matrix(centroids, k);
    free_matrix(dataPoints, n); 

    return finalCentroids_py;  
}
static PyMethodDef kmeans_pp_Methods[] = {
    {  
        "fit",                   
        (PyCFunction) fit, 
        METH_VARARGS,          
        PyDoc_STR("fit(d, n, k, maxIter, eps, PyCentroids, PyDataPoints)\n\n"
                  "Parameters:\n"
                  "d: int - dimension of the vectors\n"
                  "n: int - amount of data points\n"
                  "k: int - number of initialized centroids\n"
                  "maxIter: int - maximum number of iterations\n"
                  "eps: float - convergence threshold\n"
                  "PyCentroids: list of lists - initial centroids\n"
                  "PyDataPoints: list of lists - data points\n\n"
                  "Returns:\n"
                  "finalCentroids: list of lists - final centroids")
    }, {
        NULL, NULL, 0, NULL
        }
};


static struct PyModuleDef mykmeanssp = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    kmeans_pp_Methods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&mykmeanssp);
    if (!m) {
        return NULL;
    }
    return m;
}