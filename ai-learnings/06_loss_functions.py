# author: csgod cmps studies
# Loss Functions - MSE, Cross-entropy, Huber loss
# Loss functions measure the difference between predicted and actual values,
# guiding the optimization process during neural network training.

import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset
from sklearn.datasets import make_regression, make_classification
from sklearn.preprocessing import StandardScaler

class LossFunctions:
    @staticmethod
    def mean_squared_error(y_true, y_pred):
        """Mean Squared Error - commonly used for regression"""
        return np.mean((y_true - y_pred) ** 2)
    
    @staticmethod
    def mean_absolute_error(y_true, y_pred):
        """Mean Absolute Error - robust to outliers"""
        return np.mean(np.abs(y_true - y_pred))
    
    @staticmethod
    def huber_loss(y_true, y_pred, delta=1.0):
        """Huber Loss - combines MSE and MAE, robust to outliers"""
        residual = np.abs(y_true - y_pred)
        condition = residual <= delta
        return np.mean(np.where(condition, 
                               0.5 * residual ** 2,
                               delta * residual - 0.5 * delta ** 2))
    
    @staticmethod
    def binary_cross_entropy(y_true, y_pred, epsilon=1e-15):
        """Binary Cross-Entropy - for binary classification"""
        y_pred = np.clip(y_pred, epsilon, 1 - epsilon)
        return -np.mean(y_true * np.log(y_pred) + (1 - y_true) * np.log(1 - y_pred))
    
    @staticmethod
    def categorical_cross_entropy(y_true, y_pred, epsilon=1e-15):
        """Categorical Cross-Entropy - for multi-class classification"""
        y_pred = np.clip(y_pred, epsilon, 1 - epsilon)
        return -np.sum(y_true * np.log(y_pred), axis=1).mean()
    
    @staticmethod
    def focal_loss(y_true, y_pred, alpha=1, gamma=2, epsilon=1e-15):
        """Focal Loss - addresses class imbalance"""
        y_pred = np.clip(y_pred, epsilon, 1 - epsilon)
        pt = np.where(y_true == 1, y_pred, 1 - y_pred)
        return -alpha * np.power(1 - pt, gamma) * np.log(pt)
    
    @staticmethod
    def hinge_loss(y_true, y_pred):
        """Hinge Loss - used in SVM, y_true should be -1 or 1"""
        return np.mean(np.maximum(0, 1 - y_true * y_pred))
    
    @staticmethod
    def quantile_loss(y_true, y_pred, quantile=0.5):
        """Quantile Loss - for quantile regression"""
        residual = y_true - y_pred
        return np.mean(np.maximum(quantile * residual, (quantile - 1) * residual))

