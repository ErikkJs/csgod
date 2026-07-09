# author: csgod cmps studies
# ResNet - Residual connections and skip connections
# ResNet introduced skip connections that allow gradients to flow directly through the network,
# enabling training of very deep networks and solving the vanishing gradient problem.

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.data import DataLoader
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np

class BasicBlock(nn.Module):
    """Basic ResNet block with two 3x3 convolutions"""
    expansion = 1
    
    def __init__(self, in_channels, out_channels, stride=1, downsample=None):
        super(BasicBlock, self).__init__()
        
        # First convolution
        self.conv1 = nn.Conv2d(in_channels, out_channels, kernel_size=3, 
                              stride=stride, padding=1, bias=False)
        self.bn1 = nn.BatchNorm2d(out_channels)
        
        # Second convolution
        self.conv2 = nn.Conv2d(out_channels, out_channels, kernel_size=3, 
                              stride=1, padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(out_channels)
        
        # Downsample layer for skip connection (if needed)
        self.downsample = downsample
        self.stride = stride
    
    def forward(self, x):
        identity = x
        
        # First conv block
        out = F.relu(self.bn1(self.conv1(x)))
        
        # Second conv block
        out = self.bn2(self.conv2(out))
        
        # Skip connection
        if self.downsample is not None:
            identity = self.downsample(x)
        
        out += identity  # Add skip connection
        out = F.relu(out)  # Apply ReLU after addition
        
        return out

class Bottleneck(nn.Module):
    """Bottleneck block for deeper ResNets (ResNet-50, 101, 152)"""
    expansion = 4
    
    def __init__(self, in_channels, out_channels, stride=1, downsample=None):
        super(Bottleneck, self).__init__()
        
        # 1x1 convolution (reduce dimensions)
        self.conv1 = nn.Conv2d(in_channels, out_channels, kernel_size=1, bias=False)
        self.bn1 = nn.BatchNorm2d(out_channels)
        
        # 3x3 convolution
        self.conv2 = nn.Conv2d(out_channels, out_channels, kernel_size=3,
                              stride=stride, padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(out_channels)
        
        # 1x1 convolution (expand dimensions)
        self.conv3 = nn.Conv2d(out_channels, out_channels * self.expansion,
                              kernel_size=1, bias=False)
        self.bn3 = nn.BatchNorm2d(out_channels * self.expansion)
        
        self.downsample = downsample
        self.stride = stride
    
    def forward(self, x):
        identity = x
        
        # 1x1 conv (reduce)
        out = F.relu(self.bn1(self.conv1(x)))
        
        # 3x3 conv
        out = F.relu(self.bn2(self.conv2(out)))
        
        # 1x1 conv (expand)
        out = self.bn3(self.conv3(out))
        
        # Skip connection
        if self.downsample is not None:
            identity = self.downsample(x)
        
        out += identity
        out = F.relu(out)
        
        return out

class ResNet(nn.Module):
    """ResNet architecture"""
    def __init__(self, block, layers, num_classes=10, input_channels=3):
        super(ResNet, self).__init__()
        self.in_channels = 64
        
        # Initial convolution and pooling
        self.conv1 = nn.Conv2d(input_channels, 64, kernel_size=7, stride=2, padding=3, bias=False)
        self.bn1 = nn.BatchNorm2d(64)
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        
        # Residual layers
        self.layer1 = self._make_layer(block, 64, layers[0])
        self.layer2 = self._make_layer(block, 128, layers[1], stride=2)
        self.layer3 = self._make_layer(block, 256, layers[2], stride=2)
        self.layer4 = self._make_layer(block, 512, layers[3], stride=2)
        
        # Final layers
        self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
        self.fc = nn.Linear(512 * block.expansion, num_classes)
        
        # Initialize weights
        self._initialize_weights()
    
    def _make_layer(self, block, out_channels, blocks, stride=1):
        """Create a residual layer with multiple blocks"""
        downsample = None
        
        # Create downsample layer if needed
        if stride != 1 or self.in_channels != out_channels * block.expansion:
            downsample = nn.Sequential(
                nn.Conv2d(self.in_channels, out_channels * block.expansion,
                         kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(out_channels * block.expansion)
            )
        
        layers = []
        # First block (potentially with downsampling)
        layers.append(block(self.in_channels, out_channels, stride, downsample))
        self.in_channels = out_channels * block.expansion
        
        # Remaining blocks
        for _ in range(1, blocks):
            layers.append(block(self.in_channels, out_channels))
        
        return nn.Sequential(*layers)
    
    def _initialize_weights(self):
        """Initialize weights using He initialization"""
        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                nn.init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')
            elif isinstance(m, nn.BatchNorm2d):
                nn.init.constant_(m.weight, 1)
                nn.init.constant_(m.bias, 0)
    
    def forward(self, x):
        # Initial layers
        x = F.relu(self.bn1(self.conv1(x)))
        x = self.maxpool(x)
        
        # Residual layers
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)
        
        # Final classification
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.fc(x)
        
        return x

def resnet18(num_classes=10):
    """ResNet-18 model"""
    return ResNet(BasicBlock, [2, 2, 2, 2], num_classes)

def resnet34(num_classes=10):
    """ResNet-34 model"""
    return ResNet(BasicBlock, [3, 4, 6, 3], num_classes)

def resnet50(num_classes=10):
    """ResNet-50 model"""
    return ResNet(Bottleneck, [3, 4, 6, 3], num_classes)

class SimpleNet(nn.Module):
    """Simple CNN without skip connections for comparison"""
    def __init__(self, num_classes=10):
        super(SimpleNet, self).__init__()
        self.features = nn.Sequential(
            # Block 1
            nn.Conv2d(3, 64, 3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(),
            nn.Conv2d(64, 64, 3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(),
            nn.MaxPool2d(2),
            
            # Block 2
            nn.Conv2d(64, 128, 3, padding=1),
            nn.BatchNorm2d(128),
            nn.ReLU(),
            nn.Conv2d(128, 128, 3, padding=1),
            nn.BatchNorm2d(128),
            nn.ReLU(),
            nn.MaxPool2d(2),
            
            # Block 3
            nn.Conv2d(128, 256, 3, padding=1),
            nn.BatchNorm2d(256),
            nn.ReLU(),
            nn.Conv2d(256, 256, 3, padding=1),
            nn.BatchNorm2d(256),
            nn.ReLU(),
            nn.MaxPool2d(2),
            
            # Block 4
            nn.Conv2d(256, 512, 3, padding=1),
            nn.BatchNorm2d(512),
            nn.ReLU(),
            nn.Conv2d(512, 512, 3, padding=1),
            nn.BatchNorm2d(512),
            nn.ReLU(),
            nn.MaxPool2d(2)
        )
        
        self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
        self.classifier = nn.Linear(512, num_classes)
    
    def forward(self, x):
        x = self.features(x)
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.classifier(x)
        return x

def train_model(model, train_loader, test_loader, num_epochs=10, learning_rate=0.001):
    """Train a model and return training history"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=learning_rate, weight_decay=1e-4)
    scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)
    
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
        
        scheduler.step()
        
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

def compare_with_without_skip_connections():
    """Compare ResNet with and without skip connections"""
    print("=== Comparing ResNet vs Simple CNN ===")
    
    # Data loading
    transform_train = transforms.Compose([
        transforms.RandomCrop(32, padding=4),
        transforms.RandomHorizontalFlip(),
        transforms.ToTensor(),
        transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010))
    ])
    
    transform_test = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010))
    ])
    
    # Load CIFAR-10
    train_dataset = torchvision.datasets.CIFAR10(
        root='./data', train=True, download=True, transform=transform_train
    )
    test_dataset = torchvision.datasets.CIFAR10(
        root='./data', train=False, download=True, transform=transform_test
    )
    
    train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True, num_workers=2)
    test_loader = DataLoader(test_dataset, batch_size=128, shuffle=False, num_workers=2)
    
    print(f"Training samples: {len(train_dataset)}")
    print(f"Test samples: {len(test_dataset)}")
    
    # Models to compare
    models = {
        'ResNet-18': resnet18(num_classes=10),
        'Simple CNN': SimpleNet(num_classes=10)
    }
    
    results = {}
    
    for model_name, model in models.items():
        print(f"\n=== Training {model_name} ===")
        
        # Count parameters
        total_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
        print(f"Total parameters: {total_params:,}")
        
        # Train model
        train_losses, train_accs, test_accs = train_model(
            model, train_loader, test_loader, num_epochs=5, learning_rate=0.001
        )
        
        results[model_name] = {
            'train_losses': train_losses,
            'train_accuracies': train_accs,
            'test_accuracies': test_accs,
            'final_test_acc': test_accs[-1],
            'parameters': total_params
        }
        
        print(f"{model_name} Final Test Accuracy: {test_accs[-1]:.2f}%")
    
    # Visualize results
    fig, axes = plt.subplots(1, 3, figsize=(18, 5))
    
    epochs = range(1, len(results['ResNet-18']['train_losses']) + 1)
    
    # Training loss
    for model_name, data in results.items():
        axes[0].plot(epochs, data['train_losses'], 'o-', label=model_name, linewidth=2)
    axes[0].set_title('Training Loss')
    axes[0].set_xlabel('Epoch')
    axes[0].set_ylabel('Loss')
    axes[0].legend()
    axes[0].grid(True)
    
    # Training accuracy
    for model_name, data in results.items():
        axes[1].plot(epochs, data['train_accuracies'], 'o-', label=model_name, linewidth=2)
    axes[1].set_title('Training Accuracy')
    axes[1].set_xlabel('Epoch')
    axes[1].set_ylabel('Accuracy (%)')
    axes[1].legend()
    axes[1].grid(True)
    
    # Test accuracy
    for model_name, data in results.items():
        axes[2].plot(epochs, data['test_accuracies'], 'o-', label=model_name, linewidth=2)
    axes[2].set_title('Test Accuracy')
    axes[2].set_xlabel('Epoch')
    axes[2].set_ylabel('Accuracy (%)')
    axes[2].legend()
    axes[2].grid(True)
    
    plt.tight_layout()
    plt.show()
    
    return results

def analyze_gradient_flow():
    """Analyze gradient flow in ResNet vs simple CNN"""
    print("\n=== Gradient Flow Analysis ===")
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    # Create models
    resnet = resnet18(num_classes=10).to(device)
    simple_cnn = SimpleNet(num_classes=10).to(device)
    
    # Create dummy input
    x = torch.randn(1, 3, 32, 32).to(device)
    y = torch.randint(0, 10, (1,)).to(device)
    
    criterion = nn.CrossEntropyLoss()
    
    # Function to get gradient norms
    def get_gradient_norms(model, x, y):
        model.zero_grad()
        output = model(x)
        loss = criterion(output, y)
        loss.backward()
        
        gradient_norms = []
        layer_names = []
        
        for name, param in model.named_parameters():
            if param.grad is not None:
                grad_norm = param.grad.data.norm(2).item()
                gradient_norms.append(grad_norm)
                layer_names.append(name)
        
        return gradient_norms, layer_names
    
    # Get gradient norms
    resnet_grads, resnet_names = get_gradient_norms(resnet, x, y)
    simple_grads, simple_names = get_gradient_norms(simple_cnn, x, y)
    
    # Plot gradient norms (first few layers for visualization)
    plt.figure(figsize=(15, 6))
    
    # ResNet gradients
    plt.subplot(1, 2, 1)
    plt.semilogy(resnet_grads[:20], 'bo-', label='ResNet-18')  # First 20 layers
    plt.title('Gradient Norms - ResNet-18')
    plt.xlabel('Layer Index')
    plt.ylabel('Gradient Norm (log scale)')
    plt.grid(True)
    
    # Simple CNN gradients
    plt.subplot(1, 2, 2)
    plt.semilogy(simple_grads[:20], 'ro-', label='Simple CNN')  # First 20 layers
    plt.title('Gradient Norms - Simple CNN')
    plt.xlabel('Layer Index')
    plt.ylabel('Gradient Norm (log scale)')
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()
    
    print(f"ResNet average gradient norm: {np.mean(resnet_grads):.6f}")
    print(f"Simple CNN average gradient norm: {np.mean(simple_grads):.6f}")

def visualize_skip_connections():
    """Visualize how skip connections work"""
    print("\n=== Skip Connection Visualization ===")
    
    # Create a simple example showing skip connection
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    # Without skip connections
    ax1.set_title('Traditional CNN (Without Skip Connections)')
    layers = ['Input', 'Conv1', 'Conv2', 'Conv3', 'Conv4', 'Output']
    y_positions = np.arange(len(layers))
    
    # Draw layers
    for i, (layer, y) in enumerate(zip(layers, y_positions)):
        ax1.text(0, y, layer, ha='center', va='center', 
                bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue"))
        
        # Draw connections
        if i < len(layers) - 1:
            ax1.annotate('', xy=(0, y_positions[i+1] - 0.3), xytext=(0, y + 0.3),
                        arrowprops=dict(arrowstyle='->', lw=2, color='black'))
    
    ax1.set_xlim(-0.5, 0.5)
    ax1.set_ylim(-0.5, len(layers) - 0.5)
    ax1.axis('off')
    
    # With skip connections
    ax2.set_title('ResNet (With Skip Connections)')
    
    # Draw layers
    for i, (layer, y) in enumerate(zip(layers, y_positions)):
        ax2.text(0, y, layer, ha='center', va='center', 
                bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgreen"))
        
        # Draw main connections
        if i < len(layers) - 1:
            ax2.annotate('', xy=(0, y_positions[i+1] - 0.3), xytext=(0, y + 0.3),
                        arrowprops=dict(arrowstyle='->', lw=2, color='black'))
    
    # Draw skip connections
    skip_connections = [(1, 3), (2, 4), (3, 5)]  # Conv1->Conv3, etc.
    for start, end in skip_connections:
        ax2.annotate('', xy=(0.3, y_positions[end] - 0.2), xytext=(0.3, y_positions[start] + 0.2),
                    arrowprops=dict(arrowstyle='->', lw=2, color='red', 
                                  connectionstyle="arc3,rad=0.3"))
        ax2.text(0.45, (y_positions[start] + y_positions[end])/2, 'Skip',
                ha='center', va='center', color='red', fontweight='bold')
    
    ax2.set_xlim(-0.5, 0.7)
    ax2.set_ylim(-0.5, len(layers) - 0.5)
    ax2.axis('off')
    
    plt.tight_layout()
    plt.show()

def compare_resnet_variants():
    """Compare different ResNet variants"""
    print("\n=== ResNet Variants Comparison ===")
    
    variants = {
        'ResNet-18': {'model': resnet18, 'blocks': [2, 2, 2, 2], 'block_type': 'Basic'},
        'ResNet-34': {'model': resnet34, 'blocks': [3, 4, 6, 3], 'block_type': 'Basic'},
        'ResNet-50': {'model': resnet50, 'blocks': [3, 4, 6, 3], 'block_type': 'Bottleneck'},
    }
    
    results = []
    
    for name, info in variants.items():
        model = info['model'](num_classes=1000)  # ImageNet classes
        
        # Count parameters
        total_params = sum(p.numel() for p in model.parameters())
        
        # Estimate memory (rough approximation)
        memory_mb = total_params * 4 / (1024 * 1024)  # 4 bytes per float32
        
        results.append({
            'name': name,
            'parameters': total_params,
            'memory_mb': memory_mb,
            'blocks': info['blocks'],
            'block_type': info['block_type']
        })
        
        print(f"{name}:")
        print(f"  Block type: {info['block_type']}")
        print(f"  Blocks per layer: {info['blocks']}")
        print(f"  Parameters: {total_params:,}")
        print(f"  Memory: ~{memory_mb:.1f} MB")
        print()
    
    # Visualize comparison
    names = [r['name'] for r in results]
    params = [r['parameters'] for r in results]
    
    plt.figure(figsize=(10, 6))
    bars = plt.bar(names, params, color=['lightblue', 'lightgreen', 'lightcoral'])
    plt.title('ResNet Variants - Parameter Count')
    plt.xlabel('Model')
    plt.ylabel('Number of Parameters')
    plt.yscale('log')
    
    # Add value labels on bars
    for bar, param_count in zip(bars, params):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{param_count/1e6:.1f}M', ha='center', va='bottom')
    
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== ResNet Implementation and Analysis ===")
    
    # Compare ResNet vs Simple CNN
    comparison_results = compare_with_without_skip_connections()
    
    # Analyze gradient flow
    analyze_gradient_flow()
    
    # Visualize skip connections
    visualize_skip_connections()
    
    # Compare ResNet variants
    compare_resnet_variants()
    
    print("\n=== ResNet Key Innovations ===")
    print("1. SKIP CONNECTIONS: Direct paths for gradients")
    print("   - F(x) = H(x) - x, learn residual mapping")
    print("   - Easier to optimize identity mapping")
    print("   - Enables very deep networks (50, 101, 152 layers)")
    
    print("\n2. RESIDUAL LEARNING:")
    print("   - Instead of learning H(x), learn F(x) = H(x) - x")
    print("   - If identity mapping is optimal, easier to push F(x) to zero")
    print("   - Reduces learning difficulty for deeper networks")
    
    print("\n3. BATCH NORMALIZATION:")
    print("   - Normalizes inputs to each layer")
    print("   - Reduces internal covariate shift")
    print("   - Enables higher learning rates")
    
    print("\n4. ARCHITECTURAL CHOICES:")
    print("   - Basic Block: Two 3x3 convolutions")
    print("   - Bottleneck Block: 1x1 -> 3x3 -> 1x1 (for deeper networks)")
    print("   - Global Average Pooling instead of FC layers")
    
    print("\n=== Why Skip Connections Work ===")
    print("✓ GRADIENT FLOW: Direct path for gradients to early layers")
    print("✓ IDENTITY MAPPING: Network can learn identity if needed")
    print("✓ FEATURE REUSE: Later layers can access earlier features")
    print("✓ TRAINING STABILITY: Reduces vanishing gradient problem")
    print("✓ REPRESENTATIONAL POWER: Doesn't hurt if connections not needed")
    
    print("\n=== ResNet Impact ===")
    print("• Won ImageNet 2015 with 3.57% error (human-level)")
    print("• Enabled training of very deep networks (1000+ layers)")
    print("• Influenced many subsequent architectures")
    print("• Skip connections became standard in deep learning")
    print("• Inspired DenseNet, Highway Networks, FPN, etc.")
    
    print("\n=== Practical Applications ===")
    print("• Image classification (ImageNet)")
    print("• Object detection (Faster R-CNN backbone)")
    print("• Semantic segmentation")
    print("• Feature extraction for other tasks")
    print("• Transfer learning base model")