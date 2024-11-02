import sys
import math
import numpy as np
import symnmfmodule

np.random.seed(1234)

#reads a txt file and convert it to a 2d array V
def compute_data_matrix(filename):
    file = open(filename, 'r')
    matrix = []
    for row in file:
        matrix.append([float(x) for x in row.split(",")])

    file.close()
    return matrix

def print_matrix(matrix):

    for row in matrix:
        print(",".join(f"{x:.4f}" for x in row))

def initialize_H(n, k, W):

    m = np.mean(W)

    upper_bound = 2 * np.sqrt(m / k)

    H = np.random.uniform(0, upper_bound, (n, k))
    if H.ndim == 1:
        H = H.reshape(1, -1)

    return H.tolist()

def main():
    k = int(sys.argv[1])
    goal = sys.argv[2]
    file_name = sys.argv[3]

    dataMatrix = compute_data_matrix(file_name)
    d = len(dataMatrix[0])
    n = len(dataMatrix)
    
    if goal == 'symnmf':
        W = symnmfmodule.norm(dataMatrix, n, d)
        H = initialize_H(n, k, W)
        optimal_H = symnmfmodule.symnmf(W, H, n, k)
        print_matrix(optimal_H)
    elif goal == 'sym':
        A = symnmfmodule.sym(dataMatrix, n, d)
        print_matrix(A)
    elif goal == 'ddg':
        D = symnmfmodule.ddg(dataMatrix, n, d)
        print_matrix(D)
    elif goal == 'norm':
        W = symnmfmodule.norm(dataMatrix, n, d)
        print_matrix(W)
    else:
        perror("An Error Has Occurred")
        exit(1)


if __name__ == "__main__":
    main()
