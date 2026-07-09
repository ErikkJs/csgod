# author: csgod cmps studies
# VGG - Very Deep Convolutional Networks
# VGG demonstrated that depth matters by using very small (3x3) filters
# and achieving excellent results on ImageNet with 16-19 layer networks.

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.utils.data import DataLoader
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np
import time

class VGG(nn.Module):
    """VGG network implementation"""
    def __init__(self, features, num_classes=1000, init_weights=True, dropout=0.5):
        super(VGG, self).__init__()
        
        self.features = features
        
        # Adaptive pooling to handle different input sizes
        self.avgpool = nn.AdaptiveAvgPool2d((7, 7))
        
        # Classifier
        self.classifier = nn.Sequential(
            nn.Linear(512 * 7 * 7, 4096),
            nn.ReLU(True),
            nn.Dropout(p=dropout),
            
            nn.Linear(4096, 4096),
            nn.ReLU(True),
            nn.Dropout(p=dropout),
            
            nn.Linear(4096, num_classes),
        )
        
        if init_weights:
            self._initialize_weights()
    
    def forward(self, x):
        x = self.features(x)
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.classifier(x)
        return x
    
    def _initialize_weights(self):
        """Initialize weights following VGG paper"""
        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                nn.init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')
                if m.bias is not None:
                    nn.init.constant_(m.bias, 0)
            elif isinstance(m, nn.BatchNorm2d):
                nn.init.constant_(m.weight, 1)
                nn.init.constant_(m.bias, 0)
            elif isinstance(m, nn.Linear):
                nn.init.normal_(m.weight, 0, 0.01)
                nn.init.constant_(m.bias, 0)

def make_layers(cfg, batch_norm=False):
    """Create VGG feature layers from configuration"""
    layers = []
    in_channels = 3
    
    for v in cfg:
        if v == 'M':
            layers += [nn.MaxPool2d(kernel_size=2, stride=2)]
        else:
            conv2d = nn.Conv2d(in_channels, v, kernel_size=3, padding=1)
            if batch_norm:
                layers += [conv2d, nn.BatchNorm2d(v), nn.ReLU(inplace=True)]
            else:
                layers += [conv2d, nn.ReLU(inplace=True)]
            in_channels = v
    
    return nn.Sequential(*layers)

# VGG configurations
vgg_configs = {
    'VGG11': [64, 'M', 128, 'M', 256, 256, 'M', 512, 512, 'M', 512, 512, 'M'],
    'VGG13': [64, 64, 'M', 128, 128, 'M', 256, 256, 'M', 512, 512, 'M', 512, 512, 'M'],
    'VGG16': [64, 64, 'M', 128, 128, 'M', 256, 256, 256, 'M', 512, 512, 512, 'M', 512, 512, 512, 'M'],
    'VGG19': [64, 64, 'M', 128, 128, 'M', 256, 256, 256, 256, 'M', 512, 512, 512, 512, 'M', 512, 512, 512, 512, 'M'],
}

def vgg11(num_classes=1000, batch_norm=False, **kwargs):
    """VGG 11-layer model"""
    features = make_layers(vgg_configs['VGG11'], batch_norm=batch_norm)
    model = VGG(features, num_classes=num_classes, **kwargs)
    return model

def vgg16(num_classes=1000, batch_norm=False, **kwargs):
    """VGG 16-layer model"""
    features = make_layers(vgg_configs['VGG16'], batch_norm=batch_norm)
    model = VGG(features, num_classes=num_classes, **kwargs)
    return model

def vgg19(num_classes=1000, batch_norm=False, **kwargs):
    """VGG 19-layer model"""
    features = make_layers(vgg_configs['VGG19'], batch_norm=batch_norm)
    model = VGG(features, num_classes=num_classes, **kwargs)
    return model

