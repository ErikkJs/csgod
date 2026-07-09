# author: csgod cmps studies
# Perceptron - Single layer linear classifier
# A perceptron is the simplest neural network unit that learns a linear decision boundary
# to separate two classes using the perceptron learning algorithm.

import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_blobs

class Perceptron:
    def __init__(self, learning_rate=0.01, n_iterations=1000):
        self.learning_rate = learning_rate
        self.n_iterations = n_iterations
        self.weights = None
        self.bias = None
    
    def fit(self, X, y):
        # Initialize weights and bias
        n_samples, n_features = X.shape
        self.weights = np.zeros(n_features)
        self.bias = 0
        
        # Convert labels to -1 and 1
        y_ = np.where(y <= 0, -1, 1)
        
        # Training loop
        for i in range(self.n_iterations):
            for idx, x_i in enumerate(X):
                # Calculate linear output
                linear_output = np.dot(x_i, self.weights) + self.bias
                
                # Apply activation function (sign)
                y_predicted = np.where(linear_output >= 0, 1, -1)
                
                # Update weights if prediction is wrong
                if y_[idx] != y_predicted:
                    self.weights += self.learning_rate * y_[idx] * x_i
                    self.bias += self.learning_rate * y_[idx]
    
    def predict(self, X):
        linear_output = np.dot(X, self.weights) + self.bias
        predictions = np.where(linear_output >= 0, 1, -1)
        return predictions

# Example usage and demonstration
if __name__ == "__main__":
    # Generate synthetic linearly separable data
    X, y = make_blobs(n_samples=100, centers=2, n_features=2, 
                      random_state=42, cluster_std=1.5)
    
    # Convert labels to binary (0, 1)
    y = np.where(y == 0, -1, 1)
    
    # Create and train perceptron
    perceptron = Perceptron(learning_rate=0.01, n_iterations=1000)
    perceptron.fit(X, y)
    
    # Make predictions
    predictions = perceptron.predict(X)
    
    # Calculate accuracy
    accuracy = np.mean(predictions == y)
    print(f"Accuracy: {accuracy:.2f}")
    
    # Visualize results
    plt.figure(figsize=(10, 8))
    
    # Plot data points
    plt.scatter(X[y == 1, 0], X[y == 1, 1], c='red', marker='o', label='Class 1')
    plt.scatter(X[y == -1, 0], X[y == -1, 1], c='blue', marker='s', label='Class -1')
    
    # Plot decision boundary
    ax = plt.gca()
    xlim = ax.get_xlim()
    ylim = ax.get_ylim()
    
    # Create grid to evaluate model
    xx = np.linspace(xlim[0], xlim[1], 30)
    yy = np.linspace(ylim[0], ylim[1], 30)
    YY, XX = np.meshgrid(yy, xx)
    xy = np.vstack([XX.ravel(), YY.ravel()]).T
    Z = perceptron.predict(xy).reshape(XX.shape)
    
    # Plot decision boundary and margins
    plt.contour(XX, YY, Z, colors='black', levels=[0], alpha=0.8, linestyles='--')
    
    plt.title('Perceptron Classification')
    plt.xlabel('Feature 1')
    plt.ylabel('Feature 2')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.show()
    
    print(f"Final weights: {perceptron.weights}")
    print(f"Final bias: {perceptron.bias}")