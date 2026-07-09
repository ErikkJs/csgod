# author: csgod cmps studies
# Backpropagation - Gradient descent training algorithm
# Backpropagation computes gradients of the loss function with respect to network weights
# using the chain rule, enabling efficient training of multi-layer neural networks.

import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_classification
from sklearn.preprocessing import StandardScaler

class BackpropNetwork:
    def __init__(self, layer_sizes, learning_rate=0.01):
        self.layer_sizes = layer_sizes
        self.learning_rate = learning_rate
        self.weights = []
        self.biases = []
        
        # Initialize weights using Xavier initialization
        for i in range(len(layer_sizes) - 1):
            w = np.random.randn(layer_sizes[i], layer_sizes[i+1]) * np.sqrt(2.0 / layer_sizes[i])
            b = np.zeros((1, layer_sizes[i+1]))
            self.weights.append(w)
            self.biases.append(b)
    
    def sigmoid(self, x):
        return 1 / (1 + np.exp(-np.clip(x, -250, 250)))
    
    def sigmoid_derivative(self, x):
        return x * (1 - x)
    
    def forward_pass(self, X):
        # Store activations and z values for backpropagation
        self.activations = [X]
        self.z_values = []
        
        current_input = X
        for i in range(len(self.weights)):
            # Linear transformation
            z = np.dot(current_input, self.weights[i]) + self.biases[i]
            self.z_values.append(z)
            
            # Apply activation function
            a = self.sigmoid(z)
            self.activations.append(a)
            current_input = a
        
        return self.activations[-1]
    
    def backward_pass(self, X, y, y_pred):
        m = X.shape[0]  # Number of samples
        
        # Initialize gradients
        weight_gradients = []
        bias_gradients = []
        
        # Start with output layer error
        # For binary cross-entropy with sigmoid: dL/dz = y_pred - y
        delta = y_pred - y
        
        # Backpropagate through each layer
        for i in reversed(range(len(self.weights))):
            # Compute gradients for current layer
            dW = (1/m) * np.dot(self.activations[i].T, delta)
            db = (1/m) * np.sum(delta, axis=0, keepdims=True)
            
            weight_gradients.insert(0, dW)
            bias_gradients.insert(0, db)
            
            # Compute delta for previous layer (if not input layer)
            if i > 0:
                # Chain rule: delta = (W^T * delta) * sigmoid'(a)
                delta = np.dot(delta, self.weights[i].T) * self.sigmoid_derivative(self.activations[i])
        
        return weight_gradients, bias_gradients
    
    def update_parameters(self, weight_gradients, bias_gradients):
        # Update weights and biases using computed gradients
        for i in range(len(self.weights)):
            self.weights[i] -= self.learning_rate * weight_gradients[i]
            self.biases[i] -= self.learning_rate * bias_gradients[i]
    
    def compute_loss(self, y_true, y_pred):
        # Binary cross-entropy loss
        epsilon = 1e-8  # Small value to prevent log(0)
        return -np.mean(y_true * np.log(y_pred + epsilon) + 
                       (1 - y_true) * np.log(1 - y_pred + epsilon))
    
    def train(self, X, y, epochs=1000, verbose=True):
        self.losses = []
        self.accuracies = []
        
        for epoch in range(epochs):
            # Forward pass
            y_pred = self.forward_pass(X)
            
            # Compute loss
            loss = self.compute_loss(y, y_pred)
            self.losses.append(loss)
            
            # Compute accuracy
            predictions = (y_pred > 0.5).astype(int)
            accuracy = np.mean(predictions == y)
            self.accuracies.append(accuracy)
            
            # Backward pass
            weight_gradients, bias_gradients = self.backward_pass(X, y, y_pred)
            
            # Update parameters
            self.update_parameters(weight_gradients, bias_gradients)
            
            if verbose and epoch % 100 == 0:
                print(f"Epoch {epoch}: Loss = {loss:.4f}, Accuracy = {accuracy:.4f}")
    
    def predict(self, X):
        y_pred = self.forward_pass(X)
        return (y_pred > 0.5).astype(int)
    
    def predict_proba(self, X):
        return self.forward_pass(X)

def visualize_gradient_flow(network, X, y):
    """Visualize how gradients flow through the network"""
    # Perform one forward and backward pass
    y_pred = network.forward_pass(X)
    weight_gradients, bias_gradients = network.backward_pass(X, y, y_pred)
    
    # Plot gradient magnitudes for each layer
    plt.figure(figsize=(12, 4))
    
    # Weight gradients
    plt.subplot(1, 2, 1)
    gradient_norms = [np.linalg.norm(wg) for wg in weight_gradients]
    layers = [f'Layer {i+1}' for i in range(len(gradient_norms))]
    plt.bar(layers, gradient_norms)
    plt.title('Weight Gradient Magnitudes')
    plt.ylabel('Gradient Norm')
    plt.xticks(rotation=45)
    
    # Bias gradients
    plt.subplot(1, 2, 2)
    bias_gradient_norms = [np.linalg.norm(bg) for bg in bias_gradients]
    plt.bar(layers, bias_gradient_norms)
    plt.title('Bias Gradient Magnitudes')
    plt.ylabel('Gradient Norm')
    plt.xticks(rotation=45)
    
    plt.tight_layout()
    plt.show()

