from sklearn.datasets import load_iris
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':
    # Example data
    # Assuming you have your data in a DataFrame or NumPy array called `data`
    # data = pd.read_csv('your_data.csv').to_numpy()

    # For demonstration, let's create a sample dataset
    np.random.seed(0)
    data = load_iris()  # 100 data points with 2 features

    # Define the range for k
    k_values = range(1, 11)

    # Store the inertia values for each k
    inertia_values = []

    for k in k_values:
        kmeans = KMeans(n_clusters=k, init='k-means++', random_state=0)
        kmeans.fit(data)
        inertia_values.append(kmeans.inertia_)

    # Plotting the inertia values to observe the elbow method
    plt.plot(k_values, inertia_values, marker='o')
    plt.xlabel('Number of clusters (k)')
    plt.ylabel('Inertia')
    plt.title('Elbow Method For Optimal k')
    plt.savefig("elbow.png")




