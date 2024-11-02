import sys
import math
import numpy as np
import symnmfmodule
import kmeans

from sklearn.metrics import silhouette_score
from symnmf import initialize_H, compute_data_matrix


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

if __name__ == "__main__":
    main()