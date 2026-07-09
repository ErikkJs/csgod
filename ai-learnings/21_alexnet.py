# author: csgod cmps studies
# AlexNet - Deep Convolutional Neural Network
# AlexNet was the first CNN to win ImageNet (2012), sparking the deep learning revolution
# with innovations like ReLU activation, dropout regularization, and GPU training.

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.utils.data import DataLoader, Dataset
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np
from PIL import Image
import time

class AlexNet(nn.Module):
    """AlexNet architecture implementation"""
    def __init__(self, num_classes=1000, dropout=0.5):
        super(AlexNet, self).__init__()
        
        # Feature extraction layers (convolutional)
        self.features = nn.Sequential(
            # Layer 1: 224x224x3 -> 55x55x96
            nn.Conv2d(3, 96, kernel_size=11, stride=4, padding=2),
            nn.ReLU(inplace=True),
            nn.LocalResponseNorm(size=5, alpha=0.0001, beta=0.75, k=2),
            nn.MaxPool2d(kernel_size=3, stride=2),
            
            # Layer 2: 55x55x96 -> 27x27x256
            nn.Conv2d(96, 256, kernel_size=5, stride=1, padding=2),
            nn.ReLU(inplace=True),
            nn.LocalResponseNorm(size=5, alpha=0.0001, beta=0.75, k=2),
            nn.MaxPool2d(kernel_size=3, stride=2),
            
            # Layer 3: 27x27x256 -> 13x13x384
            nn.Conv2d(256, 384, kernel_size=3, stride=1, padding=1),
            nn.ReLU(inplace=True),
            
            # Layer 4: 13x13x384 -> 13x13x384
            nn.Conv2d(384, 384, kernel_size=3, stride=1, padding=1),
            nn.ReLU(inplace=True),
            
            # Layer 5: 13x13x384 -> 13x13x256
            nn.Conv2d(384, 256, kernel_size=3, stride=1, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=3, stride=2),
        )
        
        # Adaptive pooling to handle different input sizes
        self.avgpool = nn.AdaptiveAvgPool2d((6, 6))
        
        # Classification layers (fully connected)
        self.classifier = nn.Sequential(
            nn.Dropout(p=dropout),
            nn.Linear(256 * 6 * 6, 4096),
            nn.ReLU(inplace=True),
            
            nn.Dropout(p=dropout),
            nn.Linear(4096, 4096),
            nn.ReLU(inplace=True),
            
            nn.Linear(4096, num_classes),
        )
    
    def forward(self, x):
        # Feature extraction
        x = self.features(x)
        
        # Adaptive pooling
        x = self.avgpool(x)
        
        # Flatten
        x = torch.flatten(x, 1)
        
        # Classification
        x = self.classifier(x)
        
        return x

