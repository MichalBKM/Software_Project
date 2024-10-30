import sys
import math
import numpy as np
import symnmf

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
    """Prints a matrix with 4 decimal places."""
    for row in matrix:
        print(",".join(f"{x:.4f}" for x in row))

def initialize_H(n, k, W):
    """
    Initializes matrix H with random values in the range [0, 2 * sqrt(m / k)].
    
    Parameters:
        n (int): Number of rows in H.
        k (int): Number of columns in H (number of clusters).
        W (numpy.ndarray): The normalized similarity matrix.

    Returns:
        numpy.ndarray: Initialized matrix H of size (n, k).
    """

    # Calculate the average of all entries in W
    m = np.mean(W)

    # Determine the range for random initialization
    upper_bound = 2 * np.sqrt(m / k)

    # Initialize H with random values from the uniform distribution in [0, upper_bound]
    H = np.random.uniform(0, upper_bound, (n, k))

    return H


if __name__ == "__main__":
    k = int(sys.argv[1])
    goal = sys.argv[2]
    file_name = sys.argv[3]

    dataMatrix = compute_data_matrix(file_name)
    d = len(dataMatrix[0])
    n = len(dataMatrix)
    
    if goal == 'symnmf':
        W = symnmf.norm(dataMatrix, n, d)
        H = initialize_H(n, k, W)
        optimal_H = symnmf.symnmf(W, H, n, d, k)
        print_matrix(optimal_H)
    elif goal == 'sym':
        A = symnmf.sym(dataMatrix, n, d)
        print_matrix(A)
    elif goal == 'ddg':
        D = symnmf.ddg(dataMatrix, n, d)
        print(D)
    elif goal == 'norm':
        W = symnmf.norm(dataMatrix, n, d)
        print(W)
    else:
        perror("Invalid Goal!")
        exit(1)
