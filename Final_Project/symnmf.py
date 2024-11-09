import sys
import numpy as np
import symnmfmodule

np.random.seed(1234)

def compute_data_matrix(filename):
    """
    Reads a txt file containing a matrix and converts it to a 2D array of float type.
    
    Parameters:
    filename (str): The path to the file containing the matrix.

    Returns:
    matrix: A 2D list of numbers.
    """
    file = open(filename, 'r')
    matrix = []
    for row in file:
        matrix.append([float(x) for x in row.split(",")])

    file.close()
    return matrix

def print_matrix(matrix):
    """
    Prints a matrix.

    Parameters:
    matrix (list): A 2D list of numbers.
    """
    for row in matrix:
        print(",".join(f"{x:.4f}" for x in row))

def initialize_H(n, k, W):
    """
    Initializes matrix H of size n X k with random values bounded by an upper bound.
    
    Parameters:
    n (int): Number of rows in matrix H.
    k (int): Number of columns in matrix H.
    W (list): The data matrix used to compute the upper bound for initializing H.

    Returns:
    H: A 2D list representing the initialized matrix H.
    """
    m = np.mean(W)
    upper_bound = 2 * np.sqrt(m / k)

    H = np.random.uniform(0, upper_bound, (n, k))
    if H.ndim == 1:
        H = H.reshape(1, -1)

    return H.tolist()

def main():
    try:
        k = int(sys.argv[1])
        goal = sys.argv[2]
        file_name = sys.argv[3]

        dataMatrix = compute_data_matrix(file_name)
        n = len(dataMatrix)
        
        if goal == 'symnmf':
            W = symnmfmodule.norm(dataMatrix)
            H = initialize_H(n, k, W)
            optimal_H = symnmfmodule.symnmf(W, H, n, k)
            print_matrix(optimal_H)
        elif goal == 'sym':
            A = symnmfmodule.sym(dataMatrix)
            print_matrix(A)
        elif goal == 'ddg':
            D = symnmfmodule.ddg(dataMatrix)
            print_matrix(D)
        elif goal == 'norm':
            W = symnmfmodule.norm(dataMatrix)
            print_matrix(W)
        else:
            print("An Error Has Occurred")
            exit(1)
    except ValueError:
        print("An Error Has Occurred")
        exit(1)


if __name__ == "__main__":
    main()
