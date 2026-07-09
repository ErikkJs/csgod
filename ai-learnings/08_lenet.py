# author: csgod cmps studies
# LeNet - Classic CNN architecture
# LeNet-5 is one of the earliest successful CNN architectures, designed by Yann LeCun
# for handwritten digit recognition. It established many foundational CNN concepts.

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.utils.data import DataLoader
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np

class LeNet5(nn.Module):
    """Original LeNet-5 architecture"""
    def __init__(self, num_classes=10):
        super(LeNet5, self).__init__()
        
        # Feature extraction layers
        self.conv1 = nn.Conv2d(1, 6, kernel_size=5)    # 32x32 -> 28x28
        self.pool1 = nn.AvgPool2d(kernel_size=2)       # 28x28 -> 14x14
        self.conv2 = nn.Conv2d(6, 16, kernel_size=5)   # 14x14 -> 10x10
        self.pool2 = nn.AvgPool2d(kernel_size=2)       # 10x10 -> 5x5
        
        # Classification layers
        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, num_classes)
    
    def forward(self, x):
        # Convolutional layers with average pooling
        x = F.tanh(self.conv1(x))  # Original used tanh
        x = self.pool1(x)
        x = F.tanh(self.conv2(x))
        x = self.pool2(x)
        
        # Flatten for fully connected layers
        x = x.view(-1, 16 * 5 * 5)
        
        # Fully connected layers
        x = F.tanh(self.fc1(x))
        x = F.tanh(self.fc2(x))
        x = self.fc3(x)  # No activation on output layer
        
        return x

class ModernLeNet(nn.Module):
    """Modified LeNet with modern improvements"""
    def __init__(self, num_classes=10):
        super(ModernLeNet, self).__init__()
        
        # Feature extraction with ReLU and max pooling
        self.conv1 = nn.Conv2d(1, 6, kernel_size=5, padding=2)  # 28x28 -> 28x28
        self.pool1 = nn.MaxPool2d(kernel_size=2)                # 28x28 -> 14x14
        self.conv2 = nn.Conv2d(6, 16, kernel_size=5)            # 14x14 -> 10x10
        self.pool2 = nn.MaxPool2d(kernel_size=2)                # 10x10 -> 5x5
        
        # Batch normalization
        self.bn1 = nn.BatchNorm2d(6)
        self.bn2 = nn.BatchNorm2d(16)
        
        # Classification layers with dropout
        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, num_classes)
        
        self.dropout = nn.Dropout(0.5)
    
    def forward(self, x):
        # Modern conv layers with ReLU and batch norm
        x = self.pool1(F.relu(self.bn1(self.conv1(x))))
        x = self.pool2(F.relu(self.bn2(self.conv2(x))))
        
        # Flatten
        x = x.view(-1, 16 * 5 * 5)
        
        # FC layers with dropout
        x = F.relu(self.fc1(x))
        x = self.dropout(x)
        x = F.relu(self.fc2(x))
        x = self.dropout(x)
        x = self.fc3(x)
        
        return x

