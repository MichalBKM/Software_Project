import math
import sys
import pandas as pd
import numpy as np
import mykmeanssp as c

'''
1. reads file1 and file2
2. combines dataframes by inner join
3. sorts merged dataframe in ascending order
4. creates a matrix out of df and returns it
'''


def join_dataframes(file1, file2):
    df1 = pd.read_csv(file1, sep=",", header=None)
    df2 = pd.read_csv(file2, sep=",", header=None)

    df1.rename(columns={0: 'key'}, inplace=True)
    df2.rename(columns={0: 'key'}, inplace=True)

    df = pd.merge(df1, df2, on='key', how='inner')
    df.sort_values(by='key', inplace=True)
    df = df.iloc[:, 1:]  # after merging by key, we need to remove it
    matrix = df.to_numpy(dtype=float)
    return matrix


# calculating euclidean distance from two d-dimensional vectors V
def vector_distance(x, y):
    res = 0
    d = len(x)
    for i in range(d):
        res += math.pow((x[i] - y[i]), 2)
    res = math.sqrt(res)
    return res


def kmeans_pp(k, iter, eps, file_name_1, file_name_2):
    np.random.seed(1234)
    datapoints = join_dataframes(file_name_1, file_name_2)
    indices = []
    # print("datapoints:\n",datapoints)
    n = len(datapoints)
    index = np.random.randint(0, n)
    indices.append(index)
    centroids = np.array(
        [datapoints[index]])  # choose one center uniformly at random among the datapoints
    # print("first centroid:\n", centroids)
    while len(centroids) != k:
        dist = [min([vector_distance(datapoints[i], centroids[j]) for j in range(len(centroids))]) for i in range(n)]
        # print("dist:\n",dist)
        total = sum(dist[i] for i in range(n))
        # print("total:\n",total)
        prob = [dist[i] / total for i in range(n)]
        # print("prob:\n",prob)

        index = np.random.choice(range(n), p=prob)
        chosen_cent = datapoints[index]
        indices.append(index)
        # print("chosen centroid:\n", chosen_cent)
        centroids = np.vstack([centroids, chosen_cent])
        # print("centroids:\n",centroids)
    print(','.join(f'{value:}' for value in indices))
    centroids = np.array(c.fit(len(datapoints[0]), n, k, iter, eps, centroids.tolist(), datapoints.tolist()))
    return centroids


def print_matrix(mat):
    for row in mat:
        print(','.join(f'{value:.4f}' for value in row))

# fix with try-except like Aviv did in HW1
if __name__ == '__main__':
    if len(sys.argv) < 5 or len(sys.argv) > 6:
        print("Invalid Input!")
        exit(1)

    k = int(sys.argv[1])
    if k != float(sys.argv[1]):
        print("Invalid number of clusters!")
        exit(1)
    iter = int(sys.argv[2]) if len(sys.argv) == 6 else 300
    if len(sys.argv) == 6 and iter != float(iter):
        print("Invalid maximum iteration!")
        exit(1)
    eps = float(sys.argv[3]) if len(sys.argv) == 6 else float(sys.argv[2])
    file_name_1 = sys.argv[4] if len(sys.argv) == 6 else sys.argv[3]
    file_name_2 = sys.argv[5] if len(sys.argv) == 6 else sys.argv[4]
    kcentroids = kmeans_pp(k, iter, eps, file_name_1, file_name_2)
    print_matrix(kcentroids)
