import sys
import math
import numpy as np
import symnmfmodule
import kmeans

from sklearn.metrics import silhouette_score
from symnmf import initialize_H, compute_data_matrix

def mean_cluster(vector, cluster):

    return None

#Retruns the closest centroid for given vector
def closest_centroid(vector, centroids):
    distances = np.linalg.norm(centroids - vector, axis=1)
    return np.argmin(distances)

def main():
    k = int(sys.argv[1])
    file_name = sys.argv[2]

    dataMatrix = compute_data_matrix(file_name)
    d = len(dataMatrix[0])
    n = len(dataMatrix)

    W = symnmfmodule.norm(dataMatrix, n, d)
    H = initialize_H(n, k, W)
    optimal_H = symnmfmodule.symnmf(W, H, n, k)

    cluster_indexes = np.argmax(optimal_H, axis=1)
    #TODO: Compute centroids
    #TODO: For each data point, compute the closest centroid to the data point which isn't the cluster its currently in

    sum = 0
    for row in dataMatrix:
        #TODO: a = ?
        #TODO: b = ?

        silhouette_coefficient = (b-a)/max(a, b)
        sum += silhouette_coefficient
    

if __name__ == "__main__":
    main()