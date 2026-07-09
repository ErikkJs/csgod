# author: csgod cmps studies
# GoogLeNet/Inception - Going Deeper with Convolutions
# GoogLeNet introduced the Inception module for multi-scale feature extraction
# and went 22 layers deep while being more efficient than VGG.

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

class InceptionModule(nn.Module):
    """Basic Inception module (Inception v1)"""
    def __init__(self, in_channels, ch1x1, ch3x3red, ch3x3, ch5x5red, ch5x5, pool_proj):
        super(InceptionModule, self).__init__()
        
        # 1x1 convolution branch
        self.branch1 = nn.Conv2d(in_channels, ch1x1, kernel_size=1)
        
        # 1x1 -> 3x3 convolution branch
        self.branch2 = nn.Sequential(
            nn.Conv2d(in_channels, ch3x3red, kernel_size=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(ch3x3red, ch3x3, kernel_size=3, padding=1),
        )
        
        # 1x1 -> 5x5 convolution branch  
        self.branch3 = nn.Sequential(
            nn.Conv2d(in_channels, ch5x5red, kernel_size=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(ch5x5red, ch5x5, kernel_size=5, padding=2),
        )
        
        # 3x3 max pooling -> 1x1 convolution branch
        self.branch4 = nn.Sequential(
            nn.MaxPool2d(kernel_size=3, stride=1, padding=1),
            nn.Conv2d(in_channels, pool_proj, kernel_size=1),
        )
        
        # ReLU activations
        self.relu = nn.ReLU(inplace=True)
    
    def forward(self, x):
        branch1 = self.relu(self.branch1(x))
        branch2 = self.relu(self.branch2(x))
        branch3 = self.relu(self.branch3(x))
        branch4 = self.relu(self.branch4(x))
        
        # Concatenate along channel dimension
        outputs = torch.cat([branch1, branch2, branch3, branch4], dim=1)
        return outputs

class AuxiliaryClassifier(nn.Module):
    """Auxiliary classifier for intermediate supervision"""
    def __init__(self, in_channels, num_classes=1000, dropout=0.7):
        super(AuxiliaryClassifier, self).__init__()
        
        self.classifier = nn.Sequential(
            nn.AvgPool2d(kernel_size=5, stride=3),
            nn.Conv2d(in_channels, 128, kernel_size=1),
            nn.ReLU(inplace=True),
            nn.Flatten(),
            nn.Linear(128 * 4 * 4, 1024),  # Assuming input size leads to 4x4
            nn.ReLU(inplace=True),
            nn.Dropout(p=dropout),
            nn.Linear(1024, num_classes),
        )
    
    def forward(self, x):
        return self.classifier(x)

class GoogLeNet(nn.Module):
    """GoogLeNet/Inception v1 implementation"""
    def __init__(self, num_classes=1000, aux_logits=True, dropout=0.2):
        super(GoogLeNet, self).__init__()
        
        self.aux_logits = aux_logits
        
        # Initial convolution layers
        self.conv1 = nn.Conv2d(3, 64, kernel_size=7, stride=2, padding=3)
        self.maxpool1 = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        self.conv2 = nn.Conv2d(64, 64, kernel_size=1)
        self.conv3 = nn.Conv2d(64, 192, kernel_size=3, padding=1)
        self.maxpool2 = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        
        # Inception modules
        self.inception3a = InceptionModule(192, 64, 96, 128, 16, 32, 32)
        self.inception3b = InceptionModule(256, 128, 128, 192, 32, 96, 64)
        self.maxpool3 = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        
        self.inception4a = InceptionModule(480, 192, 96, 208, 16, 48, 64)
        self.inception4b = InceptionModule(512, 160, 112, 224, 24, 64, 64)
        self.inception4c = InceptionModule(512, 128, 128, 256, 24, 64, 64)
        self.inception4d = InceptionModule(512, 112, 144, 288, 32, 64, 64)
        self.inception4e = InceptionModule(528, 256, 160, 320, 32, 128, 128)
        self.maxpool4 = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        
        self.inception5a = InceptionModule(832, 256, 160, 320, 32, 128, 128)
        self.inception5b = InceptionModule(832, 384, 192, 384, 48, 128, 128)
        
        # Auxiliary classifiers (for training)
        if aux_logits:
            self.aux1 = AuxiliaryClassifier(512, num_classes)
            self.aux2 = AuxiliaryClassifier(528, num_classes)
        
        # Final classifier
        self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
        self.dropout = nn.Dropout(p=dropout)
        self.fc = nn.Linear(1024, num_classes)
        
        # Initialize weights
        self._initialize_weights()
    
    def forward(self, x):
        # Initial convolution layers
        x = F.relu(self.conv1(x))
        x = self.maxpool1(x)
        x = F.relu(self.conv2(x))
        x = F.relu(self.conv3(x))
        x = self.maxpool2(x)
        
        # Inception 3
        x = self.inception3a(x)
        x = self.inception3b(x)
        x = self.maxpool3(x)
        
        # Inception 4
        x = self.inception4a(x)
        
        # Auxiliary classifier 1
        if self.aux_logits and self.training:
            aux1 = self.aux1(x)
        
        x = self.inception4b(x)
        x = self.inception4c(x)
        x = self.inception4d(x)
        
        # Auxiliary classifier 2
        if self.aux_logits and self.training:
            aux2 = self.aux2(x)
        
        x = self.inception4e(x)
        x = self.maxpool4(x)
        
        # Inception 5
        x = self.inception5a(x)
        x = self.inception5b(x)
        
        # Final classifier
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.dropout(x)
        x = self.fc(x)
        
        if self.aux_logits and self.training:
            return x, aux1, aux2
        else:
            return x
    
    def _initialize_weights(self):
        for m in self.modules():
            if isinstance(m, nn.Conv2d) or isinstance(m, nn.Linear):
                nn.init.trunc_normal_(m.weight, mean=0.0, std=0.01)
                if m.bias is not None:
                    nn.init.constant_(m.bias, 0)

class SimplifiedInception(nn.Module):
    """Simplified Inception for smaller datasets like CIFAR-10"""
    def __init__(self, num_classes=10, dropout=0.4):
        super(SimplifiedInception, self).__init__()
        
        # Initial layers
        self.conv1 = nn.Sequential(
            nn.Conv2d(3, 64, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=2, stride=2),
        )
        
        # Simplified Inception modules
        self.inception1 = InceptionModule(64, 16, 16, 32, 4, 8, 8)   # 64 channels out
        self.inception2 = InceptionModule(64, 32, 32, 64, 8, 16, 16)  # 128 channels out
        self.maxpool1 = nn.MaxPool2d(kernel_size=2, stride=2)
        
        self.inception3 = InceptionModule(128, 64, 64, 128, 16, 32, 32)  # 256 channels out
        self.inception4 = InceptionModule(256, 128, 128, 256, 32, 64, 64)  # 512 channels out
        self.maxpool2 = nn.MaxPool2d(kernel_size=2, stride=2)
        
        # Final layers
        self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
        self.dropout = nn.Dropout(p=dropout)
        self.fc = nn.Linear(512, num_classes)
    
    def forward(self, x):
        x = self.conv1(x)
        x = self.inception1(x)
        x = self.inception2(x)
        x = self.maxpool1(x)
        x = self.inception3(x)
        x = self.inception4(x)
        x = self.maxpool2(x)
        
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.dropout(x)
        x = self.fc(x)
        
        return x

class InceptionV2Module(nn.Module):
    """Inception v2 module with factorized convolutions"""
    def __init__(self, in_channels, ch1x1, ch3x3red, ch3x3, ch5x5red, ch5x5, pool_proj):
        super(InceptionV2Module, self).__init__()
        
        # 1x1 branch
        self.branch1 = nn.Conv2d(in_channels, ch1x1, kernel_size=1)
        
        # 1x1 -> 3x3 branch
        self.branch2 = nn.Sequential(
            nn.Conv2d(in_channels, ch3x3red, kernel_size=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(ch3x3red, ch3x3, kernel_size=3, padding=1),
        )
        
        # 1x1 -> 3x3 -> 3x3 branch (factorized 5x5)
        self.branch3 = nn.Sequential(
            nn.Conv2d(in_channels, ch5x5red, kernel_size=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(ch5x5red, ch5x5, kernel_size=3, padding=1),
            nn.ReLU(inplace=True),
            nn.Conv2d(ch5x5, ch5x5, kernel_size=3, padding=1),
        )
        
        # Pool -> 1x1 branch
        self.branch4 = nn.Sequential(
            nn.MaxPool2d(kernel_size=3, stride=1, padding=1),
            nn.Conv2d(in_channels, pool_proj, kernel_size=1),
        )
        
        self.relu = nn.ReLU(inplace=True)
    
    def forward(self, x):
        branch1 = self.relu(self.branch1(x))
        branch2 = self.relu(self.branch2(x))
        branch3 = self.relu(self.branch3(x))
        branch4 = self.relu(self.branch4(x))
        
        return torch.cat([branch1, branch2, branch3, branch4], dim=1)

def train_inception(model, train_loader, test_loader, num_epochs=20, learning_rate=0.01, device=None):
    """Train Inception model with auxiliary losses"""
    if device is None:
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    model = model.to(device)
    
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9, weight_decay=4e-5)
    scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)
    
    train_losses = []
    train_accuracies = []
    test_losses = []
    test_accuracies = []
    
    print(f"Training on {device}")
    print(f"Model parameters: {sum(p.numel() for p in model.parameters() if p.requires_grad):,}")
    
    for epoch in range(num_epochs):
        # Training phase
        model.train()
        running_loss = 0.0
        correct = 0
        total = 0
        
        for batch_idx, (data, targets) in enumerate(train_loader):
            data, targets = data.to(device), targets.to(device)
            
            optimizer.zero_grad()
            
            # Forward pass
            if hasattr(model, 'aux_logits') and model.aux_logits and model.training:
                outputs, aux1, aux2 = model(data)
                
                # Main loss
                loss1 = criterion(outputs, targets)
                
                # Auxiliary losses (with reduced weight)
                loss2 = criterion(aux1, targets)
                loss3 = criterion(aux2, targets)
                
                # Total loss (auxiliary losses weighted at 0.3)
                loss = loss1 + 0.3 * loss2 + 0.3 * loss3
            else:
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
        
        print(f'Epoch [{epoch+1}/{num_epochs}] - Train Loss: {train_loss:.4f}, Train Acc: {train_acc:.2f}%, '
              f'Test Loss: {test_loss:.4f}, Test Acc: {test_acc:.2f}%')
    
    return train_losses, train_accuracies, test_losses, test_accuracies

def visualize_inception_module():
    """Visualize the Inception module architecture"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 8))
    
    # Original Inception module
    ax1.set_title('Inception Module v1', fontsize=16, fontweight='bold')
    ax1.set_xlim(0, 10)
    ax1.set_ylim(0, 10)
    
    # Input
    input_box = plt.Rectangle((4, 0.5), 2, 1, facecolor='lightblue', edgecolor='black')
    ax1.add_patch(input_box)
    ax1.text(5, 1, 'Input\nFeature Maps', ha='center', va='center', fontweight='bold')
    
    # Branches
    branches = [
        ('1×1 conv', (1, 4), 'lightgreen'),
        ('1×1→3×3 conv', (3.5, 4), 'lightcoral'),
        ('1×1→5×5 conv', (6, 4), 'lightyellow'),
        ('MaxPool→1×1', (8.5, 4), 'lightpink')
    ]
    
    for i, (label, pos, color) in enumerate(branches):
        # Branch box
        branch_box = plt.Rectangle((pos[0]-0.5, pos[1]), 1, 1.5, facecolor=color, edgecolor='black')
        ax1.add_patch(branch_box)
        ax1.text(pos[0], pos[1]+0.75, label, ha='center', va='center', fontsize=9, fontweight='bold')
        
        # Arrow from input to branch
        ax1.arrow(5, 1.5, pos[0]-5, pos[1]-1.5-0.2, head_width=0.1, head_length=0.1, 
                 fc='black', ec='black')
        
        # Arrow from branch to concat
        ax1.arrow(pos[0], pos[1]+1.5, 5-pos[0], 7.5-(pos[1]+1.5)-0.2, head_width=0.1, 
                 head_length=0.1, fc='black', ec='black')
    
    # Concatenation
    concat_box = plt.Rectangle((4, 7.5), 2, 1, facecolor='orange', edgecolor='black')
    ax1.add_patch(concat_box)
    ax1.text(5, 8, 'Filter\nConcatenation', ha='center', va='center', fontweight='bold')
    
    ax1.axis('off')
    
    # Multi-scale feature visualization
    ax2.set_title('Multi-Scale Feature Extraction', fontsize=16, fontweight='bold')
    
    # Simulate different receptive fields
    x = np.linspace(-2, 2, 100)
    y1 = np.exp(-(x**2)/0.1)  # Small receptive field (1x1)
    y2 = np.exp(-(x**2)/0.5)  # Medium receptive field (3x3)
    y3 = np.exp(-(x**2)/1.0)  # Large receptive field (5x5)
    y4 = np.exp(-(x**2)/0.3) * 0.5  # Pooling
    
    ax2.plot(x, y1, label='1×1 conv (fine details)', linewidth=3, color='green')
    ax2.plot(x, y2, label='3×3 conv (local features)', linewidth=3, color='red')
    ax2.plot(x, y3, label='5×5 conv (global context)', linewidth=3, color='blue')
    ax2.plot(x, y4, label='MaxPool (invariance)', linewidth=3, color='purple', linestyle='--')
    
    ax2.set_xlabel('Spatial Location')
    ax2.set_ylabel('Feature Response')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    ax2.text(0, 0.8, 'Different scales capture\ndifferent types of features', 
             ha='center', va='center', bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow"))
    
    plt.tight_layout()
    plt.show()

def analyze_parameter_efficiency():
    """Analyze parameter efficiency of Inception vs traditional CNNs"""
    print("\n=== Parameter Efficiency Analysis ===")
    
    # Compare parameter counts for same receptive field
    input_channels = 256
    output_channels = 256
    
    configurations = {
        'Traditional 5×5': {
            'layers': [('5×5 conv', input_channels * output_channels * 5 * 5)],
            'params': input_channels * output_channels * 5 * 5,
            'description': 'Single 5×5 convolution'
        },
        'Inception 5×5 branch': {
            'layers': [
                ('1×1 reduce', input_channels * 32 * 1 * 1),
                ('5×5 conv', 32 * 64 * 5 * 5)
            ],
            'params': input_channels * 32 * 1 * 1 + 32 * 64 * 5 * 5,
            'description': '1×1 reduction + 5×5 conv'
        },
        'Full Inception': {
            'layers': [
                ('1×1 branch', input_channels * 64 * 1 * 1),
                ('3×3 branch', input_channels * 96 * 1 * 1 + 96 * 128 * 3 * 3),
                ('5×5 branch', input_channels * 16 * 1 * 1 + 16 * 32 * 5 * 5),
                ('Pool branch', input_channels * 32 * 1 * 1)
            ],
            'params': (input_channels * 64 * 1 * 1 + 
                      input_channels * 96 * 1 * 1 + 96 * 128 * 3 * 3 + 
                      input_channels * 16 * 1 * 1 + 16 * 32 * 5 * 5 + 
                      input_channels * 32 * 1 * 1),
            'description': 'Full Inception module'
        }
    }
    
    # Calculate and display results
    names = list(configurations.keys())
    params = [configurations[name]['params'] for name in names]
    
    plt.figure(figsize=(12, 8))
    
    # Parameter comparison
    plt.subplot(2, 2, 1)
    bars = plt.bar(names, params, color=['red', 'orange', 'green'])
    plt.title('Parameter Count Comparison')
    plt.ylabel('Number of Parameters')
    plt.xticks(rotation=45)
    
    for bar, param in zip(bars, params):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height + height*0.01,
                f'{param/1000:.0f}K', ha='center', va='bottom')
    
    # Efficiency (output channels per parameter)
    plt.subplot(2, 2, 2)
    output_channels_config = [256, 64, 256]  # Approximate output channels
    efficiency = [out_ch / param * 1000 for out_ch, param in zip(output_channels_config, params)]
    
    bars = plt.bar(names, efficiency, color=['red', 'orange', 'green'])
    plt.title('Efficiency (Output Channels per 1K Parameters)')
    plt.ylabel('Channels per 1K Parameters')
    plt.xticks(rotation=45)
    
    # Network depth comparison
    plt.subplot(2, 2, 3)
    architectures = ['AlexNet\n(2012)', 'VGG-16\n(2014)', 'GoogLeNet\n(2014)', 'ResNet-50\n(2015)']
    depths = [8, 16, 22, 50]
    parameters = [60, 138, 7, 25.6]  # Millions of parameters
    
    ax1 = plt.gca()
    bars1 = ax1.bar(architectures, depths, color=['lightblue', 'lightgreen', 'orange', 'lightcoral'])
    ax1.set_ylabel('Number of Layers', color='blue')
    ax1.tick_params(axis='y', labelcolor='blue')
    
    ax2 = ax1.twinx()
    line = ax2.plot(architectures, parameters, 'ro-', linewidth=2, markersize=8)
    ax2.set_ylabel('Parameters (Millions)', color='red')
    ax2.tick_params(axis='y', labelcolor='red')
    
    plt.title('Architecture Comparison: Depth vs Parameters')
    
    # Computational efficiency
    plt.subplot(2, 2, 4)
    flops = [724, 15300, 1560, 4100]  # Million FLOPs (approximate)
    accuracy = [60.9, 71.5, 74.8, 76.0]  # ImageNet top-1 accuracy (approximate)
    
    plt.scatter(flops, accuracy, s=100, c=['lightblue', 'lightgreen', 'orange', 'lightcoral'])
    for i, arch in enumerate(['AlexNet', 'VGG-16', 'GoogLeNet', 'ResNet-50']):
        plt.annotate(arch, (flops[i], accuracy[i]), xytext=(5, 5), 
                    textcoords='offset points')
    
    plt.xlabel('Computational Cost (Million FLOPs)')
    plt.ylabel('ImageNet Top-1 Accuracy (%)')
    plt.title('Efficiency: Accuracy vs Computation')
    plt.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.show()
    
    # Print numerical results
    print(f"Parameter reduction with Inception:")
    traditional = configurations['Traditional 5×5']['params']
    inception = configurations['Inception 5×5 branch']['params']
    reduction = (1 - inception/traditional) * 100
    print(f"  Traditional 5×5: {traditional:,} parameters")
    print(f"  Inception 5×5 branch: {inception:,} parameters")
    print(f"  Reduction: {reduction:.1f}%")

def compare_inception_variants():
    """Compare different versions of Inception"""
    print("=== Inception Variants Comparison ===")
    
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
        'Simplified Inception': SimplifiedInception(num_classes=10),
    }
    
    results = {}
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    for name, model in models.items():
        print(f"\n{'='*50}")
        print(f"Training {name}")
        print('='*50)
        
        total_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
        print(f"Parameters: {total_params:,}")
        
        # Train model
        train_losses, train_accs, test_losses, test_accs = train_inception(
            model, train_loader, test_loader, num_epochs=15, device=device
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

# Example usage and demonstration
if __name__ == "__main__":
    print("=== GoogLeNet/Inception Implementation and Analysis ===")
    
    # Visualize Inception architecture
    visualize_inception_module()
    
    # Analyze parameter efficiency
    analyze_parameter_efficiency()
    
    # Compare variants
    comparison_results = compare_inception_variants()
    
    # Plot training results
    if comparison_results:
        fig, axes = plt.subplots(2, 2, figsize=(12, 10))
        
        for name, data in comparison_results.items():
            axes[0, 0].plot(data['train_losses'], label=f'{name} (Train)')
            axes[0, 0].plot(data['test_losses'], label=f'{name} (Test)', linestyle='--')
        axes[0, 0].set_title('Loss Curves')
        axes[0, 0].set_xlabel('Epoch')
        axes[0, 0].set_ylabel('Loss')
        axes[0, 0].legend()
        axes[0, 0].grid(True)
        
        for name, data in comparison_results.items():
            axes[0, 1].plot(data['train_accuracies'], label=f'{name} (Train)')
            axes[0, 1].plot(data['test_accuracies'], label=f'{name} (Test)', linestyle='--')
        axes[0, 1].set_title('Accuracy Curves')
        axes[0, 1].set_xlabel('Epoch')
        axes[0, 1].set_ylabel('Accuracy (%)')
        axes[0, 1].legend()
        axes[0, 1].grid(True)
        
        names = list(comparison_results.keys())
        accuracies = [comparison_results[name]['final_accuracy'] for name in names]
        params = [comparison_results[name]['parameters'] for name in names]
        
        bars = axes[1, 0].bar(names, accuracies)
        axes[1, 0].set_title('Final Test Accuracy')
        axes[1, 0].set_ylabel('Accuracy (%)')
        for bar, acc in zip(bars, accuracies):
            height = bar.get_height()
            axes[1, 0].text(bar.get_x() + bar.get_width()/2., height + 1,
                           f'{acc:.1f}%', ha='center', va='bottom')
        
        bars = axes[1, 1].bar(names, params)
        axes[1, 1].set_title('Model Parameters')
        axes[1, 1].set_ylabel('Number of Parameters')
        for bar, param in zip(bars, params):
            height = bar.get_height()
            axes[1, 1].text(bar.get_x() + bar.get_width()/2., height + height*0.05,
                           f'{param/1e6:.1f}M', ha='center', va='bottom')
        
        plt.tight_layout()
        plt.show()
    
    print("\n=== GoogLeNet Key Innovations ===")
    print("1. INCEPTION MODULE:")
    print("   - Multi-scale convolutions in parallel")
    print("   - 1×1, 3×3, 5×5 convolutions + max pooling")
    print("   - Captures features at different scales")
    print("   - Filter concatenation instead of addition")
    
    print("\n2. 1×1 CONVOLUTIONS FOR DIMENSIONALITY REDUCTION:")
    print("   - Reduce input channels before expensive operations")
    print("   - bottleneck design: 1×1 → 3×3/5×5 → output")
    print("   - Dramatically reduces parameter count")
    print("   - Introduces additional non-linearity")
    
    print("\n3. AUXILIARY CLASSIFIERS:")
    print("   - Additional losses at intermediate layers")
    print("   - Combat vanishing gradient problem")
    print("   - Provide regularization effect")
    print("   - Weighted at 0.3 in total loss")
    
    print("\n4. GLOBAL AVERAGE POOLING:")
    print("   - Replace large fully connected layers")
    print("   - Reduce overfitting and parameters")
    print("   - More robust to spatial translations")
    
    print("\n=== Architecture Innovations ===")
    print("• 22 layers deep (including pooling)")
    print("• Only 7 million parameters (vs VGG's 138M)")
    print("• Network In Network inspiration")
    print("• Efficient use of computational budget")
    print("• Multi-scale feature extraction")
    
    print("\n=== Training Innovations ===")
    print("• Auxiliary losses for gradient flow")
    print("• Aggressive data augmentation")
    print("• Different learning rates for different layers")
    print("• Careful weight initialization")
    
    print("\n=== Performance Impact ===")
    print("✓ Won ImageNet 2014 with 6.67% top-5 error")
    print("✓ 12× fewer parameters than AlexNet")
    print("✓ Better accuracy than VGG-16")
    print("✓ More efficient computation")
    print("✓ Introduced multi-scale thinking")
    
    print("\n=== Inception Evolution ===")
    print("INCEPTION V1 (GoogLeNet, 2014):")
    print("• Original inception module")
    print("• Auxiliary classifiers")
    print("• Global average pooling")
    
    print("\nINCEPTION V2 (2015):")
    print("• Factorized convolutions (5×5 → two 3×3)")
    print("• Batch normalization")
    print("• Reduced representational bottlenecks")
    
    print("\nINCEPTION V3 (2015):")
    print("• Factorized 3×3 → 1×3 + 3×1")
    print("• Wider inception modules")
    print("• RMSProp optimizer")
    
    print("\nINCEPTION V4 & INCEPTION-RESNET (2016):")
    print("• Residual connections")
    print("• Simplified architecture")
    print("• Better training dynamics")
    
    print("\n=== Key Architectural Principles ===")
    print("1. MULTI-SCALE FEATURE EXTRACTION:")
    print("   - Different filter sizes capture different patterns")
    print("   - Small filters: fine details")
    print("   - Large filters: global context")
    
    print("\n2. COMPUTATIONAL EFFICIENCY:")
    print("   - 1×1 convolutions reduce dimensions")
    print("   - Factorize large convolutions")
    print("   - Balance width and depth")
    
    print("\n3. AUXILIARY SUPERVISION:")
    print("   - Additional gradient signal")
    print("   - Regularization through intermediate losses")
    print("   - Better gradient flow")
    
    print("\n=== Modern Relevance ===")
    print("STILL RELEVANT:")
    print("✓ Multi-scale feature extraction")
    print("✓ 1×1 convolutions for efficiency")
    print("✓ Network-in-Network principles")
    print("✓ Computational efficiency focus")
    
    print("\nSUPERSEDED BY:")
    print("• ResNet: residual connections")
    print("• DenseNet: dense connections")
    print("• EfficientNet: compound scaling")
    print("• Vision Transformers: attention mechanisms")
    
    print("\n=== Computational Analysis ===")
    print("GOOGLENET EFFICIENCY:")
    print("• Parameters: 7M (vs VGG-16: 138M)")
    print("• FLOPs: 1.6B (vs VGG-16: 15.3B)")
    print("• Memory: Much lower activation memory")
    print("• Speed: Faster inference")
    
    print("\n=== Legacy and Impact ===")
    print("• Proved efficiency can beat brute force")
    print("• Introduced multi-path architectures")
    print("• Inspired attention mechanisms")
    print("• Showed importance of architectural design")
    print("• Foundation for modern efficient networks")