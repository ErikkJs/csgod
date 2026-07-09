# author: csgod cmps studies
# Optimizers - SGD, Adam, RMSprop, AdaGrad
# Optimizers determine how neural network parameters are updated based on gradients
# to minimize the loss function during training.

import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset
from sklearn.datasets import make_classification
from sklearn.preprocessing import StandardScaler

class Optimizer:
    """Base optimizer class"""
    def __init__(self, parameters, learning_rate=0.01):
        self.parameters = parameters
        self.learning_rate = learning_rate
    
    def zero_grad(self):
        for param in self.parameters:
            if param['grad'] is not None:
                param['grad'].fill_(0)
    
    def step(self):
        raise NotImplementedError

class SGD(Optimizer):
    """Stochastic Gradient Descent"""
    def __init__(self, parameters, learning_rate=0.01, momentum=0.0, weight_decay=0.0):
        super().__init__(parameters, learning_rate)
        self.momentum = momentum
        self.weight_decay = weight_decay
        self.velocity = {}
        
        # Initialize velocity for each parameter
        for i, param in enumerate(self.parameters):
            self.velocity[i] = np.zeros_like(param['data'])
    
    def step(self):
        for i, param in enumerate(self.parameters):
            if param['grad'] is None:
                continue
            
            grad = param['grad']
            
            # Add weight decay
            if self.weight_decay != 0:
                grad = grad + self.weight_decay * param['data']
            
            # Update velocity with momentum
            self.velocity[i] = self.momentum * self.velocity[i] - self.learning_rate * grad
            
            # Update parameters
            param['data'] += self.velocity[i]

class Adam(Optimizer):
    """Adaptive Moment Estimation"""
    def __init__(self, parameters, learning_rate=0.001, beta1=0.9, beta2=0.999, eps=1e-8):
        super().__init__(parameters, learning_rate)
        self.beta1 = beta1
        self.beta2 = beta2
        self.eps = eps
        self.t = 0  # time step
        
        # Initialize moment estimates
        self.m = {}  # First moment
        self.v = {}  # Second moment
        
        for i, param in enumerate(self.parameters):
            self.m[i] = np.zeros_like(param['data'])
            self.v[i] = np.zeros_like(param['data'])
    
    def step(self):
        self.t += 1
        
        for i, param in enumerate(self.parameters):
            if param['grad'] is None:
                continue
            
            grad = param['grad']
            
            # Update biased first moment estimate
            self.m[i] = self.beta1 * self.m[i] + (1 - self.beta1) * grad
            
            # Update biased second moment estimate
            self.v[i] = self.beta2 * self.v[i] + (1 - self.beta2) * grad**2
            
            # Compute bias-corrected first moment estimate
            m_hat = self.m[i] / (1 - self.beta1**self.t)
            
            # Compute bias-corrected second moment estimate
            v_hat = self.v[i] / (1 - self.beta2**self.t)
            
            # Update parameters
            param['data'] -= self.learning_rate * m_hat / (np.sqrt(v_hat) + self.eps)

class RMSprop(Optimizer):
    """Root Mean Square Propagation"""
    def __init__(self, parameters, learning_rate=0.01, alpha=0.99, eps=1e-8):
        super().__init__(parameters, learning_rate)
        self.alpha = alpha
        self.eps = eps
        
        # Initialize squared gradient accumulator
        self.square_avg = {}
        
        for i, param in enumerate(self.parameters):
            self.square_avg[i] = np.zeros_like(param['data'])
    
    def step(self):
        for i, param in enumerate(self.parameters):
            if param['grad'] is None:
                continue
            
            grad = param['grad']
            
            # Update squared gradient moving average
            self.square_avg[i] = self.alpha * self.square_avg[i] + (1 - self.alpha) * grad**2
            
            # Update parameters
            param['data'] -= self.learning_rate * grad / (np.sqrt(self.square_avg[i]) + self.eps)