class SimplifiedAlexNet(nn.Module):
    """Simplified AlexNet for smaller datasets like CIFAR-10"""
    def __init__(self, num_classes=10, dropout=0.5):
        super(SimplifiedAlexNet, self).__init__()
        
        self.features = nn.Sequential(
            # Layer 1: 32x32x3 -> 8x8x64
            nn.Conv2d(3, 64, kernel_size=5, stride=1, padding=2),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            
            # Layer 2: 8x8x64 -> 4x4x128
            nn.Conv2d(64, 128, kernel_size=3, stride=1, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            
            # Layer 3: 4x4x128 -> 4x4x256
            nn.Conv2d(128, 256, kernel_size=3, stride=1, padding=1),
            nn.ReLU(inplace=True),
            
            # Layer 4: 4x4x256 -> 4x4x256
            nn.Conv2d(256, 256, kernel_size=3, stride=1, padding=1),
            nn.ReLU(inplace=True),
            
            # Layer 5: 4x4x256 -> 2x2x128
            nn.Conv2d(256, 128, kernel_size=3, stride=1, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
        )
        
        self.classifier = nn.Sequential(
            nn.Dropout(p=dropout),
            nn.Linear(128 * 2 * 2, 1024),
            nn.ReLU(inplace=True),
            
            nn.Dropout(p=dropout),
            nn.Linear(1024, 512),
            nn.ReLU(inplace=True),
            
            nn.Linear(512, num_classes),
        )
    
    def forward(self, x):
        x = self.features(x)
        x = torch.flatten(x, 1)
        x = self.classifier(x)
        return x

class SimpleConvNet(nn.Module):
    """Simple CNN for comparison with AlexNet"""
    def __init__(self, num_classes=10):
        super(SimpleConvNet, self).__init__()
        
        self.features = nn.Sequential(
            nn.Conv2d(3, 32, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2),
            
            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(2),
        )
        
        self.classifier = nn.Sequential(
            nn.Linear(64 * 8 * 8, 128),
            nn.ReLU(),
            nn.Linear(128, num_classes),
        )
    
    def forward(self, x):
        x = self.features(x)
        x = torch.flatten(x, 1)
        x = self.classifier(x)
        return x

def train_model(model, train_loader, val_loader, num_epochs=20, learning_rate=0.001, device=None):
    """Train model with proper learning rate scheduling"""
    if device is None:
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    model = model.to(device)
    
    # Loss and optimizer (following original AlexNet paper)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9, weight_decay=5e-4)
    
    # Learning rate scheduler
    scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)
    
    train_losses = []
    train_accuracies = []
    val_losses = []
    val_accuracies = []
    
    print(f"Training on {device}")
    print(f"Model parameters: {sum(p.numel() for p in model.parameters() if p.requires_grad):,}")
    
    start_time = time.time()
    
    for epoch in range(num_epochs):
        # Training phase
        model.train()
        running_loss = 0.0
        correct = 0
        total = 0
        
        for batch_idx, (data, targets) in enumerate(train_loader):
            data, targets = data.to(device), targets.to(device)
            
            optimizer.zero_grad()
            outputs = model(data)
            loss = criterion(outputs, targets)
            loss.backward()
            optimizer.step()
            
            running_loss += loss.item()
            _, predicted = outputs.max(1)
            total += targets.size(0)
            correct += predicted.eq(targets).sum().item()
            
            if batch_idx % 100 == 0:
                print(f'Epoch [{epoch+1}/{num_epochs}], Batch [{batch_idx}/{len(train_loader)}], '
                      f'Loss: {loss.item():.4f}, Acc: {100.*correct/total:.2f}%')
        
        train_loss = running_loss / len(train_loader)
        train_acc = 100. * correct / total
        train_losses.append(train_loss)
        train_accuracies.append(train_acc)
        
        # Validation phase
        model.eval()
        val_loss = 0.0
        correct = 0
        total = 0
        
        with torch.no_grad():
            for data, targets in val_loader:
                data, targets = data.to(device), targets.to(device)
                outputs = model(data)
                loss = criterion(outputs, targets)
                
                val_loss += loss.item()
                _, predicted = outputs.max(1)
                total += targets.size(0)
                correct += predicted.eq(targets).sum().item()
        
        val_loss /= len(val_loader)
        val_acc = 100. * correct / total
        val_losses.append(val_loss)
        val_accuracies.append(val_acc)
        
        # Update learning rate
        scheduler.step()
        current_lr = optimizer.param_groups[0]['lr']
        
        print(f'Epoch [{epoch+1}/{num_epochs}] - Train Loss: {train_loss:.4f}, Train Acc: {train_acc:.2f}%, '
              f'Val Loss: {val_loss:.4f}, Val Acc: {val_acc:.2f}%, LR: {current_lr:.6f}')
    
    training_time = time.time() - start_time
    print(f'Training completed in {training_time:.2f} seconds')
    
    return train_losses, train_accuracies, val_losses, val_accuracies

def visualize_filters(model, layer_name='features.0'):
    """Visualize learned convolutional filters"""
    # Get the specified layer
    layer = dict(model.named_modules())[layer_name]
    
    if isinstance(layer, nn.Conv2d):
        # Get filter weights
        filters = layer.weight.data.cpu().numpy()
        
        # Number of filters to display
        num_filters = min(64, filters.shape[0])
        
        # Create subplot grid
        rows = int(np.sqrt(num_filters))
        cols = int(np.ceil(num_filters / rows))
        
        fig, axes = plt.subplots(rows, cols, figsize=(12, 12))
        if rows == 1:
            axes = axes.reshape(1, -1)
        
        for i in range(num_filters):
            row, col = i // cols, i % cols
            
            # Get filter (take first 3 channels if RGB)
            filter_img = filters[i]
            if filter_img.shape[0] >= 3:
                filter_img = np.transpose(filter_img[:3], (1, 2, 0))
                filter_img = (filter_img - filter_img.min()) / (filter_img.max() - filter_img.min())
            else:
                filter_img = filter_img[0]
            
            axes[row, col].imshow(filter_img, cmap='gray' if len(filter_img.shape) == 2 else None)
            axes[row, col].set_title(f'Filter {i}')
            axes[row, col].axis('off')
        
        # Hide unused subplots
        for i in range(num_filters, rows * cols):
            row, col = i // cols, i % cols
            axes[row, col].axis('off')
        
        plt.suptitle(f'Learned Filters from {layer_name}')
        plt.tight_layout()
        plt.show()
    else:
        print(f"Layer {layer_name} is not a convolutional layer!")

def visualize_feature_maps(model, test_loader, layer_name='features.2'):
    """Visualize feature maps for a test image"""
    model.eval()
    device = next(model.parameters()).device
    
    # Get a test image
    data_iter = iter(test_loader)
    images, labels = next(data_iter)
    test_image = images[0:1].to(device)
    
    # Register hook to capture feature maps
    feature_maps = {}
    def get_activation(name):
        def hook(model, input, output):
            feature_maps[name] = output.detach()
        return hook
    
    # Register hook
    layer = dict(model.named_modules())[layer_name]
    layer.register_forward_hook(get_activation(layer_name))
    
    # Forward pass
    with torch.no_grad():
        _ = model(test_image)
    
    if layer_name in feature_maps:
        features = feature_maps[layer_name].cpu().numpy()[0]  # Remove batch dimension
        
        # Show original image
        plt.figure(figsize=(15, 10))
        
        plt.subplot(2, 8, 1)
        orig_img = images[0].permute(1, 2, 0).numpy()
        if orig_img.min() < 0:  # Denormalize if needed
            orig_img = (orig_img * 0.5) + 0.5
        plt.imshow(orig_img)
        plt.title('Original Image')
        plt.axis('off')
        
        # Show feature maps
        num_features = min(15, features.shape[0])
        for i in range(num_features):
            plt.subplot(2, 8, i + 2)
            plt.imshow(features[i], cmap='viridis')
            plt.title(f'Feature {i}')
            plt.axis('off')
        
        plt.suptitle(f'Feature Maps from {layer_name}')
        plt.tight_layout()
        plt.show()

def compare_architectures():
    """Compare AlexNet with simpler architectures"""
    print("=== Architecture Comparison ===")
    
    # Data preparation (CIFAR-10)
    transform_train = transforms.Compose([
        transforms.RandomCrop(32, padding=4),
        transforms.RandomHorizontalFlip(),
        transforms.ToTensor(),
        transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010)),
    ])
    
    transform_test = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010)),
    ])
    
    train_dataset = torchvision.datasets.CIFAR10(
        root='./data', train=True, download=True, transform=transform_train
    )
    test_dataset = torchvision.datasets.CIFAR10(
        root='./data', train=False, download=True, transform=transform_test
    )
    
    train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=128, shuffle=False)
    
    # Models to compare
    models = {
        'Simple CNN': SimpleConvNet(num_classes=10),
        'Simplified AlexNet': SimplifiedAlexNet(num_classes=10, dropout=0.5),
    }
    
    results = {}
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    for name, model in models.items():
        print(f"\n{'='*50}")
        print(f"Training {name}")
        print('='*50)
        
        # Count parameters
        total_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
        print(f"Parameters: {total_params:,}")
        
        # Train model
        train_losses, train_accs, val_losses, val_accs = train_model(
            model, train_loader, test_loader, num_epochs=15, device=device
        )
        
        results[name] = {
            'model': model,
            'train_losses': train_losses,
            'train_accuracies': train_accs,
            'val_losses': val_losses,
            'val_accuracies': val_accs,
            'final_accuracy': val_accs[-1],
            'parameters': total_params
        }
        
        print(f"Final Test Accuracy: {val_accs[-1]:.2f}%")
    
    # Plot comparison
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    
    # Training loss
    for name, data in results.items():
        axes[0, 0].plot(data['train_losses'], label=name, linewidth=2)
    axes[0, 0].set_title('Training Loss')
    axes[0, 0].set_xlabel('Epoch')
    axes[0, 0].set_ylabel('Loss')
    axes[0, 0].legend()
    axes[0, 0].grid(True)
    
    # Validation loss
    for name, data in results.items():
        axes[0, 1].plot(data['val_losses'], label=name, linewidth=2)
    axes[0, 1].set_title('Test Loss')
    axes[0, 1].set_xlabel('Epoch')
    axes[0, 1].set_ylabel('Loss')
    axes[0, 1].legend()
    axes[0, 1].grid(True)
    
    # Training accuracy
    for name, data in results.items():
        axes[1, 0].plot(data['train_accuracies'], label=name, linewidth=2)
    axes[1, 0].set_title('Training Accuracy')
    axes[1, 0].set_xlabel('Epoch')
    axes[1, 0].set_ylabel('Accuracy (%)')
    axes[1, 0].legend()
    axes[1, 0].grid(True)
    
    # Test accuracy
    for name, data in results.items():
        axes[1, 1].plot(data['val_accuracies'], label=name, linewidth=2)
    axes[1, 1].set_title('Test Accuracy')
    axes[1, 1].set_xlabel('Epoch')
    axes[1, 1].set_ylabel('Accuracy (%)')
    axes[1, 1].legend()
    axes[1, 1].grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Final comparison
    names = list(results.keys())
    accuracies = [results[name]['final_accuracy'] for name in names]
    params = [results[name]['parameters'] for name in names]
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
    
    # Accuracy comparison
    bars1 = ax1.bar(names, accuracies, color=['lightblue', 'lightgreen'])
    ax1.set_title('Final Test Accuracy')
    ax1.set_ylabel('Accuracy (%)')
    ax1.set_ylim(0, 100)
    
    # Add value labels
    for bar, acc in zip(bars1, accuracies):
        height = bar.get_height()
        ax1.text(bar.get_x() + bar.get_width()/2., height + 1,
                f'{acc:.1f}%', ha='center', va='bottom')
    
    # Parameter comparison
    bars2 = ax2.bar(names, params, color=['lightcoral', 'lightyellow'])
    ax2.set_title('Model Parameters')
    ax2.set_ylabel('Number of Parameters')
    ax2.set_yscale('log')
    
    # Add value labels
    for bar, param in zip(bars2, params):
        height = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width()/2., height * 1.1,
                f'{param/1e6:.1f}M', ha='center', va='bottom')
    
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.show()
    
    return results

