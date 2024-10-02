import sys
import math
import numpy as np

np.random.seed(1234)

if __name__ == "__main__":
    k = int(sys.argv[1])
    goal = sys.argv[2]
    file_name = sys.argv[3]

    if goal == 'symnmf':
        #TODO
    elif goal == 'sym':
        #TODO
    elif goal == 'ddg':
        #TODO
    elif goal == 'norm':
        #TODO
    else:
        print("Invalid Goal!")
        exit(1)
