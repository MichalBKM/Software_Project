import numpy as np
import mykmeanssp as c

if __name__ :
    k = 3
    d = 2
    eps = 0.1
    data  = [[5, 7], [7,7], [7,5], [5 ,5], [-8, 5], [-8, 7], [-10, -10]]
    centroids = [[-10,-10], [-8, 7], [5,7]]

    iter = 200
    print(c.fit(2, 7, 3, iter, eps, centroids, data))
