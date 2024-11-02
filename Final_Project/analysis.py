import sys
import numpy as np
import math
import symnmfmodule
import kmeans
from sklearn.metrics import silhouette_score
from symnmf import initialize_H, compute_data_matrix

def symnmf_clustering(dataMatrix, n, d, k):
    W = symnmfmodule.norm(dataMatrix, n, d)
    H = initialize_H(n, k, W)
    optimal_H = symnmfmodule.symnmf(W, H, n, k) #calculate OPT h using W and init H
    symnmf_indexes = np.argmax(optimal_H, axis=1)
    return symnmf_indexes

def kmeans_clustering(dataMatrix, n, d, k):
    #todo

def main():
    k = int(sys.argv[1])
    file_name = sys.argv[2]

    dataMatrix = compute_data_matrix(file_name) #load data
    n = len(dataMatrix)
    d = len(dataMatrix[0])

    symnmf_values = symnmf_clustering(dataMatrix, n, d, k)
    symnmf_score = silhouette_score(dataMatrix, symnmf_values)

    symnmf_values = kmeans_clustering(...) #todo
    kmeans_score = silhouette_score(dataMatrix, kmeans_values)

    print(f"nmf: {symnmf_score: .4f}")
    print(f"kmeans: {kmeans_score: .4f}")



if __name__ == "__main__":
    main()