def visualize_loss_functions():
    """Visualize different loss functions and their properties"""
    # Generate predictions around true value
    y_true = 0
    y_pred_range = np.linspace(-3, 3, 100)
    
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    
    # MSE
    mse_losses = [(y_true - y_pred)**2 for y_pred in y_pred_range]
    axes[0, 0].plot(y_pred_range, mse_losses, 'b-', linewidth=2)
    axes[0, 0].set_title('Mean Squared Error')
    axes[0, 0].set_xlabel('Predicted Value')
    axes[0, 0].set_ylabel('Loss')
    axes[0, 0].axvline(y_true, color='r', linestyle='--', label='True Value')
    axes[0, 0].grid(True)
    axes[0, 0].legend()
    
    # MAE
    mae_losses = [abs(y_true - y_pred) for y_pred in y_pred_range]
    axes[0, 1].plot(y_pred_range, mae_losses, 'g-', linewidth=2)
    axes[0, 1].set_title('Mean Absolute Error')
    axes[0, 1].set_xlabel('Predicted Value')
    axes[0, 1].set_ylabel('Loss')
    axes[0, 1].axvline(y_true, color='r', linestyle='--', label='True Value')
    axes[0, 1].grid(True)
    axes[0, 1].legend()
    
    # Huber Loss
    delta = 1.0
    huber_losses = []
    for y_pred in y_pred_range:
        residual = abs(y_true - y_pred)
        if residual <= delta:
            loss = 0.5 * residual ** 2
        else:
            loss = delta * residual - 0.5 * delta ** 2
        huber_losses.append(loss)
    
    axes[0, 2].plot(y_pred_range, huber_losses, 'm-', linewidth=2)
    axes[0, 2].set_title(f'Huber Loss (δ={delta})')
    axes[0, 2].set_xlabel('Predicted Value')
    axes[0, 2].set_ylabel('Loss')
    axes[0, 2].axvline(y_true, color='r', linestyle='--', label='True Value')
    axes[0, 2].grid(True)
    axes[0, 2].legend()
    
    # Binary Cross-Entropy
    y_true_binary = 1
    epsilon = 1e-15
    y_pred_prob = np.linspace(epsilon, 1-epsilon, 100)
    bce_losses = [-np.log(y_pred) if y_true_binary == 1 else -np.log(1-y_pred) for y_pred in y_pred_prob]
    
    axes[1, 0].plot(y_pred_prob, bce_losses, 'r-', linewidth=2)
    axes[1, 0].set_title('Binary Cross-Entropy (y=1)')
    axes[1, 0].set_xlabel('Predicted Probability')
    axes[1, 0].set_ylabel('Loss')
    axes[1, 0].set_yscale('log')
    axes[1, 0].grid(True)
    
    # Hinge Loss
    y_true_hinge = 1
    y_pred_hinge = np.linspace(-3, 3, 100)
    hinge_losses = [max(0, 1 - y_true_hinge * y_pred) for y_pred in y_pred_hinge]
    
    axes[1, 1].plot(y_pred_hinge, hinge_losses, 'orange', linewidth=2)
    axes[1, 1].set_title('Hinge Loss (y=1)')
    axes[1, 1].set_xlabel('Predicted Value')
    axes[1, 1].set_ylabel('Loss')
    axes[1, 1].grid(True)
    
    # Comparison of regression losses
    axes[1, 2].plot(y_pred_range, mse_losses, 'b-', linewidth=2, label='MSE')
    axes[1, 2].plot(y_pred_range, mae_losses, 'g-', linewidth=2, label='MAE')
    axes[1, 2].plot(y_pred_range, huber_losses, 'm-', linewidth=2, label='Huber')
    axes[1, 2].set_title('Regression Loss Comparison')
    axes[1, 2].set_xlabel('Predicted Value')
    axes[1, 2].set_ylabel('Loss')
    axes[1, 2].legend()
    axes[1, 2].grid(True)
    axes[1, 2].set_ylim(0, 5)
    
    plt.tight_layout()
    plt.show()

class RegressionModel(nn.Module):
    def __init__(self, input_size, hidden_size):
        super(RegressionModel, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, 1)
        self.relu = nn.ReLU()
    
    def forward(self, x):
        x = self.relu(self.fc1(x))
        x = self.relu(self.fc2(x))
        x = self.fc3(x)
        return x

