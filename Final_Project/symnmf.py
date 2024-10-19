import sys
import math
import numpy as np
import symnmf #importing the C-PYTHON API

np.random.seed(1234)

def main():
    #Reading user CMD Arguments
    k = int(sys.argv[1]) #Number of required clusters
    goal = sys.argv[2]
    file_name = sys.argv[3]

#אביב, כאן צריך לקרוא לפונקציות בהתאם למודול
    if goal == 'symnmf':
        #TODO
    elif goal == 'sym':
        result = symnmf.sym()
    elif goal == 'ddg':
        #TODO
    elif goal == 'norm':
        #TODO
    else:
        print("Invalid Goal!")
        exit(1)

def initialize_H(W,k):
    m = np.mean(W)
    return np.random.uniform(0, 2*math.sqrt((m/k)), size=(len(W),k))

if __name__ == "__main__":
    main()