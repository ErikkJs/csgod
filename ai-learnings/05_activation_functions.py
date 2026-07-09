# author: csgod cmps studies
# Activation Functions - ReLU, Sigmoid, Tanh, Softmax comparison
# Activation functions introduce non-linearity into neural networks,
# enabling them to learn complex patterns and relationships in data.

import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset
from sklearn.datasets import make_classification, make_circles
from sklearn.preprocessing import StandardScaler

class ActivationFunctions:
    @staticmethod
    def sigmoid(x):
        return 1 / (1 + np.exp(-np.clip(x, -250, 250)))
    
    @staticmethod
    def sigmoid_derivative(x):
        s = ActivationFunctions.sigmoid(x)
        return s * (1 - s)
    
    @staticmethod
    def tanh(x):
        return np.tanh(x)
    
    @staticmethod
    def tanh_derivative(x):
        return 1 - np.tanh(x) ** 2
    
    @staticmethod
    def relu(x):
        return np.maximum(0, x)
    
    @staticmethod
    def relu_derivative(x):
        return np.where(x > 0, 1, 0)
    
    @staticmethod
    def leaky_relu(x, alpha=0.01):
        return np.where(x > 0, x, alpha * x)
    
    @staticmethod
    def leaky_relu_derivative(x, alpha=0.01):
        return np.where(x > 0, 1, alpha)
    
    @staticmethod
    def elu(x, alpha=1.0):
        return np.where(x > 0, x, alpha * (np.exp(x) - 1))
    
    @staticmethod
    def elu_derivative(x, alpha=1.0):
        return np.where(x > 0, 1, alpha * np.exp(x))
    
    @staticmethod
    def swish(x, beta=1.0):
        return x * ActivationFunctions.sigmoid(beta * x)
    
    @staticmethod
    def softmax(x):
        exp_x = np.exp(x - np.max(x, axis=-1, keepdims=True))
        return exp_x / np.sum(exp_x, axis=-1, keepdims=True)

def plot_activation_functions():
    """Visualize different activation functions and their derivatives"""
    x = np.linspace(-5, 5, 1000)
    
    fig, axes = plt.subplots(2, 4, figsize=(16, 8))
    
    # Sigmoid
    axes[0, 0].plot(x, ActivationFunctions.sigmoid(x), 'b-', linewidth=2, label='Sigmoid')
    axes[0, 0].plot(x, ActivationFunctions.sigmoid_derivative(x), 'r--', linewidth=2, label="Sigmoid'")
    axes[0, 0].set_title('Sigmoid')
    axes[0, 0].grid(True)
    axes[0, 0].legend()
    axes[0, 0].set_ylim(-0.5, 1.5)
    
    # Tanh
    axes[0, 1].plot(x, ActivationFunctions.tanh(x), 'b-', linewidth=2, label='Tanh')
    axes[0, 1].plot(x, ActivationFunctions.tanh_derivative(x), 'r--', linewidth=2, label="Tanh'")
    axes[0, 1].set_title('Tanh')
    axes[0, 1].grid(True)
    axes[0, 1].legend()
    axes[0, 1].set_ylim(-1.5, 1.5)
    
    # ReLU
    axes[0, 2].plot(x, ActivationFunctions.relu(x), 'b-', linewidth=2, label='ReLU')
    axes[0, 2].plot(x, ActivationFunctions.relu_derivative(x), 'r--', linewidth=2, label="ReLU'")
    axes[0, 2].set_title('ReLU')
    axes[0, 2].grid(True)
    axes[0, 2].legend()
    axes[0, 2].set_ylim(-1, 5)
    
    # Leaky ReLU
    axes[0, 3].plot(x, ActivationFunctions.leaky_relu(x), 'b-', linewidth=2, label='Leaky ReLU')
    axes[0, 3].plot(x, ActivationFunctions.leaky_relu_derivative(x), 'r--', linewidth=2, label="Leaky ReLU'")
    axes[0, 3].set_title('Leaky ReLU')
    axes[0, 3].grid(True)
    axes[0, 3].legend()
    axes[0, 3].set_ylim(-1, 5)
    
    # ELU
    axes[1, 0].plot(x, ActivationFunctions.elu(x), 'b-', linewidth=2, label='ELU')
    axes[1, 0].plot(x, ActivationFunctions.elu_derivative(x), 'r--', linewidth=2, label="ELU'")
    axes[1, 0].set_title('ELU')
    axes[1, 0].grid(True)
    axes[1, 0].legend()
    axes[1, 0].set_ylim(-2, 5)
    
    # Swish
    axes[1, 1].plot(x, ActivationFunctions.swish(x), 'b-', linewidth=2, label='Swish')
    swish_derivative = x * ActivationFunctions.sigmoid_derivative(x) + ActivationFunctions.sigmoid(x)
    axes[1, 1].plot(x, swish_derivative, 'r--', linewidth=2, label="Swish'")
    axes[1, 1].set_title('Swish')
    axes[1, 1].grid(True)
    axes[1, 1].legend()
    axes[1, 1].set_ylim(-1, 5)
    
    # Softmax (for different inputs)
    x_soft = np.array([[1, 2, 3], [1, 1, 1], [3, 2, 1]])
    softmax_output = ActivationFunctions.softmax(x_soft)
    axes[1, 2].bar(range(3), softmax_output[0], alpha=0.7, label='Input: [1,2,3]')
    axes[1, 2].bar(range(3), softmax_output[1], alpha=0.7, label='Input: [1,1,1]')
    axes[1, 2].bar(range(3), softmax_output[2], alpha=0.7, label='Input: [3,2,1]')
    axes[1, 2].set_title('Softmax Examples')
    axes[1, 2].set_xlabel('Output Index')
    axes[1, 2].set_ylabel('Probability')
    axes[1, 2].legend()
    
    # Comparison of gradients
    axes[1, 3].plot(x, ActivationFunctions.sigmoid_derivative(x), label="Sigmoid'", linewidth=2)
    axes[1, 3].plot(x, ActivationFunctions.tanh_derivative(x), label="Tanh'", linewidth=2)
    axes[1, 3].plot(x, ActivationFunctions.relu_derivative(x), label="ReLU'", linewidth=2)
    axes[1, 3].set_title('Gradient Comparison')
    axes[1, 3].grid(True)
    axes[1, 3].legend()
    axes[1, 3].set_ylim(-0.1, 1.1)
    
    plt.tight_layout()
    plt.show()

