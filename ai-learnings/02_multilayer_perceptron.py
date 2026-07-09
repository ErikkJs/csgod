# author: csgod cmps studies
# Multi-Layer Perceptron (MLP) - Basic feedforward neural network
# An MLP consists of multiple layers of perceptrons with non-linear activation functions,
# trained using backpropagation to learn complex patterns in data.

import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_circles, make_classification
from sklearn.preprocessing import StandardScaler

class MLP:
    def __init__(self, layers, learning_rate=0.01, epochs=1000):
        self.layers = layers  # List of layer sizes [input, hidden1, hidden2, ..., output]
        self.learning_rate = learning_rate
        self.epochs = epochs
        self.weights = []
        self.biases = []
        
        # Initialize weights and biases
        for i in range(len(layers) - 1):
            # Xavier initialization
            w = np.random.randn(layers[i], layers[i+1]) * np.sqrt(2.0 / layers[i])
            b = np.zeros((1, layers[i+1]))
            self.weights.append(w)
            self.biases.append(b)
    
    def sigmoid(self, x):
        return 1 / (1 + np.exp(-np.clip(x, -250, 250)))  # Clip to prevent overflow
    
    def sigmoid_derivative(self, x):
        return x * (1 - x)
    
    def relu(self, x):
        return np.maximum(0, x)
    
    def relu_derivative(self, x):
        return np.where(x > 0, 1, 0)
    
    def forward(self, X):
        self.activations = [X]
        self.z_values = []
        
        for i in range(len(self.weights)):
            z = np.dot(self.activations[-1], self.weights[i]) + self.biases[i]
            self.z_values.append(z)
            
            if i == len(self.weights) - 1:  # Output layer
                a = self.sigmoid(z)
            else:  # Hidden layers
                a = self.relu(z)
            
            self.activations.append(a)
        
        return self.activations[-1]
    
    def backward(self, X, y, output):
        m = X.shape[0]
        
        # Calculate output layer error
        dz = output - y
        
        # Backpropagate through layers
        for i in reversed(range(len(self.weights))):
            # Calculate gradients
            dw = (1/m) * np.dot(self.activations[i].T, dz)
            db = (1/m) * np.sum(dz, axis=0, keepdims=True)
            
            # Update weights and biases
            self.weights[i] -= self.learning_rate * dw
            self.biases[i] -= self.learning_rate * db
            
            # Calculate error for previous layer (if not input layer)
            if i > 0:
                dz = np.dot(dz, self.weights[i].T) * self.relu_derivative(self.activations[i])
    
    def train(self, X, y):
        self.costs = []
        
        for epoch in range(self.epochs):
            # Forward pass
            output = self.forward(X)
            
            # Calculate cost (binary cross-entropy)
            cost = -np.mean(y * np.log(output + 1e-8) + (1-y) * np.log(1-output + 1e-8))
            self.costs.append(cost)
            
            # Backward pass
            self.backward(X, y, output)
            
            if epoch % 100 == 0:
                print(f"Epoch {epoch}, Cost: {cost:.4f}")
    
    def predict(self, X):
        output = self.forward(X)
        return (output > 0.5).astype(int)
    
    def predict_proba(self, X):
        return self.forward(X)

# Example usage and demonstration
if __name__ == "__main__":
    # Example 1: XOR problem (non-linearly separable)
    print("=== XOR Problem ===")
    X_xor = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
    y_xor = np.array([[0], [1], [1], [0]])
    
    # Create and train MLP for XOR
    mlp_xor = MLP(layers=[2, 4, 1], learning_rate=0.5, epochs=2000)
    mlp_xor.train(X_xor, y_xor)
    
    # Test predictions
    predictions = mlp_xor.predict(X_xor)
    print(f"XOR Predictions: {predictions.flatten()}")
    print(f"XOR Actual:      {y_xor.flatten()}")
    print(f"XOR Accuracy:    {np.mean(predictions.flatten() == y_xor.flatten()):.2f}")
    
    # Example 2: Circles dataset (non-linearly separable)
    print("\n=== Circles Dataset ===")
    X_circles, y_circles = make_circles(n_samples=1000, noise=0.1, factor=0.3, random_state=42)
    y_circles = y_circles.reshape(-1, 1)
    
    # Standardize features
    scaler = StandardScaler()
    X_circles_scaled = scaler.fit_transform(X_circles)
    
    # Split into train and test
    split = int(0.8 * len(X_circles_scaled))
    X_train, X_test = X_circles_scaled[:split], X_circles_scaled[split:]
    y_train, y_test = y_circles[:split], y_circles[split:]
    
    # Create and train MLP
    mlp_circles = MLP(layers=[2, 8, 4, 1], learning_rate=0.01, epochs=1000)
    mlp_circles.train(X_train, y_train)
    
    # Evaluate
    train_accuracy = np.mean(mlp_circles.predict(X_train) == y_train)
    test_accuracy = np.mean(mlp_circles.predict(X_test) == y_test)
    print(f"Train Accuracy: {train_accuracy:.2f}")
    print(f"Test Accuracy:  {test_accuracy:.2f}")
    
    # Visualization
    plt.figure(figsize=(15, 5))
    
    # Plot 1: XOR learning curve
    plt.subplot(1, 3, 1)
    plt.plot(mlp_xor.costs)
    plt.title('XOR Learning Curve')
    plt.xlabel('Epoch')
    plt.ylabel('Cost')
    plt.grid(True)
    
    # Plot 2: Circles learning curve
    plt.subplot(1, 3, 2)
    plt.plot(mlp_circles.costs)
    plt.title('Circles Learning Curve')
    plt.xlabel('Epoch')
    plt.ylabel('Cost')
    plt.grid(True)
    
    # Plot 3: Decision boundary for circles
    plt.subplot(1, 3, 3)
    h = 0.02
    x_min, x_max = X_circles_scaled[:, 0].min() - 1, X_circles_scaled[:, 0].max() + 1
    y_min, y_max = X_circles_scaled[:, 1].min() - 1, X_circles_scaled[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
    
    mesh_points = np.c_[xx.ravel(), yy.ravel()]
    Z = mlp_circles.predict_proba(mesh_points)
    Z = Z.reshape(xx.shape)
    
    plt.contourf(xx, yy, Z, levels=50, alpha=0.6, cmap=plt.cm.RdYlBu)
    colors = ['red', 'blue']
    for i, color in enumerate(colors):
        idx = np.where(y_circles.flatten() == i)
        plt.scatter(X_circles_scaled[idx, 0], X_circles_scaled[idx, 1], 
                   c=color, label=f'Class {i}', alpha=0.7)
    
    plt.title('MLP Decision Boundary')
    plt.xlabel('Feature 1')
    plt.ylabel('Feature 2')
    plt.legend()
    
    plt.tight_layout()
    plt.show()