class AdaGrad(Optimizer):
    """Adaptive Gradient Algorithm"""
    def __init__(self, parameters, learning_rate=0.01, eps=1e-8):
        super().__init__(parameters, learning_rate)
        self.eps = eps
        
        # Initialize accumulated squared gradients
        self.acc_grad_sq = {}
        
        for i, param in enumerate(self.parameters):
            self.acc_grad_sq[i] = np.zeros_like(param['data'])
    
    def step(self):
        for i, param in enumerate(self.parameters):
            if param['grad'] is None:
                continue
            
            grad = param['grad']
            
            # Accumulate squared gradients
            self.acc_grad_sq[i] += grad**2
            
            # Update parameters
            param['data'] -= self.learning_rate * grad / (np.sqrt(self.acc_grad_sq[i]) + self.eps)

def rosenbrock_function(x, y):
    """Rosenbrock function for optimization visualization"""
    return (1 - x)**2 + 100 * (y - x**2)**2

def rosenbrock_gradient(x, y):
    """Gradient of Rosenbrock function"""
    dx = -2 * (1 - x) - 400 * x * (y - x**2)
    dy = 200 * (y - x**2)
    return np.array([dx, dy])

def visualize_optimization_paths():
    """Visualize different optimizer paths on Rosenbrock function"""
    print("=== Optimizer Path Visualization ===")
    
    # Create meshgrid for contour plot
    x = np.linspace(-2, 2, 100)
    y = np.linspace(-1, 3, 100)
    X, Y = np.meshgrid(x, y)
    Z = rosenbrock_function(X, Y)
    
    # Starting point
    start_point = np.array([-1.5, 2.5])
    
    # Define optimizers with their parameters
    optimizers_config = {
        'SGD': {'lr': 0.001, 'momentum': 0.0},
        'SGD + Momentum': {'lr': 0.001, 'momentum': 0.9},
        'AdaGrad': {'lr': 0.1},
        'RMSprop': {'lr': 0.01},
        'Adam': {'lr': 0.01}
    }
    
    paths = {}
    
    for opt_name, config in optimizers_config.items():
        print(f"Running {opt_name}...")
        
        # Initialize parameters
        params = [{'data': start_point.copy(), 'grad': None}]
        
        # Create optimizer
        if opt_name == 'SGD':
            optimizer = SGD(params, learning_rate=config['lr'], momentum=config['momentum'])
        elif opt_name == 'SGD + Momentum':
            optimizer = SGD(params, learning_rate=config['lr'], momentum=config['momentum'])
        elif opt_name == 'AdaGrad':
            optimizer = AdaGrad(params, learning_rate=config['lr'])
        elif opt_name == 'RMSprop':
            optimizer = RMSprop(params, learning_rate=config['lr'])
        elif opt_name == 'Adam':
            optimizer = Adam(params, learning_rate=config['lr'])
        
        # Optimization path
        path = [params[0]['data'].copy()]
        
        for step in range(1000):
            # Compute gradient
            current_pos = params[0]['data']
            params[0]['grad'] = rosenbrock_gradient(current_pos[0], current_pos[1])
            
            # Update parameters
            optimizer.step()
            
            # Store path
            path.append(params[0]['data'].copy())
            
            # Check convergence
            if np.linalg.norm(params[0]['grad']) < 1e-6:
                break
        
        paths[opt_name] = np.array(path)
    
    # Plot optimization paths
    plt.figure(figsize=(15, 10))
    
    # Contour plot
    plt.contour(X, Y, Z, levels=np.logspace(-1, 3, 20), alpha=0.6)
    plt.contourf(X, Y, Z, levels=np.logspace(-1, 3, 20), alpha=0.3, cmap='viridis')
    
    # Plot paths
    colors = ['red', 'blue', 'green', 'orange', 'purple']
    for i, (opt_name, path) in enumerate(paths.items()):
        plt.plot(path[:, 0], path[:, 1], colors[i], linewidth=2, 
                label=f'{opt_name} ({len(path)} steps)', alpha=0.8)
        plt.plot(path[0, 0], path[0, 1], 'ko', markersize=8)  # Start point
        plt.plot(path[-1, 0], path[-1, 1], colors[i], marker='*', markersize=12)  # End point
    
    # Mark global minimum
    plt.plot(1, 1, 'r*', markersize=15, label='Global Minimum')
    
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('Optimizer Paths on Rosenbrock Function')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.show()
    
    # Print final distances from optimum
    print("\nFinal distances from global minimum (1, 1):")
    for opt_name, path in paths.items():
        final_pos = path[-1]
        distance = np.linalg.norm(final_pos - np.array([1, 1]))
        print(f"{opt_name}: {distance:.6f}")