class MLPWithActivations(nn.Module):
    def __init__(self, input_size, hidden_size, output_size, activation='relu'):
        super(MLPWithActivations, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, output_size)
        
        # Choose activation function
        if activation == 'relu':
            self.activation = nn.ReLU()
        elif activation == 'sigmoid':
            self.activation = nn.Sigmoid()
        elif activation == 'tanh':
            self.activation = nn.Tanh()
        elif activation == 'leaky_relu':
            self.activation = nn.LeakyReLU()
        elif activation == 'elu':
            self.activation = nn.ELU()
        else:
            raise ValueError(f"Unknown activation: {activation}")
    
    def forward(self, x):
        x = self.activation(self.fc1(x))
        x = self.activation(self.fc2(x))
        x = torch.sigmoid(self.fc3(x))  # Output layer always sigmoid for binary classification
        return x

def train_model(model, train_loader, criterion, optimizer, epochs=100):
    model.train()
    losses = []
    
    for epoch in range(epochs):
        epoch_loss = 0
        for batch_idx, (data, target) in enumerate(train_loader):
            optimizer.zero_grad()
            output = model(data)
            loss = criterion(output, target.float().unsqueeze(1))
            loss.backward()
            optimizer.step()
            epoch_loss += loss.item()
        
        losses.append(epoch_loss / len(train_loader))
        
        if epoch % 20 == 0:
            print(f'Epoch {epoch}, Loss: {losses[-1]:.4f}')
    
    return losses

def compare_activations():
    """Compare different activation functions on the same dataset"""
    print("=== Comparing Activation Functions ===")
    
    # Generate non-linear dataset
    X, y = make_circles(n_samples=1000, noise=0.1, factor=0.3, random_state=42)
    
    # Standardize
    scaler = StandardScaler()
    X = scaler.fit_transform(X)
    
    # Convert to PyTorch tensors
    X_tensor = torch.FloatTensor(X)
    y_tensor = torch.LongTensor(y)
    
    # Create data loader
    dataset = TensorDataset(X_tensor, y_tensor)
    train_loader = DataLoader(dataset, batch_size=32, shuffle=True)
    
    activations = ['relu', 'sigmoid', 'tanh', 'leaky_relu', 'elu']
    results = {}
    
    plt.figure(figsize=(15, 10))
    
    for i, activation in enumerate(activations):
        print(f"\nTraining with {activation} activation...")
        
        # Create model
        model = MLPWithActivations(input_size=2, hidden_size=10, output_size=1, activation=activation)
        criterion = nn.BCELoss()
        optimizer = optim.Adam(model.parameters(), lr=0.01)
        
        # Train
        losses = train_model(model, train_loader, criterion, optimizer, epochs=100)
        results[activation] = losses
        
        # Evaluate
        model.eval()
        with torch.no_grad():
            predictions = model(X_tensor)
            predicted_classes = (predictions > 0.5).float()
            accuracy = (predicted_classes.squeeze() == y_tensor.float()).float().mean()
            print(f"{activation} final accuracy: {accuracy:.4f}")
        
        # Plot decision boundary
        plt.subplot(2, 3, i + 1)
        
        # Create mesh
        h = 0.02
        x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
        y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
        xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
        
        mesh_points = torch.FloatTensor(np.c_[xx.ravel(), yy.ravel()])
        with torch.no_grad():
            Z = model(mesh_points).numpy()
        Z = Z.reshape(xx.shape)
        
        plt.contourf(xx, yy, Z, levels=50, alpha=0.6, cmap=plt.cm.RdYlBu)
        colors = ['red', 'blue']
        for j, color in enumerate(colors):
            idx = np.where(y == j)
            plt.scatter(X[idx, 0], X[idx, 1], c=color, label=f'Class {j}', alpha=0.7)
        
        plt.title(f'{activation.upper()} (Acc: {accuracy:.3f})')
        plt.xlabel('Feature 1')
        plt.ylabel('Feature 2')
    
    # Plot learning curves
    plt.subplot(2, 3, 6)
    for activation, losses in results.items():
        plt.plot(losses, label=activation, linewidth=2)
    plt.title('Learning Curves Comparison')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()