class ClassificationModel(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(ClassificationModel, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, output_size)
        self.relu = nn.ReLU()
    
    def forward(self, x):
        x = self.relu(self.fc1(x))
        x = self.relu(self.fc2(x))
        x = self.fc3(x)
        return x

def compare_regression_losses():
    """Compare different loss functions on a regression task with outliers"""
    print("=== Regression Loss Functions Comparison ===")
    
    # Generate data with outliers
    np.random.seed(42)
    X, y = make_regression(n_samples=300, n_features=1, noise=10, random_state=42)
    
    # Add some outliers
    outlier_indices = np.random.choice(len(y), 20, replace=False)
    y[outlier_indices] += np.random.normal(0, 50, 20)
    
    # Standardize
    scaler_X = StandardScaler()
    scaler_y = StandardScaler()
    X_scaled = scaler_X.fit_transform(X)
    y_scaled = scaler_y.fit_transform(y.reshape(-1, 1)).flatten()
    
    # Convert to PyTorch
    X_tensor = torch.FloatTensor(X_scaled)
    y_tensor = torch.FloatTensor(y_scaled)
    
    dataset = TensorDataset(X_tensor, y_tensor)
    train_loader = DataLoader(dataset, batch_size=32, shuffle=True)
    
    # Define different loss functions
    loss_functions = {
        'MSE': nn.MSELoss(),
        'L1 (MAE)': nn.L1Loss(),
        'Huber': nn.HuberLoss(delta=1.0),
        'Smooth L1': nn.SmoothL1Loss()
    }
    
    results = {}
    models = {}
    
    plt.figure(figsize=(15, 10))
    
    for i, (loss_name, criterion) in enumerate(loss_functions.items()):
        print(f"\nTraining with {loss_name} loss...")
        
        # Create model
        model = RegressionModel(input_size=1, hidden_size=64)
        optimizer = optim.Adam(model.parameters(), lr=0.01)
        
        # Training
        model.train()
        losses = []
        
        for epoch in range(200):
            epoch_loss = 0
            for batch_x, batch_y in train_loader:
                optimizer.zero_grad()
                outputs = model(batch_x).squeeze()
                loss = criterion(outputs, batch_y)
                loss.backward()
                optimizer.step()
                epoch_loss += loss.item()
            
            losses.append(epoch_loss / len(train_loader))
            
            if epoch % 50 == 0:
                print(f'Epoch {epoch}, Loss: {losses[-1]:.4f}')
        
        results[loss_name] = losses
        models[loss_name] = model
        
        # Plot results
        plt.subplot(2, 2, i + 1)
        
        # Generate predictions for plotting
        X_plot = np.linspace(X_scaled.min(), X_scaled.max(), 100).reshape(-1, 1)
        X_plot_tensor = torch.FloatTensor(X_plot)
        
        model.eval()
        with torch.no_grad():
            y_plot = model(X_plot_tensor).numpy()
        
        # Plot data and predictions
        plt.scatter(X_scaled, y_scaled, alpha=0.6, label='Data')
        plt.plot(X_plot, y_plot, 'r-', linewidth=2, label='Prediction')
        plt.xlabel('Feature')
        plt.ylabel('Target')
        plt.title(f'{loss_name} Loss')
        plt.legend()
        plt.grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Plot learning curves
    plt.figure(figsize=(10, 6))
    for loss_name, losses in results.items():
        plt.plot(losses, label=loss_name, linewidth=2)
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.title('Learning Curves Comparison')
    plt.legend()
    plt.grid(True)
    plt.show()

def compare_classification_losses():
    """Compare different loss functions for classification with imbalanced data"""
    print("\n=== Classification Loss Functions Comparison ===")
    
    # Generate imbalanced dataset
    X, y = make_classification(n_samples=1000, n_classes=2, weights=[0.9, 0.1], 
                             n_informative=2, n_redundant=0, random_state=42)
    
    # Standardize
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)
    
    # Convert to PyTorch
    X_tensor = torch.FloatTensor(X_scaled)
    y_tensor = torch.LongTensor(y)
    
    dataset = TensorDataset(X_tensor, y_tensor)
    train_loader = DataLoader(dataset, batch_size=32, shuffle=True)
    
    # Define different loss functions
    loss_functions = {
        'Cross-Entropy': nn.CrossEntropyLoss(),
        'Weighted CE': nn.CrossEntropyLoss(weight=torch.tensor([1.0, 9.0])),  # Weight minority class more
        'Focal Loss': None  # Custom implementation
    }
    
    class FocalLoss(nn.Module):
        def __init__(self, alpha=1, gamma=2):
            super(FocalLoss, self).__init__()
            self.alpha = alpha
            self.gamma = gamma
    
        def forward(self, inputs, targets):
            ce_loss = nn.CrossEntropyLoss()(inputs, targets)
            pt = torch.exp(-ce_loss)
            focal_loss = self.alpha * (1-pt)**self.gamma * ce_loss
            return focal_loss
    
    loss_functions['Focal Loss'] = FocalLoss(alpha=1, gamma=2)
    
    results = {}
    
    for loss_name, criterion in loss_functions.items():
        print(f"\nTraining with {loss_name}...")
        
        model = ClassificationModel(input_size=X.shape[1], hidden_size=64, output_size=2)
        optimizer = optim.Adam(model.parameters(), lr=0.01)
        
        # Training
        model.train()
        losses = []
        accuracies = []
        
        for epoch in range(100):
            epoch_loss = 0
            correct = 0
            total = 0
            
            for batch_x, batch_y in train_loader:
                optimizer.zero_grad()
                outputs = model(batch_x)
                loss = criterion(outputs, batch_y)
                loss.backward()
                optimizer.step()
                
                epoch_loss += loss.item()
                _, predicted = torch.max(outputs, 1)
                total += batch_y.size(0)
                correct += (predicted == batch_y).sum().item()
            
            losses.append(epoch_loss / len(train_loader))
            accuracies.append(100 * correct / total)
        
        results[loss_name] = {'losses': losses, 'accuracies': accuracies}
        
        # Evaluate on class-wise metrics
        model.eval()
        with torch.no_grad():
            outputs = model(X_tensor)
            _, predicted = torch.max(outputs, 1)
            
            # Calculate per-class accuracy
            class_0_mask = y_tensor == 0
            class_1_mask = y_tensor == 1
            
            class_0_acc = (predicted[class_0_mask] == y_tensor[class_0_mask]).float().mean()
            class_1_acc = (predicted[class_1_mask] == y_tensor[class_1_mask]).float().mean()
            
            print(f"{loss_name} - Class 0 Accuracy: {class_0_acc:.4f}, Class 1 Accuracy: {class_1_acc:.4f}")
    
    # Visualize results
    plt.figure(figsize=(12, 5))
    
    # Loss curves
    plt.subplot(1, 2, 1)
    for loss_name, data in results.items():
        plt.plot(data['losses'], label=loss_name, linewidth=2)
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.title('Loss Curves')
    plt.legend()
    plt.grid(True)
    
    # Accuracy curves
    plt.subplot(1, 2, 2)
    for loss_name, data in results.items():
        plt.plot(data['accuracies'], label=loss_name, linewidth=2)
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy (%)')
    plt.title('Accuracy Curves')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()