class SimplifiedVGG(nn.Module):
    """Simplified VGG for smaller datasets like CIFAR-10"""
    def __init__(self, num_classes=10, dropout=0.5):
        super(SimplifiedVGG, self).__init__()
        
        # Feature extraction layers (similar to VGG16 but smaller)
        self.features = nn.Sequential(
            # Block 1: 32x32 -> 16x16
            nn.Conv2d(3, 64, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(64, 64, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            
            # Block 2: 16x16 -> 8x8
            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(128, 128, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            
            # Block 3: 8x8 -> 4x4
            nn.Conv2d(128, 256, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(256, 256, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(256, 256, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            
            # Block 4: 4x4 -> 2x2
            nn.Conv2d(256, 512, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(512, 512, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(512, 512, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            
            # Block 5: 2x2 -> 1x1
            nn.Conv2d(512, 512, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(512, 512, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(512, 512, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
        )
        
        # Classifier
        self.classifier = nn.Sequential(
            nn.Linear(512 * 1 * 1, 4096),
            nn.ReLU(True),
            nn.Dropout(p=dropout),
            
            nn.Linear(4096, 1024),
            nn.ReLU(True),
            nn.Dropout(p=dropout),
            
            nn.Linear(1024, num_classes),
        )
    
    def forward(self, x):
        x = self.features(x)
        x = torch.flatten(x, 1)
        x = self.classifier(x)
        return x

class VGGWithBatchNorm(nn.Module):
    """VGG with Batch Normalization for comparison"""
    def __init__(self, num_classes=10, dropout=0.5):
        super(VGGWithBatchNorm, self).__init__()
        
        self.features = nn.Sequential(
            # Block 1
            nn.Conv2d(3, 64, kernel_size=3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(inplace=True),
            nn.Conv2d(64, 64, kernel_size=3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            
            # Block 2
            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.BatchNorm2d(128),
            nn.ReLU(inplace=True),
            nn.Conv2d(128, 128, kernel_size=3, padding=1),
            nn.BatchNorm2d(128),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            
            # Block 3
            nn.Conv2d(128, 256, kernel_size=3, padding=1),
            nn.BatchNorm2d(256),
            nn.ReLU(inplace=True),
            nn.Conv2d(256, 256, kernel_size=3, padding=1),
            nn.BatchNorm2d(256),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
            
            # Block 4
            nn.Conv2d(256, 512, kernel_size=3, padding=1),
            nn.BatchNorm2d(512),
            nn.ReLU(inplace=True),
            nn.Conv2d(512, 512, kernel_size=3, padding=1),
            nn.BatchNorm2d(512),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
        )
        
        self.classifier = nn.Sequential(
            nn.Linear(512 * 2 * 2, 1024),
            nn.ReLU(True),
            nn.Dropout(p=dropout),
            nn.Linear(1024, num_classes),
        )
    
    def forward(self, x):
        x = self.features(x)
        x = torch.flatten(x, 1)
        x = self.classifier(x)
        return x

def train_model(model, train_loader, test_loader, num_epochs=20, learning_rate=0.01, device=None):
    """Train VGG model"""
    if device is None:
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    model = model.to(device)
    
    # Loss and optimizer (following VGG paper)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9, weight_decay=5e-4)
    
    # Learning rate scheduler
    scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)
    
    train_losses = []
    train_accuracies = []
    test_losses = []
    test_accuracies = []
    
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
        
        # Test phase
        model.eval()
        test_loss = 0.0
        correct = 0
        total = 0
        
        with torch.no_grad():
            for data, targets in test_loader:
                data, targets = data.to(device), targets.to(device)
                outputs = model(data)
                loss = criterion(outputs, targets)
                
                test_loss += loss.item()
                _, predicted = outputs.max(1)
                total += targets.size(0)
                correct += predicted.eq(targets).sum().item()
        
        test_loss /= len(test_loader)
        test_acc = 100. * correct / total
        test_losses.append(test_loss)
        test_accuracies.append(test_acc)
        
        scheduler.step()
        current_lr = optimizer.param_groups[0]['lr']
        
        print(f'Epoch [{epoch+1}/{num_epochs}] - Train Loss: {train_loss:.4f}, Train Acc: {train_acc:.2f}%, '
              f'Test Loss: {test_loss:.4f}, Test Acc: {test_acc:.2f}%, LR: {current_lr:.6f}')
    
    training_time = time.time() - start_time
    print(f'Training completed in {training_time:.2f} seconds')
    
    return train_losses, train_accuracies, test_losses, test_accuracies

def compare_vgg_variants():
    """Compare different VGG variants and modifications"""
    print("=== VGG Variants Comparison ===")
    
    # Data preparation
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
        'VGG11': vgg11(num_classes=10),
        'VGG16 (Simplified)': SimplifiedVGG(num_classes=10),
        'VGG + BatchNorm': VGGWithBatchNorm(num_classes=10),
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
        
        # Train model (reduced epochs for demo)
        train_losses, train_accs, test_losses, test_accs = train_model(
            model, train_loader, test_loader, num_epochs=12, device=device
        )
        
        results[name] = {
            'model': model,
            'train_losses': train_losses,
            'train_accuracies': train_accs,
            'test_losses': test_losses,
            'test_accuracies': test_accs,
            'final_accuracy': test_accs[-1],
            'parameters': total_params
        }
        
        print(f"Final Test Accuracy: {test_accs[-1]:.2f}%")
    
    return results

def analyze_receptive_field():
    """Analyze receptive field of VGG networks"""
    print("\n=== Receptive Field Analysis ===")
    
    def calculate_receptive_field(layers_info):
        """Calculate receptive field size"""
        rf = 1
        stride = 1
        
        for kernel_size, layer_stride in layers_info:
            rf = rf + (kernel_size - 1) * stride
            stride = stride * layer_stride
        
        return rf
    
    # VGG-style layers: (kernel_size, stride)
    vgg_layers = [
        (3, 1), (3, 1), (2, 2),  # Block 1: conv + conv + pool
        (3, 1), (3, 1), (2, 2),  # Block 2: conv + conv + pool
        (3, 1), (3, 1), (3, 1), (2, 2),  # Block 3: conv + conv + conv + pool
        (3, 1), (3, 1), (3, 1), (2, 2),  # Block 4: conv + conv + conv + pool
        (3, 1), (3, 1), (3, 1), (2, 2),  # Block 5: conv + conv + conv + pool
    ]
    
    # Calculate receptive field at each layer
    rf_sizes = []
    partial_layers = []
    
    for i in range(len(vgg_layers)):
        partial_layers.append(vgg_layers[i])
        rf = calculate_receptive_field(partial_layers)
        rf_sizes.append(rf)
        
        if i % 3 == 2:  # After each block
            print(f"After Block {i//3 + 1}: Receptive Field = {rf}×{rf}")
    
    # Plot receptive field growth
    plt.figure(figsize=(12, 5))
    
    plt.subplot(1, 2, 1)
    plt.plot(range(1, len(rf_sizes) + 1), rf_sizes, 'bo-', linewidth=2, markersize=6)
    plt.title('Receptive Field Growth in VGG')
    plt.xlabel('Layer Number')
    plt.ylabel('Receptive Field Size')
    plt.grid(True)
    
    # Add block boundaries
    block_boundaries = [3, 6, 10, 14, 18]
    for i, boundary in enumerate(block_boundaries):
        plt.axvline(x=boundary, color='red', linestyle='--', alpha=0.7)
        plt.text(boundary, max(rf_sizes) * 0.8, f'Block {i+1}', rotation=90)
    
    # Compare different filter sizes
    plt.subplot(1, 2, 2)
    filter_sizes = [1, 3, 5, 7]
    equivalent_3x3 = [1, 1, 2, 3]  # Number of 3x3 layers needed
    
    bars = plt.bar(range(len(filter_sizes)), filter_sizes, 
                   color=['lightblue', 'orange', 'lightgreen', 'lightcoral'],
                   alpha=0.7)
    
    # Add text showing 3x3 equivalent
    for i, (bar, equiv) in enumerate(zip(bars, equivalent_3x3)):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height + 0.1,
                f'{equiv}× 3×3', ha='center', va='bottom')
    
    plt.title('Filter Size vs 3×3 Equivalent')
    plt.xlabel('Single Filter Size')
    plt.ylabel('Filter Size')
    plt.xticks(range(len(filter_sizes)), [f'{s}×{s}' for s in filter_sizes])
    
    plt.tight_layout()
    plt.show()

def visualize_feature_maps(model, test_loader, layer_indices=[2, 5, 10, 15]):
    """Visualize feature maps at different depths"""
    model.eval()
    device = next(model.parameters()).device
    
    # Get a test image
    data_iter = iter(test_loader)
    images, _ = next(data_iter)
    test_image = images[0:1].to(device)
    
    # Hook to capture feature maps
    feature_maps = {}
    hooks = []
    
    def get_activation(name):
        def hook(model, input, output):
            feature_maps[name] = output.detach()
        return hook
    
    # Register hooks at specified layers
    layer_count = 0
    for name, module in model.named_modules():
        if isinstance(module, nn.Conv2d):
            if layer_count in layer_indices:
                hook = module.register_forward_hook(get_activation(f'layer_{layer_count}'))
                hooks.append(hook)
            layer_count += 1
    
    # Forward pass
    with torch.no_grad():
        _ = model(test_image)
    
    # Plot feature maps
    fig, axes = plt.subplots(len(layer_indices), 8, figsize=(16, 2*len(layer_indices)))
    
    for i, layer_idx in enumerate(layer_indices):
        layer_name = f'layer_{layer_idx}'
        if layer_name in feature_maps:
            features = feature_maps[layer_name].cpu().numpy()[0]  # Remove batch dim
            
            # Show first 8 feature maps
            for j in range(min(8, features.shape[0])):
                row_idx = i if len(layer_indices) > 1 else 0
                axes[row_idx, j].imshow(features[j], cmap='viridis')
                axes[row_idx, j].set_title(f'L{layer_idx} F{j}')
                axes[row_idx, j].axis('off')
    
    plt.suptitle('VGG Feature Maps at Different Depths')
    plt.tight_layout()
    plt.show()
    
    # Remove hooks
    for hook in hooks:
        hook.remove()

def demonstrate_3x3_advantage():
    """Demonstrate why 3x3 filters are better than larger ones"""
    print("\n=== 3×3 Filter Advantage Analysis ===")
    
    # Parameter count comparison
    input_channels = 128
    output_channels = 128
    
    # Single large filter vs multiple small filters
    configs = {
        '7×7 Single': {'filters': [(7, 1)], 'params': input_channels * output_channels * 7 * 7},
        '5×5 Single': {'filters': [(5, 1)], 'params': input_channels * output_channels * 5 * 5},
        '3×3 Triple': {'filters': [(3, 3)], 'params': 3 * input_channels * output_channels * 3 * 3},
        '3×3 Double': {'filters': [(3, 2)], 'params': 2 * input_channels * output_channels * 3 * 3},
    }
    
    # Calculate effective receptive fields
    for name, config in configs.items():
        if '7×7' in name:
            rf = 7
        elif '5×5' in name:
            rf = 5
        elif 'Triple' in name:
            rf = 7  # 3 layers of 3×3 = 7×7 RF
        else:  # Double
            rf = 5  # 2 layers of 3×3 = 5×5 RF
        
        config['receptive_field'] = rf
    
    # Visualization
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(15, 5))
    
    # Parameter comparison
    names = list(configs.keys())
    params = [configs[name]['params'] for name in names]
    rfs = [configs[name]['receptive_field'] for name in names]
    
    bars1 = ax1.bar(names, params, color=['red', 'orange', 'green', 'blue'])
    ax1.set_title('Parameter Count Comparison')
    ax1.set_ylabel('Number of Parameters')
    ax1.tick_params(axis='x', rotation=45)
    
    # Add parameter values
    for bar, param in zip(bars1, params):
        height = bar.get_height()
        ax1.text(bar.get_x() + bar.get_width()/2., height + height*0.01,
                f'{param/1000:.1f}K', ha='center', va='bottom')
    
    # Receptive field comparison
    bars2 = ax2.bar(names, rfs, color=['red', 'orange', 'green', 'blue'])
    ax2.set_title('Effective Receptive Field')
    ax2.set_ylabel('Receptive Field Size')
    ax2.tick_params(axis='x', rotation=45)
    
    # Efficiency (RF per parameter)
    efficiency = [rf * 1e6 / param for rf, param in zip(rfs, params)]
    bars3 = ax3.bar(names, efficiency, color=['red', 'orange', 'green', 'blue'])
    ax3.set_title('Efficiency (RF per Million Parameters)')
    ax3.set_ylabel('Receptive Field per 1M Params')
    ax3.tick_params(axis='x', rotation=45)
    
    plt.tight_layout()
    plt.show()
    
    # Print advantages
    print("ADVANTAGES OF 3×3 FILTERS:")
    print("1. FEWER PARAMETERS:")
    print(f"   - 7×7 filter: {configs['7×7 Single']['params']:,} parameters")
    print(f"   - 3×3 triple stack: {configs['3×3 Triple']['params']:,} parameters")
    print(f"   - Reduction: {(1 - configs['3×3 Triple']['params']/configs['7×7 Single']['params'])*100:.1f}%")
    
    print("\n2. MORE NON-LINEARITY:")
    print("   - Each 3×3 layer adds ReLU activation")
    print("   - Triple stack = 3 ReLUs vs single 7×7 = 1 ReLU")
    print("   - More expressive power")
    
    print("\n3. SAME RECEPTIVE FIELD:")
    print("   - 3×3 × 3 layers = 7×7 effective receptive field")
    print("   - 3×3 × 2 layers = 5×5 effective receptive field")

# Example usage and demonstration
if __name__ == "__main__":
    print("=== VGG Networks Implementation and Analysis ===")
    
    # Show different VGG architectures
    print("\n=== VGG Architecture Variants ===")
    for name, config in vgg_configs.items():
        conv_count = len([x for x in config if x != 'M'])
        fc_count = 3  # Always 3 FC layers
        total_layers = conv_count + fc_count
        print(f"{name}: {conv_count} conv layers + {fc_count} FC layers = {total_layers} total")
        print(f"  Config: {config}")
    
    # Compare VGG variants
    comparison_results = compare_vgg_variants()
    
    # Analyze receptive field
    analyze_receptive_field()
    
    # Demonstrate 3x3 filter advantages
    demonstrate_3x3_advantage()
    
    # Visualize feature maps
    if comparison_results:
        best_model = comparison_results['VGG + BatchNorm']['model']
        # Create a simple test loader for visualization
        transform_test = transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010)),
        ])
        test_dataset = torchvision.datasets.CIFAR10(
            root='./data', train=False, download=True, transform=transform_test
        )
        test_loader = DataLoader(test_dataset, batch_size=1, shuffle=False)
        visualize_feature_maps(best_model, test_loader)
    
    # Plot final results
    if comparison_results:
        names = list(comparison_results.keys())
        accuracies = [comparison_results[name]['final_accuracy'] for name in names]
        params = [comparison_results[name]['parameters'] for name in names]
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
        
        # Accuracy comparison
        bars1 = ax1.bar(names, accuracies, color=['lightblue', 'lightgreen', 'lightcoral'])
        ax1.set_title('Final Test Accuracy')
        ax1.set_ylabel('Accuracy (%)')
        ax1.set_ylim(0, 100)
        
        for bar, acc in zip(bars1, accuracies):
            height = bar.get_height()
            ax1.text(bar.get_x() + bar.get_width()/2., height + 1,
                    f'{acc:.1f}%', ha='center', va='bottom')
        
        # Parameter comparison
        bars2 = ax2.bar(names, params, color=['lightblue', 'lightgreen', 'lightcoral'])
        ax2.set_title('Model Parameters')
        ax2.set_ylabel('Number of Parameters')
        ax2.set_yscale('log')
        
        for bar, param in zip(bars2, params):
            height = bar.get_height()
            ax2.text(bar.get_x() + bar.get_width()/2., height * 1.2,
                    f'{param/1e6:.1f}M', ha='center', va='bottom')
        
        plt.xticks(rotation=45)
        plt.tight_layout()
        plt.show()
    
    print("\n=== VGG Key Innovations ===")
    print("1. VERY SMALL FILTERS:")
    print("   - 3×3 convolution filters throughout")
    print("   - Smaller than AlexNet's 11×11 and 5×5")
    print("   - Stack multiple 3×3 for larger receptive fields")
    
    print("\n2. INCREASED DEPTH:")
    print("   - VGG-16: 16 layers (13 conv + 3 FC)")
    print("   - VGG-19: 19 layers (16 conv + 3 FC)")
    print("   - Demonstrated that depth improves performance")
    
    print("\n3. UNIFORM ARCHITECTURE:")
    print("   - Same 3×3 conv and 2×2 max-pool throughout")
    print("   - Simple and systematic design")
    print("   - Easy to understand and implement")
    
    print("\n4. DOUBLE FEATURE MAPS:")
    print("   - 64 → 128 → 256 → 512 → 512 channels")
    print("   - Double channels after each pooling")
    print("   - Maintain computational balance")
    
    print("\n=== VGG Impact ===")
    print("✓ Proved that depth matters more than filter size")
    print("✓ Established 3×3 as standard filter size")
    print("✓ Simple, systematic architecture design")
    print("✓ Strong ImageNet performance (92.7% top-5)")
    print("✓ Widely used for transfer learning")
    print("✓ Influenced all subsequent architectures")
    
    print("\n=== Architecture Details ===")
    print("INPUT: 224×224×3 RGB images")
    print("CONV LAYERS: 3×3 filters, stride 1, padding 1")
    print("POOLING: 2×2 max pooling, stride 2")
    print("ACTIVATION: ReLU throughout")
    print("FC LAYERS: 4096 → 4096 → 1000")
    print("DROPOUT: 0.5 in FC layers")
    
    print("\n=== Training Details ===")
    print("• SGD with momentum 0.9")
    print("• Weight decay 5×10⁻⁴")
    print("• Batch size 256")
    print("• Learning rate 0.01, reduced by 10 when plateaus")
    print("• Data augmentation: random crops, horizontal flips")
    
    print("\n=== VGG vs AlexNet ===")
    print("IMPROVEMENTS OVER ALEXNET:")
    print("✓ Smaller filters (3×3 vs 11×11/5×5)")
    print("✓ Deeper networks (16-19 vs 8 layers)")
    print("✓ More uniform architecture")
    print("✓ Better ImageNet performance")
    
    print("\nKEPT FROM ALEXNET:")
    print("• ReLU activation")
    print("• Dropout regularization")
    print("• Data augmentation")
    print("• SGD with momentum")
    
    print("\n=== Computational Analysis ===")
    print("VGG-16 FLOPS (per forward pass):")
    print("• Total: ~15.5 billion FLOPs")
    print("• Conv layers: ~15.3 billion (98.7%)")
    print("• FC layers: ~0.2 billion (1.3%)")
    print("• Memory: ~528 MB (feature maps)")
    
    print("\n=== VGG Limitations ===")
    print("❌ VERY LARGE MODEL: 138M parameters")
    print("❌ HIGH MEMORY USAGE: >500MB feature maps")
    print("❌ SLOW INFERENCE: Many operations")
    print("❌ OVERFITTING: Large FC layers")
    
    print("\n=== Modern Perspective ===")
    print("OUTDATED COMPONENTS:")
    print("❌ Large FC layers → Global Average Pooling")
    print("❌ No batch normalization")
    print("❌ No skip connections")
    
    print("\nTIMELESS CONTRIBUTIONS:")
    print("✓ 3×3 filter standardization")
    print("✓ Importance of depth")
    print("✓ Systematic architecture design")
    print("✓ Transfer learning backbone")