class SimpleModel(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(SimpleModel, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, output_size)
        self.relu = nn.ReLU()
    
    def forward(self, x):
        x = self.relu(self.fc1(x))
        x = self.relu(self.fc2(x))
        x = self.fc3(x)
        return x

def compare_optimizers_on_neural_network():
    """Compare optimizers on a neural network training task"""
    print("\n=== Neural Network Optimizer Comparison ===")
    
    # Generate dataset
    X, y = make_classification(n_samples=2000, n_features=20, n_informative=10,
                             n_redundant=5, n_classes=2, random_state=42)
    
    # Standardize
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)
    
    # Split data
    split = int(0.8 * len(X_scaled))
    X_train, X_test = X_scaled[:split], X_scaled[split:]
    y_train, y_test = y[:split], y[split:]
    
    # Convert to PyTorch tensors
    X_train_tensor = torch.FloatTensor(X_train)
    y_train_tensor = torch.LongTensor(y_train)
    X_test_tensor = torch.FloatTensor(X_test)
    y_test_tensor = torch.LongTensor(y_test)
    
    train_dataset = TensorDataset(X_train_tensor, y_train_tensor)
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
    
    # Define optimizers to compare
    optimizer_configs = {
        'SGD': lambda model: optim.SGD(model.parameters(), lr=0.01),
        'SGD + Momentum': lambda model: optim.SGD(model.parameters(), lr=0.01, momentum=0.9),
        'AdaGrad': lambda model: optim.Adagrad(model.parameters(), lr=0.01),
        'RMSprop': lambda model: optim.RMSprop(model.parameters(), lr=0.001),
        'Adam': lambda model: optim.Adam(model.parameters(), lr=0.001),
        'AdamW': lambda model: optim.AdamW(model.parameters(), lr=0.001)
    }
    
    results = {}
    
    for opt_name, opt_fn in optimizer_configs.items():
        print(f"\nTraining with {opt_name}...")
        
        # Create model and optimizer
        model = SimpleModel(input_size=20, hidden_size=64, output_size=2)
        optimizer = opt_fn(model)
        criterion = nn.CrossEntropyLoss()
        
        # Training
        train_losses = []
        train_accuracies = []
        test_accuracies = []
        
        for epoch in range(100):
            # Training phase
            model.train()
            epoch_loss = 0
            correct_train = 0
            total_train = 0
            
            for batch_x, batch_y in train_loader:
                optimizer.zero_grad()
                outputs = model(batch_x)
                loss = criterion(outputs, batch_y)
                loss.backward()
                optimizer.step()
                
                epoch_loss += loss.item()
                _, predicted = torch.max(outputs, 1)
                total_train += batch_y.size(0)
                correct_train += (predicted == batch_y).sum().item()
            
            train_losses.append(epoch_loss / len(train_loader))
            train_accuracies.append(100 * correct_train / total_train)
            
            # Test phase
            model.eval()
            with torch.no_grad():
                test_outputs = model(X_test_tensor)
                _, test_predicted = torch.max(test_outputs, 1)
                test_accuracy = 100 * (test_predicted == y_test_tensor).float().mean()
                test_accuracies.append(test_accuracy.item())
        
        results[opt_name] = {
            'train_losses': train_losses,
            'train_accuracies': train_accuracies,
            'test_accuracies': test_accuracies
        }
        
        print(f"{opt_name} - Final test accuracy: {test_accuracies[-1]:.2f}%")
    
    # Visualize results
    fig, axes = plt.subplots(1, 3, figsize=(18, 5))
    
    # Training loss
    axes[0].set_title('Training Loss')
    for opt_name, data in results.items():
        axes[0].plot(data['train_losses'], label=opt_name, linewidth=2)
    axes[0].set_xlabel('Epoch')
    axes[0].set_ylabel('Loss')
    axes[0].legend()
    axes[0].grid(True)
    
    # Training accuracy
    axes[1].set_title('Training Accuracy')
    for opt_name, data in results.items():
        axes[1].plot(data['train_accuracies'], label=opt_name, linewidth=2)
    axes[1].set_xlabel('Epoch')
    axes[1].set_ylabel('Accuracy (%)')
    axes[1].legend()
    axes[1].grid(True)
    
    # Test accuracy
    axes[2].set_title('Test Accuracy')
    for opt_name, data in results.items():
        axes[2].plot(data['test_accuracies'], label=opt_name, linewidth=2)
    axes[2].set_xlabel('Epoch')
    axes[2].set_ylabel('Accuracy (%)')
    axes[2].legend()
    axes[2].grid(True)
    
    plt.tight_layout()
    plt.show()

