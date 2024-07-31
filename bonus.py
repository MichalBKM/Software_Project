from sklearn.datasets import load_iris
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':
    
    np.random.seed(0)
    iris = load_iris()
    data = iris.data

    x_axis = range(1, 11)

    inertia_values = []

    for k in x_axis:
        kmeans = KMeans(n_clusters=k, init='k-means++', random_state=0)
        kmeans.fit(data)
        inertia_values.append(kmeans.inertia_)

    #Plotting the Graph - Elbow Method
    plt.plot(x_axis, inertia_values, marker='o', color='red')
    plt.xlabel('Number of clusters (k)')
    plt.ylabel('Inertia')
    plt.grid(which='both')
    plt.title('Elbow Method for selection of optimal K clusters',weight='bold')
    plt.savefig("elbow.png",dpi=150)




