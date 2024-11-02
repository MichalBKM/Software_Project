import sys
import numpy as np
import math
import symnmfmodule
import kmeans
from sklearn.metrics import silhouette_score
from symnmf import initialize_H, compute_data_matrix

MAX_ITER = 300

def closest_centroid(vector, centroids):
    distances = np.linalg.norm(np.array(centroids) - np.array(vector), axis=1)
    return np.argmin(distances)

def symnmf_clustering(data_matrix, k):
    n = len(data_matrix)
    d = len(data_matrix[0])
    W = symnmfmodule.norm(data_matrix, n, d)
    H = initialize_H(n, k, W)
    optimal_H = symnmfmodule.symnmf(W, H, n, k) #calculate OPT h using W and init H
    optimal_H = np.array(optimal_H)
    symnmf_indexes = np.argmax(optimal_H, axis=1)
    return symnmf_indexes

def kmeans_clustering(data_matrix, k):
    centroids = kmeans.k_means(k, MAX_ITER, data_matrix)
    return [closest_centroid(vector, centroids) for vector in data_matrix]

def main():
    k = int(sys.argv[1])
    file_name = sys.argv[2]

    data_matrix = compute_data_matrix(file_name) #load data
    

    symnmf_values = symnmf_clustering(data_matrix, k)
    symnmf_score = silhouette_score(data_matrix, symnmf_values)

    kmeans_values = kmeans_clustering(data_matrix, k) 
    kmeans_score = silhouette_score(data_matrix, kmeans_values)

    print(f"nmf: {symnmf_score: .4f}")
    print(f"kmeans: {kmeans_score: .4f}")



if __name__ == "__main__":
    main()