def demonstrate_optimizer_properties():
    """Demonstrate key properties and characteristics of different optimizers"""
    print("\n=== Optimizer Properties ===")
    
    properties = {
        'SGD': {
            'Memory Usage': 'O(n) - stores parameters only',
            'Convergence': 'Slow but stable',
            'Hyperparameters': 'Learning rate, momentum',
            'Good For': 'Large datasets, simple problems',
            'Drawbacks': 'Slow convergence, sensitive to learning rate'
        },
        'SGD + Momentum': {
            'Memory Usage': 'O(2n) - stores parameters + velocity',
            'Convergence': 'Faster than SGD, can overshoot',
            'Hyperparameters': 'Learning rate, momentum coefficient',
            'Good For': 'Navigating ravines, escaping local minima',
            'Drawbacks': 'May overshoot optimal point'
        },
        'AdaGrad': {
            'Memory Usage': 'O(2n) - stores parameters + accumulated gradients',
            'Convergence': 'Fast initially, then slows down',
            'Hyperparameters': 'Learning rate',
            'Good For': 'Sparse gradients, different learning rates per parameter',
            'Drawbacks': 'Learning rate decays to zero'
        },
        'RMSprop': {
            'Memory Usage': 'O(2n) - stores parameters + moving average',
            'Convergence': 'Stable, good for non-convex problems',
            'Hyperparameters': 'Learning rate, decay rate',
            'Good For': 'Non-convex optimization, RNNs',
            'Drawbacks': 'Still requires manual learning rate tuning'
        },
        'Adam': {
            'Memory Usage': 'O(3n) - stores parameters + 1st & 2nd moments',
            'Convergence': 'Fast and stable',
            'Hyperparameters': 'Learning rate, β1, β2',
            'Good For': 'Most problems, default choice',
            'Drawbacks': 'May not converge to optimal solution'
        },
        'AdamW': {
            'Memory Usage': 'O(3n) - stores parameters + 1st & 2nd moments',
            'Convergence': 'Fast and stable with better regularization',
            'Hyperparameters': 'Learning rate, β1, β2, weight decay',
            'Good For': 'Deep learning with weight decay',
            'Drawbacks': 'Slightly more complex than Adam'
        }
    }
    
    for opt_name, props in properties.items():
        print(f"\n{opt_name}:")
        for prop, value in props.items():
            print(f"  {prop}: {value}")

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Optimizer Comparison and Visualization ===")
    
    # Visualize optimization paths
    visualize_optimization_paths()
    
    # Compare on neural network
    compare_optimizers_on_neural_network()
    
    # Show properties
    demonstrate_optimizer_properties()
    
    print("\n=== Key Recommendations ===")
    print("1. Adam: Good default choice for most problems")
    print("2. SGD + Momentum: Better for final fine-tuning")
    print("3. AdamW: Better than Adam when using weight decay")
    print("4. RMSprop: Good for RNNs and non-stationary objectives")
    print("5. Learning rate scheduling often more important than optimizer choice")
    print("6. Always tune hyperparameters for your specific problem")