def demonstrate_loss_properties():
    """Demonstrate key properties of different loss functions"""
    print("\n=== Loss Function Properties ===")
    
    properties = {
        'MSE': {
            'Use Case': 'Regression',
            'Robust to Outliers': False,
            'Differentiable': True,
            'Gradient': 'Quadratic growth',
            'When to Use': 'Normal regression, Gaussian noise'
        },
        'MAE': {
            'Use Case': 'Regression',
            'Robust to Outliers': True,
            'Differentiable': False,  # at zero
            'Gradient': 'Constant',
            'When to Use': 'Robust regression, outliers present'
        },
        'Huber': {
            'Use Case': 'Regression',
            'Robust to Outliers': True,
            'Differentiable': True,
            'Gradient': 'Quadratic + Linear',
            'When to Use': 'Balance between MSE and MAE'
        },
        'Binary Cross-Entropy': {
            'Use Case': 'Binary Classification',
            'Robust to Outliers': False,
            'Differentiable': True,
            'Gradient': 'Exponential growth',
            'When to Use': 'Binary classification with probabilities'
        },
        'Focal Loss': {
            'Use Case': 'Classification (Imbalanced)',
            'Robust to Outliers': False,
            'Differentiable': True,
            'Gradient': 'Reduced for easy examples',
            'When to Use': 'Imbalanced datasets, hard examples'
        },
        'Hinge Loss': {
            'Use Case': 'Classification (SVM)',
            'Robust to Outliers': True,
            'Differentiable': False,  # at margin
            'Gradient': 'Constant or zero',
            'When to Use': 'Maximum margin classifiers'
        }
    }
    
    for loss_name, props in properties.items():
        print(f"\n{loss_name}:")
        for prop, value in props.items():
            print(f"  {prop}: {value}")

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Loss Functions Visualization ===")
    visualize_loss_functions()
    
    # Compare regression losses
    compare_regression_losses()
    
    # Compare classification losses
    compare_classification_losses()
    
    # Show properties
    demonstrate_loss_properties()
    
    print("\n=== Key Takeaways ===")
    print("1. MSE penalizes large errors more heavily than small ones")
    print("2. MAE is more robust to outliers but has unstable gradients")
    print("3. Huber loss combines benefits of both MSE and MAE")
    print("4. Cross-entropy is the standard for classification problems")
    print("5. Focal loss helps with class imbalance by focusing on hard examples")
    print("6. Choose loss function based on problem characteristics and data distribution")