def train_model(model, train_loader, test_loader, num_epochs=10, learning_rate=0.001):
    """Train a model and return training history"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=learning_rate)
    
    train_losses = []
    train_accuracies = []
    test_accuracies = []
    
    for epoch in range(num_epochs):
        model.train()
        running_loss = 0.0
        correct_train = 0
        total_train = 0
        
        for batch_idx, (data, target) in enumerate(train_loader):
            data, target = data.to(device), target.to(device)
            
            optimizer.zero_grad()
            outputs = model(data)
            loss = criterion(outputs, target)
            loss.backward()
            optimizer.step()
            
            running_loss += loss.item()
            _, predicted = torch.max(outputs.data, 1)
            total_train += target.size(0)
            correct_train += (predicted == target).sum().item()
            
            if batch_idx % 100 == 0:
                print(f'Epoch [{epoch+1}/{num_epochs}], Step [{batch_idx+1}/{len(train_loader)}], '
                      f'Loss: {loss.item():.4f}')
        
        train_loss = running_loss / len(train_loader)
        train_acc = 100 * correct_train / total_train
        
        # Test accuracy
        model.eval()
        correct_test = 0
        total_test = 0
        with torch.no_grad():
            for data, target in test_loader:
                data, target = data.to(device), target.to(device)
                outputs = model(data)
                _, predicted = torch.max(outputs, 1)
                total_test += target.size(0)
                correct_test += (predicted == target).sum().item()
        
        test_acc = 100 * correct_test / total_test
        
        train_losses.append(train_loss)
        train_accuracies.append(train_acc)
        test_accuracies.append(test_acc)
        
        print(f'Epoch [{epoch+1}/{num_epochs}] - Loss: {train_loss:.4f}, '
              f'Train Acc: {train_acc:.2f}%, Test Acc: {test_acc:.2f}%')
    
    return train_losses, train_accuracies, test_accuracies

def visualize_filters(model, layer_name='conv1'):
    """Visualize convolutional filters"""
    if layer_name == 'conv1':
        filters = model.conv1.weight.data.clone()
    elif layer_name == 'conv2':
        filters = model.conv2.weight.data.clone()
    else:
        raise ValueError("Invalid layer name")
    
    # Normalize filters for visualization
    filters = filters - filters.min()
    filters = filters / filters.max()
    
    # Plot filters
    num_filters = filters.shape[0]
    fig, axes = plt.subplots(1, num_filters, figsize=(15, 3))
    
    for i in range(num_filters):
        if filters.shape[1] == 1:  # Single channel (conv1)
            axes[i].imshow(filters[i, 0], cmap='gray')
        else:  # Multiple channels (conv2) - show first channel
            axes[i].imshow(filters[i, 0], cmap='gray')
        axes[i].set_title(f'Filter {i+1}')
        axes[i].axis('off')
    
    plt.suptitle(f'{layer_name.upper()} Filters')
    plt.tight_layout()
    plt.show()

def visualize_feature_maps(model, data_loader, layer_name='conv1'):
    """Visualize feature maps for a sample input"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    model.eval()
    
    # Get a sample image
    data_iter = iter(data_loader)
    images, _ = next(data_iter)
    sample_image = images[0:1].to(device)  # Take first image
    
    # Register hook to capture feature maps
    activation = {}
    def get_activation(name):
        def hook(model, input, output):
            activation[name] = output.detach()
        return hook
    
    # Register hook
    if layer_name == 'conv1':
        model.conv1.register_forward_hook(get_activation('conv1'))
    elif layer_name == 'conv2':
        model.conv2.register_forward_hook(get_activation('conv2'))
    
    # Forward pass
    with torch.no_grad():
        _ = model(sample_image)
    
    # Get feature maps
    feature_maps = activation[layer_name].cpu().numpy()[0]  # Remove batch dimension
    
    # Visualize original image and feature maps
    plt.figure(figsize=(15, 8))
    
    # Original image
    plt.subplot(2, 4, 1)
    plt.imshow(sample_image.cpu().numpy()[0, 0], cmap='gray')
    plt.title('Original Image')
    plt.axis('off')
    
    # Feature maps
    num_maps = min(7, feature_maps.shape[0])
    for i in range(num_maps):
        plt.subplot(2, 4, i + 2)
        plt.imshow(feature_maps[i], cmap='gray')
        plt.title(f'Feature Map {i+1}')
        plt.axis('off')
    
    plt.suptitle(f'{layer_name.upper()} Feature Maps')
    plt.tight_layout()
    plt.show()

def compare_architectures():
    """Compare original LeNet-5 with modern improvements"""
    print("=== Comparing LeNet Architectures ===")
    
    # Data loading with transforms
    transform_original = transforms.Compose([
        transforms.Resize((32, 32)),  # LeNet expects 32x32
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])
    
    transform_modern = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])
    
    # Load MNIST dataset
    train_dataset_original = torchvision.datasets.MNIST(
        root='./data', train=True, download=True, transform=transform_original
    )
    train_dataset_modern = torchvision.datasets.MNIST(
        root='./data', train=True, download=True, transform=transform_modern
    )
    
    test_dataset_original = torchvision.datasets.MNIST(
        root='./data', train=False, download=True, transform=transform_original
    )
    test_dataset_modern = torchvision.datasets.MNIST(
        root='./data', train=False, download=True, transform=transform_modern
    )
    
    train_loader_original = DataLoader(train_dataset_original, batch_size=64, shuffle=True)
    train_loader_modern = DataLoader(train_dataset_modern, batch_size=64, shuffle=True)
    test_loader_original = DataLoader(test_dataset_original, batch_size=64, shuffle=False)
    test_loader_modern = DataLoader(test_dataset_modern, batch_size=64, shuffle=False)
    
    # Train original LeNet-5
    print("\nTraining Original LeNet-5...")
    original_model = LeNet5(num_classes=10)
    original_losses, original_train_acc, original_test_acc = train_model(
        original_model, train_loader_original, test_loader_original, num_epochs=5
    )
    
    # Train modern LeNet
    print("\nTraining Modern LeNet...")
    modern_model = ModernLeNet(num_classes=10)
    modern_losses, modern_train_acc, modern_test_acc = train_model(
        modern_model, train_loader_modern, test_loader_modern, num_epochs=5
    )
    
    # Visualize results
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    
    epochs = range(1, len(original_losses) + 1)
    
    # Training loss
    axes[0].plot(epochs, original_losses, 'b-o', label='Original LeNet-5')
    axes[0].plot(epochs, modern_losses, 'r-s', label='Modern LeNet')
    axes[0].set_title('Training Loss')
    axes[0].set_xlabel('Epoch')
    axes[0].set_ylabel('Loss')
    axes[0].legend()
    axes[0].grid(True)
    
    # Training accuracy
    axes[1].plot(epochs, original_train_acc, 'b-o', label='Original LeNet-5')
    axes[1].plot(epochs, modern_train_acc, 'r-s', label='Modern LeNet')
    axes[1].set_title('Training Accuracy')
    axes[1].set_xlabel('Epoch')
    axes[1].set_ylabel('Accuracy (%)')
    axes[1].legend()
    axes[1].grid(True)
    
    # Test accuracy
    axes[2].plot(epochs, original_test_acc, 'b-o', label='Original LeNet-5')
    axes[2].plot(epochs, modern_test_acc, 'r-s', label='Modern LeNet')
    axes[2].set_title('Test Accuracy')
    axes[2].set_xlabel('Epoch')
    axes[2].set_ylabel('Accuracy (%)')
    axes[2].legend()
    axes[2].grid(True)
    
    plt.tight_layout()
    plt.show()
    
    print(f"\nFinal Results:")
    print(f"Original LeNet-5 - Test Accuracy: {original_test_acc[-1]:.2f}%")
    print(f"Modern LeNet - Test Accuracy: {modern_test_acc[-1]:.2f}%")
    
    return original_model, modern_model, test_loader_modern