def demonstrate_vanishing_gradient():
    """Demonstrate vanishing gradient problem with sigmoid vs ReLU"""
    print("\n=== Vanishing Gradient Demonstration ===")
    
    # Create a deep network with sigmoid activations
    def create_deep_sigmoid_gradients():
        x = np.linspace(-3, 3, 100)
        gradients = []
        
        # Simulate gradients flowing backward through multiple sigmoid layers
        current_grad = np.ones_like(x)  # Start with gradient of 1
        
        for layer in range(10):  # 10 layers
            sigmoid_deriv = ActivationFunctions.sigmoid_derivative(x)
            current_grad = current_grad * sigmoid_deriv
            gradients.append(current_grad.copy())
        
        return gradients
    
    # Create gradients for ReLU
    def create_deep_relu_gradients():
        x = np.linspace(-3, 3, 100)
        gradients = []
        
        current_grad = np.ones_like(x)
        
        for layer in range(10):
            relu_deriv = ActivationFunctions.relu_derivative(x)
            current_grad = current_grad * relu_deriv
            gradients.append(current_grad.copy())
        
        return gradients
    
    sigmoid_grads = create_deep_sigmoid_gradients()
    relu_grads = create_deep_relu_gradients()
    
    plt.figure(figsize=(12, 5))
    
    # Plot gradient magnitude vs layer depth
    plt.subplot(1, 2, 1)
    sigmoid_magnitudes = [np.mean(np.abs(grad)) for grad in sigmoid_grads]
    relu_magnitudes = [np.mean(np.abs(grad)) for grad in relu_grads]
    
    layers = list(range(1, 11))
    plt.plot(layers, sigmoid_magnitudes, 'r-o', label='Sigmoid', linewidth=2)
    plt.plot(layers, relu_magnitudes, 'b-s', label='ReLU', linewidth=2)
    plt.yscale('log')
    plt.xlabel('Layer Depth')
    plt.ylabel('Average Gradient Magnitude')
    plt.title('Gradient Magnitude vs Depth')
    plt.legend()
    plt.grid(True)
    
    # Plot final gradients
    plt.subplot(1, 2, 2)
    x = np.linspace(-3, 3, 100)
    plt.plot(x, sigmoid_grads[-1], 'r-', label='Sigmoid (Layer 10)', linewidth=2)
    plt.plot(x, relu_grads[-1], 'b-', label='ReLU (Layer 10)', linewidth=2)
    plt.xlabel('Input Value')
    plt.ylabel('Gradient')
    plt.title('Final Layer Gradients')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()
    
    print(f"Sigmoid final gradient magnitude: {np.mean(np.abs(sigmoid_grads[-1])):.6f}")
    print(f"ReLU final gradient magnitude: {np.mean(np.abs(relu_grads[-1])):.6f}")

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Activation Functions Visualization ===")
    plot_activation_functions()
    
    # Compare activations on a classification task
    compare_activations()
    
    # Demonstrate vanishing gradient problem
    demonstrate_vanishing_gradient()
    
    # Show properties of different activations
    print("\n=== Activation Function Properties ===")
    properties = {
        'Sigmoid': {
            'Range': '(0, 1)',
            'Derivative Range': '(0, 0.25)',
            'Zero-Centered': False,
            'Monotonic': True,
            'Vanishing Gradient': True
        },
        'Tanh': {
            'Range': '(-1, 1)',
            'Derivative Range': '(0, 1)',
            'Zero-Centered': True,
            'Monotonic': True,
            'Vanishing Gradient': True
        },
        'ReLU': {
            'Range': '[0, ∞)',
            'Derivative Range': '{0, 1}',
            'Zero-Centered': False,
            'Monotonic': True,
            'Vanishing Gradient': False
        },
        'Leaky ReLU': {
            'Range': '(-∞, ∞)',
            'Derivative Range': '{α, 1}',
            'Zero-Centered': False,
            'Monotonic': True,
            'Vanishing Gradient': False
        },
        'ELU': {
            'Range': '(-α, ∞)',
            'Derivative Range': '(0, 1)',
            'Zero-Centered': False,
            'Monotonic': True,
            'Vanishing Gradient': False
        }
    }
    
    for activation, props in properties.items():
        print(f"\n{activation}:")
        for prop, value in props.items():
            print(f"  {prop}: {value}")