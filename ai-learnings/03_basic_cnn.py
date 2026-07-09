# author: csgod cmps studies
# Basic Convolutional Neural Network (CNN) - Image classification
# CNNs use convolutional layers to detect local features in images,
# followed by pooling layers for dimensionality reduction and fully connected layers for classification.

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.utils.data import DataLoader, TensorDataset
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np

class BasicCNN(nn.Module):
    def __init__(self, num_classes=10):
        super(BasicCNN, self).__init__()
        
        # Convolutional layers
        self.conv1 = nn.Conv2d(1, 32, kernel_size=3, padding=1)  # 28x28 -> 28x28
        self.conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)  # 28x28 -> 28x28
        self.conv3 = nn.Conv2d(64, 64, kernel_size=3, padding=1)  # 14x14 -> 14x14
        
        # Pooling layer
        self.pool = nn.MaxPool2d(2, 2)  # Reduces size by half
        
        # Fully connected layers
        self.fc1 = nn.Linear(64 * 7 * 7, 128)  # 7x7 after two pooling operations
        self.fc2 = nn.Linear(128, num_classes)
        
        # Dropout for regularization
        self.dropout = nn.Dropout(0.5)
    
    def forward(self, x):
        # First conv block
        x = self.pool(F.relu(self.conv1(x)))  # 28x28 -> 14x14
        
        # Second conv block
        x = self.pool(F.relu(self.conv2(x)))  # 14x14 -> 7x7
        
        # Third conv layer (no pooling)
        x = F.relu(self.conv3(x))  # 7x7 -> 7x7
        
        # Flatten for fully connected layers
        x = x.view(-1, 64 * 7 * 7)
        
        # Fully connected layers with dropout
        x = F.relu(self.fc1(x))
        x = self.dropout(x)
        x = self.fc2(x)
        
        return x

def train_model(model, train_loader, criterion, optimizer, epochs=5):
    model.train()
    train_losses = []
    train_accuracies = []
    
    for epoch in range(epochs):
        running_loss = 0.0
        correct = 0
        total = 0
        
        for batch_idx, (data, target) in enumerate(train_loader):
            # Zero gradients
            optimizer.zero_grad()
            
            # Forward pass
            outputs = model(data)
            loss = criterion(outputs, target)
            
            # Backward pass and optimize
            loss.backward()
            optimizer.step()
            
            # Statistics
            running_loss += loss.item()
            _, predicted = torch.max(outputs.data, 1)
            total += target.size(0)
            correct += (predicted == target).sum().item()
            
            if batch_idx % 100 == 0:
                print(f'Epoch [{epoch+1}/{epochs}], Step [{batch_idx+1}/{len(train_loader)}], '
                      f'Loss: {loss.item():.4f}')
        
        epoch_loss = running_loss / len(train_loader)
        epoch_acc = 100 * correct / total
        train_losses.append(epoch_loss)
        train_accuracies.append(epoch_acc)
        
        print(f'Epoch [{epoch+1}/{epochs}] - Loss: {epoch_loss:.4f}, Accuracy: {epoch_acc:.2f}%')
    
    return train_losses, train_accuracies

def test_model(model, test_loader):
    model.eval()
    correct = 0
    total = 0
    class_correct = list(0. for i in range(10))
    class_total = list(0. for i in range(10))
    
    with torch.no_grad():
        for data, target in test_loader:
            outputs = model(data)
            _, predicted = torch.max(outputs, 1)
            total += target.size(0)
            correct += (predicted == target).sum().item()
            
            # Per-class accuracy
            c = (predicted == target).squeeze()
            for i in range(target.size(0)):
                label = target[i]
                class_correct[label] += c[i].item()
                class_total[label] += 1
    
    overall_accuracy = 100 * correct / total
    print(f'Test Accuracy: {overall_accuracy:.2f}%')
    
    # Print per-class accuracies
    classes = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
    for i in range(10):
        if class_total[i] > 0:
            accuracy = 100 * class_correct[i] / class_total[i]
            print(f'Accuracy of {classes[i]}: {accuracy:.2f}%')
    
    return overall_accuracy

def visualize_predictions(model, test_loader, num_images=8):
    model.eval()
    images, labels = next(iter(test_loader))
    outputs = model(images[:num_images])
    _, predicted = torch.max(outputs, 1)
    
    plt.figure(figsize=(12, 6))
    for i in range(num_images):
        plt.subplot(2, 4, i + 1)
        plt.imshow(images[i].squeeze(), cmap='gray')
        plt.title(f'Predicted: {predicted[i].item()}, Actual: {labels[i].item()}')
        plt.axis('off')
    
    plt.tight_layout()
    plt.show()

def visualize_filters(model):
    # Visualize first layer filters
    filters = model.conv1.weight.data.clone()
    filters = filters - filters.min()
    filters = filters / filters.max()
    
    plt.figure(figsize=(10, 8))
    for i in range(min(32, filters.shape[0])):
        plt.subplot(4, 8, i + 1)
        plt.imshow(filters[i].squeeze(), cmap='gray')
        plt.axis('off')
        plt.title(f'Filter {i+1}')
    
    plt.suptitle('First Layer Convolutional Filters')
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    # Set random seed for reproducibility
    torch.manual_seed(42)
    
    # Load MNIST dataset
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))  # MNIST normalization
    ])
    
    # Download and load training data
    train_dataset = torchvision.datasets.MNIST(
        root='./data', train=True, download=True, transform=transform
    )
    train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
    
    # Download and load test data
    test_dataset = torchvision.datasets.MNIST(
        root='./data', train=False, download=True, transform=transform
    )
    test_loader = DataLoader(test_dataset, batch_size=64, shuffle=False)
    
    print(f"Training samples: {len(train_dataset)}")
    print(f"Test samples: {len(test_dataset)}")
    
    # Create model, loss function, and optimizer
    model = BasicCNN(num_classes=10)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=0.001)
    
    print("Model architecture:")
    print(model)
    
    # Count parameters
    total_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
    print(f"\nTotal trainable parameters: {total_params:,}")
    
    # Train the model
    print("\nStarting training...")
    train_losses, train_accuracies = train_model(model, train_loader, criterion, optimizer, epochs=3)
    
    # Test the model
    print("\nEvaluating model...")
    test_accuracy = test_model(model, test_loader)
    
    # Visualizations
    plt.figure(figsize=(12, 4))
    
    # Plot training loss
    plt.subplot(1, 2, 1)
    plt.plot(train_losses)
    plt.title('Training Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.grid(True)
    
    # Plot training accuracy
    plt.subplot(1, 2, 2)
    plt.plot(train_accuracies)
    plt.title('Training Accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy (%)')
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Visualize some predictions
    print("\nVisualizing predictions...")
    visualize_predictions(model, test_loader)
    
    # Visualize learned filters
    print("\nVisualizing learned filters...")
    visualize_filters(model)
    
    # Save the model
    torch.save(model.state_dict(), 'basic_cnn_mnist.pth')
    print("\nModel saved as 'basic_cnn_mnist.pth'")