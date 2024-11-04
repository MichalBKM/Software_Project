import math
import sys

# Reads a txt file and converts it to a 2D array (matrix) of floats.
# @param filename: Name of the file.
# @return matrix:.
def create_data_matrix(filename):
    file = open(filename, 'r')
    matrix = []
    for row in file:
        matrix.append([float(x) for x in row.split(",")])

    file.close()
    return matrix

# Prints a 2D matrix with values formatted to 4 decimal places.
# @param mat: 2D list of floats representing the matrix.
def print_matrix(mat):
    for row in mat:
        print(','.join(f'{value:.4f}' for value in row))


# Calculates the Euclidean distance between two d-dimensional vectors.
# @param x: First vector.
# @param y: Second vector.
# @return res: The Euclidean distance:
def vector_distance(x, y):
    res = 0
    d = len(x)
    for i in range(d):
        res += math.pow((x[i]-y[i]), 2)
    res = math.sqrt(res)
    return res

# Finds the closest centroid to a given vector and assigns it to a cluster.
# @param vector: Data point to be assigned as a list of floats.
# @param centroids: List of centroid vectors.
# @param clusters: List of clusters, where each cluster is a list of vectors.
# @param k: Number of clusters.
def find_closest_point(vector, centroids, clusters, k):
    min_index = 0
    minimum = float('inf') #infinity
    for i in range(k):
        dist = vector_distance(vector, centroids[i])
        if (dist < minimum):
            min_index = i
            minimum = dist
    clusters[min_index].append(vector)

# Updates centroids based on the average of points in each cluster.
# @param clusters: List of clusters with each cluster as a list of vectors.
# @param k: Number of clusters.
# @return new_centroids: Updated list of centroid vectors.
def update_centroids(clusters, k):
    new_centroids = [] 
    for i in range(k):
        new_centroids.append([sum(column) / len(column) for column in zip(*clusters[i])])
    return new_centroids

def convergence(centroids, prev_centroids, curr_iter, iter, k):
    if (curr_iter >= iter):
        return True
    for i in range(k):
        dist = vector_distance(centroids[i], prev_centroids[i])
        if (dist >= 0.001):
            return False
    return True

# Implements the k-means clustering algo
# rithm on a data matrix.
# @param k: Number of clusters.
# @param iter: Maximum number of iterations.
# @param data_matrix: 2D list representing the data matrix.
# @return centroids: Final centroids after clustering.
def k_means(k, iter, data_matrix):
    
    n = len(data_matrix)

    assert k>1 and k<n, "Invalid number of clusters!"
    assert iter>1 and iter<1000, "Invalid maximum iteration!"

    d = len(data_matrix[0])

    centroids = [data_matrix[i] for i in range(k)]
  
    prev_centroids = [[float('inf') for _ in range(d)] for _ in range(k)]
    curr_iter = 0

    while not (convergence(centroids,prev_centroids,curr_iter,iter,k)):
        clusters = [[] for _ in range(k)] 
        for vector in data_matrix:
            find_closest_point(vector,centroids,clusters,k)
        prev_centroids = centroids
        centroids = update_centroids(clusters, k)
        curr_iter += 1

    return centroids

if __name__ == "__main__":
    try:
        if len(sys.argv) < 3 or len(sys.argv) > 4:
            print("An Error Has Ocucurred")
            exit(1)
        k = int(sys.argv[1])  
        if k != float(sys.argv[1]):
            print("Invalid number of clusters!")
            exit(1) 
    except ValueError:
        print("Invalid number of clusters!")
        exit(1)
    try:
        iter = int(sys.argv[2]) if len(sys.argv) == 4 else 200  
        if len(sys.argv) == 4 and iter != float(iter):
            print("Invalid maximum iteration!")
            exit(1)
        filename = sys.argv[3] if len(sys.argv) == 4 else sys.argv[2]
        data_matrix = create_data_matrix(filename)
        print_matrix(k_means(k, iter, data_matrix))
    except ValueError:
        print("Invalid maximum iteration!")
        exit(1)

        