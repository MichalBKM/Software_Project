import math
import sys

#reads a txt file and convert it to a 2d array V
def create_matrix(filename):
    file = open(filename, 'r')
    matrix = []
    for row in file:
        matrix.append([float(x) for x in row.split(",")])

    file.close()
    return matrix

#takes a matrix and create a new one from the first k vectors V
def copy_matrix(mat, k):
    ret_mat = [[0 for i in range(len(mat[0]))] for i in range(k)]
    for i in range(k):
        for j in range(len(mat[0])):
            ret_mat[i][j] = mat[i][j]

    return ret_mat

def clear_matrix(clusters, vectors_per_clusters, k, d):
    for i in range(k):
        vectors_per_clusters[i] = 0
        for j in range(d):
            clusters[i][j] = 0.0

#fix this to be the same as the output needed in the assignment
def print_matrix(mat):
    for row in mat:
        print(','.join(f"{x:.4f}" for x in row))


#calculating euclidean distance from two d-dimensional vectors V
def vector_distance(x,y):
    res = 0
    d = len(x)
    for i in range(d):
        res += math.pow((x[i]-y[i]), 2)
    res = math.sqrt(res)
    return res

#void
def find_closest_point(vector, centroids, vectors_per_cluster, clusters, k):
    d = len(vector)
    min_index = 0
    minimum = float('inf') #infinity
    for i in range(k):
        dist = vector_distance(vector, centroids[i])
        if (dist < minimum):
            min_index = i
            minimum = dist
    vectors_per_cluster[min_index] += 1
    for j in range(d):
        clusters[min_index][j] += vector[j]


#void WTF....
def update_centroids(centroids, vectors_per_cluster, clusters, k):
    d = len(centroids[0])
    for i in range(k):
        if vectors_per_cluster[i] ==0:
            continue
        for j in range(d):
            centroids[i][j] = (clusters[i][j]) / vectors_per_cluster[i]


#boolean True \ False
def convergence(centroids, prev_centroids, curr_iter, iter, k):
    if curr_iter >= iter:
        return True
    for i in range(k):
        if vector_distance(centroids[i], prev_centroids[i]) >= 0.001:
            return False
    return True


def k_means(k, iter, filename):
    matrix = create_matrix(filename)
    n = len(matrix)
    d = len(matrix[0])

    assert k>1 and k<n, "Invalid number of clusters!"
    assert iter>1 and iter<1000, "Invalid maximum iteration!"

    centroids = copy_matrix(matrix, k) #initialize centroids as first k datapoints
    prev_centroids = [[0.0] *d for _ in range(k)]
    curr_iter = 0
    vectors_per_cluster = [0] * k
    clusters = [[0.0] * d for _ in range(k)]

    while not (convergence(centroids,prev_centroids,curr_iter,iter,k)):
        clear_matrix(clusters, vectors_per_cluster, k, d)
        for vector in matrix:
            find_closest_point(vector,centroids, vectors_per_cluster, clusters,k)
        prev_centroids = copy_matrix(centroids, k)
        update_centroids(centroids,vectors_per_cluster, clusters, k)
        curr_iter += 1

    print_matrix(centroids)
    return 0


if __name__== "__main__":
    k_means(3, 600, 'input_1.txt')