def demonstrate_chain_rule():
    """Demonstrate the chain rule computation manually"""
    print("=== Manual Chain Rule Demonstration ===")
    
    # Simple example: f(x) = sigmoid(w2 * sigmoid(w1 * x + b1) + b2)
    x = 2.0
    w1, b1 = 0.5, 0.1
    w2, b2 = 0.3, 0.2
    target = 1.0
    
    # Forward pass
    z1 = w1 * x + b1
    a1 = 1 / (1 + np.exp(-z1))  # sigmoid
    z2 = w2 * a1 + b2
    a2 = 1 / (1 + np.exp(-z2))  # sigmoid
    
    print(f"Forward pass:")
    print(f"z1 = {z1:.4f}, a1 = {a1:.4f}")
    print(f"z2 = {z2:.4f}, a2 = {a2:.4f}")
    
    # Loss (squared error for simplicity)
    loss = 0.5 * (a2 - target)**2
    print(f"Loss = {loss:.4f}")
    
    # Backward pass using chain rule
    # dL/da2
    dL_da2 = a2 - target
    
    # dL/dz2 = dL/da2 * da2/dz2
    da2_dz2 = a2 * (1 - a2)  # sigmoid derivative
    dL_dz2 = dL_da2 * da2_dz2
    
    # dL/dw2 = dL/dz2 * dz2/dw2
    dz2_dw2 = a1
    dL_dw2 = dL_dz2 * dz2_dw2
    
    # dL/da1 = dL/dz2 * dz2/da1
    dz2_da1 = w2
    dL_da1 = dL_dz2 * dz2_da1
    
    # dL/dz1 = dL/da1 * da1/dz1
    da1_dz1 = a1 * (1 - a1)  # sigmoid derivative
    dL_dz1 = dL_da1 * da1_dz1
    
    # dL/dw1 = dL/dz1 * dz1/dw1
    dz1_dw1 = x
    dL_dw1 = dL_dz1 * dz1_dw1
    
    print(f"\nBackward pass (chain rule):")
    print(f"dL/dw2 = {dL_dw2:.4f}")
    print(f"dL/dw1 = {dL_dw1:.4f}")
    
    return dL_dw1, dL_dw2

# Example usage and demonstration
if __name__ == "__main__":
    # Demonstrate chain rule manually
    demonstrate_chain_rule()
    
    # Generate synthetic dataset
    print("\n=== Backpropagation Network Training ===")
    X, y = make_classification(n_samples=1000, n_features=2, n_redundant=0, 
                             n_informative=2, n_clusters_per_class=1, 
                             random_state=42)
    y = y.reshape(-1, 1)
    
    # Standardize features
    scaler = StandardScaler()
    X = scaler.fit_transform(X)
    
    # Split data
    split = int(0.8 * len(X))
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]
    
    # Create and train network
    network = BackpropNetwork(layer_sizes=[2, 8, 4, 1], learning_rate=0.1)
    
    print("Training network with backpropagation...")
    network.train(X_train, y_train, epochs=1000, verbose=True)
    
    # Evaluate
    train_accuracy = np.mean(network.predict(X_train) == y_train)
    test_accuracy = np.mean(network.predict(X_test) == y_test)
    
    print(f"\nFinal Results:")
    print(f"Train Accuracy: {train_accuracy:.4f}")
    print(f"Test Accuracy: {test_accuracy:.4f}")
    
    # Visualizations
    plt.figure(figsize=(15, 5))
    
    # Loss curve
    plt.subplot(1, 3, 1)
    plt.plot(network.losses)
    plt.title('Training Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.grid(True)
    
    # Accuracy curve
    plt.subplot(1, 3, 2)
    plt.plot(network.accuracies)
    plt.title('Training Accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.grid(True)
    
    # Decision boundary
    plt.subplot(1, 3, 3)
    h = 0.02
    x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))
    
    mesh_points = np.c_[xx.ravel(), yy.ravel()]
    Z = network.predict_proba(mesh_points)
    Z = Z.reshape(xx.shape)
    
    plt.contourf(xx, yy, Z, levels=50, alpha=0.6, cmap=plt.cm.RdYlBu)
    colors = ['red', 'blue']
    for i, color in enumerate(colors):
        idx = np.where(y.flatten() == i)
        plt.scatter(X[idx, 0], X[idx, 1], c=color, label=f'Class {i}')
    
    plt.title('Decision Boundary')
    plt.xlabel('Feature 1')
    plt.ylabel('Feature 2')
    plt.legend()
    
    plt.tight_layout()
    plt.show()
    
    # Visualize gradient flow
    print("\nVisualizing gradient flow...")
    visualize_gradient_flow(network, X_train, y_train)