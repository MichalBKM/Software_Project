import numpy as np
import random
import math
import sys
import pandas as pd

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

    df = pd.merge(df1,df2,on='key', how='inner')
    df.sort_values(by='key', inplace=True)
    df = df.iloc[:,1:] #after merging by key, we need to remove it
    matrix = df.to_numpy(dtype=float)
    print(matrix)
    return matrix

#calculating euclidean distance from two d-dimensional vectors V
def vector_distance(x,y):
    res = 0
    d = len(x)
    for i in range(d):
        res += math.pow((x[i]-y[i]), 2)
    res = math.sqrt(res)
    return res


'''    for i in range(n): #for each datapoint
        if np.array_equal(datapoints[i], cent)==False:
            dx = vector_distance(datapoints[i], cent)
            distances.append((i,dx))
    print("distances:\n",distances)
    prob = [distances[i][1] for i in range(n)]
    '''

def kmeans_pp(k, iter, eps, file_name_1, file_name_2):
    datapoints = join_dataframes(file_name_1, file_name_2)
    n = len(datapoints)
    centroids = [datapoints[np.random.choice(n)]] #choose one center uniformly at random among the datapoints
    print("centroids:\n",centroids)
    distances = []
    while len(centroids) != 2:
        for i in range(n): #for each datapoint
            dx = []
            for j in range(len(centroids)): #for each centroid
                dx.append(vector_distance(datapoints[i],centroids[j]))
                distances.append((i,dx))
    print("distances:\n",distances)

        #total = sum([distances[i][1] for i in range(n)])
        #prob = [distances[i][1] / total for i in range(n)]
        #print(prob)
        #choice1 = random.choices(datapoints,weights=prob,k=1)
        #print("choice:\n",choice1)




#fix with try-except like Aviv did in HW1
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
    kmeans_pp(k, iter, eps, file_name_1, file_name_2)