def analyze_alexnet_innovations():
    """Analyze key innovations introduced by AlexNet"""
    print("\n=== AlexNet Innovation Analysis ===")
    
    # Create visualization of architectural differences
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    
    # ReLU vs Tanh activation comparison
    x = np.linspace(-3, 3, 1000)
    relu = np.maximum(0, x)
    tanh = np.tanh(x)
    sigmoid = 1 / (1 + np.exp(-x))
    
    axes[0, 0].plot(x, relu, label='ReLU', linewidth=2)
    axes[0, 0].plot(x, tanh, label='Tanh', linewidth=2)
    axes[0, 0].plot(x, sigmoid, label='Sigmoid', linewidth=2)
    axes[0, 0].set_title('Activation Functions')
    axes[0, 0].set_xlabel('Input')
    axes[0, 0].set_ylabel('Output')
    axes[0, 0].legend()
    axes[0, 0].grid(True)
    axes[0, 0].text(0.5, 2, 'ReLU helps with\nvanishing gradient', 
                   bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow"))
    
    # Dropout effect simulation
    np.random.seed(42)
    neurons = 100
    keep_prob = [1.0, 0.8, 0.5, 0.2]
    overfitting = []
    
    for p in keep_prob:
        # Simulate overfitting reduction
        base_overfit = 0.3
        dropout_effect = base_overfit * (1 - p) * 0.8
        overfitting.append(base_overfit - dropout_effect)
    
    axes[0, 1].plot(keep_prob, overfitting, 'bo-', linewidth=2, markersize=8)
    axes[0, 1].set_title('Dropout Effect on Overfitting')
    axes[0, 1].set_xlabel('Keep Probability')
    axes[0, 1].set_ylabel('Overfitting (Lower is Better)')
    axes[0, 1].grid(True)
    axes[0, 1].invert_xaxis()
    
    # Network depth comparison
    architectures = ['LeNet-5\n(1998)', 'AlexNet\n(2012)', 'VGG-16\n(2014)', 'ResNet-50\n(2015)']
    depths = [5, 8, 16, 50]
    accuracies = [98.0, 84.7, 92.7, 96.4]  # ImageNet Top-5 accuracy (approximate)
    
    bars = axes[1, 0].bar(architectures, depths, color=['lightblue', 'orange', 'lightgreen', 'lightcoral'])
    axes[1, 0].set_title('Network Depth Evolution')
    axes[1, 0].set_ylabel('Number of Layers')
    
    # Add accuracy as text
    for bar, acc in zip(bars, accuracies):
        height = bar.get_height()
        axes[1, 0].text(bar.get_x() + bar.get_width()/2., height + 1,
                       f'{acc}%\nTop-5', ha='center', va='bottom', fontsize=8)
    
    # Data augmentation impact
    augmentation_types = ['None', 'Crop\n& Flip', '+ Color\nJitter', '+ Rotation\n& Scale']
    accuracy_gain = [75.0, 78.5, 81.2, 82.8]
    
    axes[1, 1].plot(range(len(augmentation_types)), accuracy_gain, 'go-', linewidth=2, markersize=8)
    axes[1, 1].set_title('Data Augmentation Impact')
    axes[1, 1].set_xlabel('Augmentation Complexity')
    axes[1, 1].set_ylabel('Test Accuracy (%)')
    axes[1, 1].set_xticks(range(len(augmentation_types)))
    axes[1, 1].set_xticklabels(augmentation_types)
    axes[1, 1].grid(True)
    
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== AlexNet Implementation and Analysis ===")
    
    # Show AlexNet architecture
    print("\n=== AlexNet Architecture ===")
    model = AlexNet(num_classes=1000)
    print(f"AlexNet Parameters: {sum(p.numel() for p in model.parameters()):,}")
    
    simplified_model = SimplifiedAlexNet(num_classes=10)
    print(f"Simplified AlexNet Parameters: {sum(p.numel() for p in simplified_model.parameters()):,}")
    
    # Compare architectures on CIFAR-10
    comparison_results = compare_architectures()
    
    # Analyze innovations
    analyze_alexnet_innovations()
    
    # Visualize filters from trained model
    if comparison_results:
        best_model = comparison_results['Simplified AlexNet']['model']
        visualize_filters(best_model, 'features.0')
    
    print("\n=== AlexNet Key Innovations ===")
    print("1. RELU ACTIVATION:")
    print("   - f(x) = max(0, x)")
    print("   - Eliminates vanishing gradient problem")
    print("   - Faster training than sigmoid/tanh")
    print("   - Sparse activation patterns")
    
    print("\n2. DROPOUT REGULARIZATION:")
    print("   - Randomly set neurons to 0 during training")
    print("   - Prevents overfitting in large networks")
    print("   - p=0.5 in fully connected layers")
    print("   - Ensemble effect improves generalization")
    
    print("\n3. LOCAL RESPONSE NORMALIZATION:")
    print("   - Normalize activations across feature maps")
    print("   - Inspired by biological lateral inhibition")
    print("   - Helps with generalization")
    print("   - Later replaced by Batch Normalization")
    
    print("\n4. DATA AUGMENTATION:")
    print("   - Random crops and horizontal flips")
    print("   - Color jittering (PCA on RGB)")
    print("   - Increases effective dataset size")
    print("   - Crucial for preventing overfitting")
    
    print("\n5. GPU PARALLELIZATION:")
    print("   - First CNN trained on GPUs")
    print("   - Split network across 2 GTX 580s")
    print("   - Enabled training deep networks")
    print("   - Started the GPU computing revolution")
    
    print("\n=== AlexNet Impact ===")
    print("✓ Won ImageNet 2012 with 15.3% error (vs 26.2% runner-up)")
    print("✓ Demonstrated power of deep learning")
    print("✓ Sparked the deep learning revolution")
    print("✓ Influenced all subsequent CNN architectures")
    print("✓ Established importance of ReLU and dropout")
    print("✓ Showed value of data augmentation")
    
    print("\n=== Architecture Details ===")
    print("• Input: 224×224×3 RGB images")
    print("• 5 convolutional layers + 3 fully connected")
    print("• 60 million parameters, 650,000 neurons")
    print("• ReLU activation throughout")
    print("• Max pooling with overlapping windows")
    print("• Dropout in FC layers (p=0.5)")
    
    print("\n=== Training Innovations ===")
    print("• SGD with momentum (0.9)")
    print("• Weight decay (5×10⁻⁴)")
    print("• Learning rate reduction by factor of 10")
    print("• Batch size 128")
    print("• 90 epochs on ImageNet")
    
    print("\n=== Comparison with Earlier Methods ===")
    print("BEFORE ALEXNET (2012):")
    print("• Hand-crafted features (SIFT, HOG)")
    print("• Shallow classifiers (SVM, Random Forest)")
    print("• Limited by feature engineering")
    print("• ImageNet error rate: ~25-30%")
    
    print("\nALEXNET BREAKTHROUGH:")
    print("• End-to-end learning")
    print("• Automatic feature extraction")
    print("• Deep hierarchical representations")
    print("• ImageNet error rate: 15.3%")
    
    print("\n=== Legacy and Influence ===")
    print("• Launched deep learning boom")
    print("• Inspired VGG, GoogLeNet, ResNet")
    print("• Established CNN as gold standard")
    print("• Proved importance of scale and compute")
    print("• Led to GPU-first AI development")
    
    print("\n=== Modern Perspective ===")
    print("OUTDATED COMPONENTS:")
    print("❌ Local Response Normalization → Batch Norm")
    print("❌ Large filter sizes → 3×3 filters")
    print("❌ Full connections → Global Average Pooling")
    
    print("\nTIMELESS CONTRIBUTIONS:")
    print("✓ ReLU activation")
    print("✓ Dropout regularization")
    print("✓ Data augmentation")
    print("✓ Deep CNN architecture")
    print("✓ End-to-end learning paradigm")