def analyze_network_architecture():
    """Analyze the LeNet architecture and its properties"""
    print("\n=== LeNet Architecture Analysis ===")
    
    model = LeNet5()
    
    # Count parameters
    total_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
    print(f"Total trainable parameters: {total_params:,}")
    
    # Analyze each layer
    print("\nLayer-by-layer analysis:")
    print("Input: 32x32x1 (1,024 pixels)")
    
    # Conv1
    conv1_params = 5 * 5 * 1 * 6 + 6  # weights + biases
    print(f"Conv1: 32x32x1 -> 28x28x6, Parameters: {conv1_params}")
    
    # Pool1
    print(f"Pool1: 28x28x6 -> 14x14x6, Parameters: 0")
    
    # Conv2
    conv2_params = 5 * 5 * 6 * 16 + 16
    print(f"Conv2: 14x14x6 -> 10x10x16, Parameters: {conv2_params}")
    
    # Pool2
    print(f"Pool2: 10x10x16 -> 5x5x16, Parameters: 0")
    
    # FC layers
    fc1_params = 16 * 5 * 5 * 120 + 120
    fc2_params = 120 * 84 + 84
    fc3_params = 84 * 10 + 10
    
    print(f"FC1: 400 -> 120, Parameters: {fc1_params}")
    print(f"FC2: 120 -> 84, Parameters: {fc2_params}")
    print(f"FC3: 84 -> 10, Parameters: {fc3_params}")
    
    # Memory analysis
    print(f"\nMemory requirements (approximate):")
    print(f"Input: {32*32*1*4/1024:.1f} KB")  # float32
    print(f"After Conv1: {28*28*6*4/1024:.1f} KB")
    print(f"After Pool1: {14*14*6*4/1024:.1f} KB")
    print(f"After Conv2: {10*10*16*4/1024:.1f} KB")
    print(f"After Pool2: {5*5*16*4/1024:.1f} KB")

# Example usage and demonstration
if __name__ == "__main__":
    print("=== LeNet Architecture Implementation and Analysis ===")
    
    # Analyze architecture
    analyze_network_architecture()
    
    # Compare architectures
    original_model, modern_model, test_loader = compare_architectures()
    
    # Visualize filters and feature maps
    print("\n=== Visualization ===")
    print("Visualizing learned filters...")
    visualize_filters(modern_model, 'conv1')
    visualize_filters(modern_model, 'conv2')
    
    print("Visualizing feature maps...")
    visualize_feature_maps(modern_model, test_loader, 'conv1')
    visualize_feature_maps(modern_model, test_loader, 'conv2')
    
    # Key insights
    print("\n=== Key Insights about LeNet ===")
    print("1. LeNet established the basic CNN pattern: Conv -> Pool -> Conv -> Pool -> FC")
    print("2. Original used tanh activation and average pooling")
    print("3. Modern improvements: ReLU, max pooling, batch norm, dropout")
    print("4. Small by modern standards but pioneered many concepts")
    print("5. Good for simple datasets like MNIST")
    print("6. Foundation for more complex architectures like AlexNet")
    
    print("\n=== Historical Significance ===")
    print("- Published by Yann LeCun et al. in 1998")
    print("- One of the first successful CNNs")
    print("- Used for ZIP code recognition by US Postal Service")
    print("- Inspired modern deep learning architectures")
    print("- Demonstrated effectiveness of gradient-based learning")