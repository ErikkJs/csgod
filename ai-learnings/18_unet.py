# author: csgod cmps studies
# U-Net - Encoder-decoder for segmentation
# U-Net is a convolutional network architecture designed for biomedical image segmentation,
# featuring skip connections between encoder and decoder for precise localization.

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.utils.data import DataLoader, Dataset
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np
from PIL import Image
import cv2

class DoubleConv(nn.Module):
    """Double convolution block used in U-Net"""
    def __init__(self, in_channels, out_channels):
        super(DoubleConv, self).__init__()
        self.double_conv = nn.Sequential(
            nn.Conv2d(in_channels, out_channels, kernel_size=3, padding=1),
            nn.BatchNorm2d(out_channels),
            nn.ReLU(inplace=True),
            nn.Conv2d(out_channels, out_channels, kernel_size=3, padding=1),
            nn.BatchNorm2d(out_channels),
            nn.ReLU(inplace=True)
        )
    
    def forward(self, x):
        return self.double_conv(x)

class DownBlock(nn.Module):
    """Downsampling block with max pooling"""
    def __init__(self, in_channels, out_channels):
        super(DownBlock, self).__init__()
        self.maxpool_conv = nn.Sequential(
            nn.MaxPool2d(2),
            DoubleConv(in_channels, out_channels)
        )
    
    def forward(self, x):
        return self.maxpool_conv(x)

class UpBlock(nn.Module):
    """Upsampling block with concatenation"""
    def __init__(self, in_channels, out_channels, bilinear=True):
        super(UpBlock, self).__init__()
        
        if bilinear:
            self.up = nn.Upsample(scale_factor=2, mode='bilinear', align_corners=True)
            self.conv = DoubleConv(in_channels, out_channels)
        else:
            self.up = nn.ConvTranspose2d(in_channels, in_channels // 2, kernel_size=2, stride=2)
            self.conv = DoubleConv(in_channels, out_channels)
    
    def forward(self, x1, x2):
        x1 = self.up(x1)
        
        # Handle size differences between x1 and x2
        diff_y = x2.size()[2] - x1.size()[2]
        diff_x = x2.size()[3] - x1.size()[3]
        
        x1 = F.pad(x1, [diff_x // 2, diff_x - diff_x // 2,
                       diff_y // 2, diff_y - diff_y // 2])
        
        # Concatenate along channel dimension
        x = torch.cat([x2, x1], dim=1)
        return self.conv(x)

class UNet(nn.Module):
    """U-Net architecture for semantic segmentation"""
    def __init__(self, n_channels=3, n_classes=1, bilinear=True):
        super(UNet, self).__init__()
        self.n_channels = n_channels
        self.n_classes = n_classes
        self.bilinear = bilinear
        
        # Encoder (Contracting path)
        self.inc = DoubleConv(n_channels, 64)
        self.down1 = DownBlock(64, 128)
        self.down2 = DownBlock(128, 256)
        self.down3 = DownBlock(256, 512)
        factor = 2 if bilinear else 1
        self.down4 = DownBlock(512, 1024 // factor)
        
        # Decoder (Expansive path)
        self.up1 = UpBlock(1024, 512 // factor, bilinear)
        self.up2 = UpBlock(512, 256 // factor, bilinear)
        self.up3 = UpBlock(256, 128 // factor, bilinear)
        self.up4 = UpBlock(128, 64, bilinear)
        
        # Output layer
        self.outc = nn.Conv2d(64, n_classes, kernel_size=1)
    
    def forward(self, x):
        # Encoder
        x1 = self.inc(x)
        x2 = self.down1(x1)
        x3 = self.down2(x2)
        x4 = self.down3(x3)
        x5 = self.down4(x4)
        
        # Decoder with skip connections
        x = self.up1(x5, x4)
        x = self.up2(x, x3)
        x = self.up3(x, x2)
        x = self.up4(x, x1)
        
        # Output
        logits = self.outc(x)
        return logits

class SimpleSegmentationDataset(Dataset):
    """Simple dataset for demonstrating segmentation"""
    def __init__(self, num_samples=1000, image_size=256):
        self.num_samples = num_samples
        self.image_size = image_size
    
    def __len__(self):
        return self.num_samples
    
    def __getitem__(self, idx):
        # Generate synthetic image with simple shapes
        image, mask = self.generate_synthetic_data()
        
        # Convert to tensors
        image_tensor = torch.FloatTensor(image).permute(2, 0, 1) / 255.0  # CHW format
        mask_tensor = torch.FloatTensor(mask).unsqueeze(0)  # Add channel dimension
        
        return image_tensor, mask_tensor
    
    def generate_synthetic_data(self):
        """Generate synthetic segmentation data"""
        # Create blank image
        image = np.zeros((self.image_size, self.image_size, 3), dtype=np.uint8)
        mask = np.zeros((self.image_size, self.image_size), dtype=np.float32)
        
        # Add random shapes
        num_shapes = np.random.randint(3, 8)
        
        for i in range(num_shapes):
            shape_type = np.random.choice(['circle', 'rectangle', 'triangle'])
            color = np.random.randint(50, 255, 3).tolist()
            
            if shape_type == 'circle':
                center = (np.random.randint(50, self.image_size-50), 
                         np.random.randint(50, self.image_size-50))
                radius = np.random.randint(20, 60)
                cv2.circle(image, center, radius, color, -1)
                cv2.circle(mask, center, radius, 1.0, -1)
            
            elif shape_type == 'rectangle':
                pt1 = (np.random.randint(0, self.image_size-100), 
                       np.random.randint(0, self.image_size-100))
                pt2 = (pt1[0] + np.random.randint(30, 100), 
                       pt1[1] + np.random.randint(30, 100))
                cv2.rectangle(image, pt1, pt2, color, -1)
                cv2.rectangle(mask, pt1, pt2, 1.0, -1)
            
            elif shape_type == 'triangle':
                pts = np.array([
                    [np.random.randint(50, self.image_size-50), np.random.randint(50, self.image_size-50)],
                    [np.random.randint(50, self.image_size-50), np.random.randint(50, self.image_size-50)],
                    [np.random.randint(50, self.image_size-50), np.random.randint(50, self.image_size-50)]
                ], np.int32)
                cv2.fillPoly(image, [pts], color)
                cv2.fillPoly(mask, [pts], 1.0)
        
        # Add some noise
        noise = np.random.normal(0, 25, image.shape).astype(np.int16)
        image = np.clip(image.astype(np.int16) + noise, 0, 255).astype(np.uint8)
        
        return image, mask

class DiceLoss(nn.Module):
    """Dice Loss for segmentation tasks"""
    def __init__(self, smooth=1e-6):
        super(DiceLoss, self).__init__()
        self.smooth = smooth
    
    def forward(self, predictions, targets):
        # Flatten tensors
        predictions = predictions.view(-1)
        targets = targets.view(-1)
        
        # Calculate Dice coefficient
        intersection = (predictions * targets).sum()
        dice = (2. * intersection + self.smooth) / (predictions.sum() + targets.sum() + self.smooth)
        
        # Return Dice loss
        return 1 - dice

class CombinedLoss(nn.Module):
    """Combined BCE and Dice loss"""
    def __init__(self, bce_weight=0.5, dice_weight=0.5):
        super(CombinedLoss, self).__init__()
        self.bce_weight = bce_weight
        self.dice_weight = dice_weight
        self.bce_loss = nn.BCEWithLogitsLoss()
        self.dice_loss = DiceLoss()
    
    def forward(self, predictions, targets):
        bce = self.bce_loss(predictions, targets)
        
        # Apply sigmoid for Dice loss calculation
        predictions_sigmoid = torch.sigmoid(predictions)
        dice = self.dice_loss(predictions_sigmoid, targets)
        
        return self.bce_weight * bce + self.dice_weight * dice

def train_unet(model, train_loader, val_loader, num_epochs=50, learning_rate=1e-4):
    """Train U-Net model"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    
    # Loss and optimizer
    criterion = CombinedLoss()
    optimizer = optim.Adam(model.parameters(), lr=learning_rate, weight_decay=1e-5)
    scheduler = optim.lr_scheduler.ReduceLROnPlateau(optimizer, mode='min', patience=5)
    
    train_losses = []
    val_losses = []
    dice_scores = []
    
    print(f"Training U-Net on {device}")
    print(f"Model parameters: {sum(p.numel() for p in model.parameters() if p.requires_grad):,}")
    
    for epoch in range(num_epochs):
        # Training phase
        model.train()
        running_loss = 0.0
        
        for batch_idx, (images, masks) in enumerate(train_loader):
            images, masks = images.to(device), masks.to(device)
            
            optimizer.zero_grad()
            outputs = model(images)
            loss = criterion(outputs, masks)
            loss.backward()
            optimizer.step()
            
            running_loss += loss.item()
            
            if batch_idx % 50 == 0:
                print(f'Epoch [{epoch+1}/{num_epochs}], Batch [{batch_idx}/{len(train_loader)}], '
                      f'Loss: {loss.item():.4f}')
        
        train_loss = running_loss / len(train_loader)
        train_losses.append(train_loss)
        
        # Validation phase
        model.eval()
        val_loss = 0.0
        dice_score = 0.0
        
        with torch.no_grad():
            for images, masks in val_loader:
                images, masks = images.to(device), masks.to(device)
                outputs = model(images)
                
                loss = criterion(outputs, masks)
                val_loss += loss.item()
                
                # Calculate Dice score
                predictions = torch.sigmoid(outputs) > 0.5
                intersection = (predictions * masks).sum().float()
                dice = (2. * intersection) / (predictions.sum() + masks.sum() + 1e-6)
                dice_score += dice.item()
        
        val_loss /= len(val_loader)
        dice_score /= len(val_loader)
        
        val_losses.append(val_loss)
        dice_scores.append(dice_score)
        
        scheduler.step(val_loss)
        
        print(f'Epoch [{epoch+1}/{num_epochs}] - Train Loss: {train_loss:.4f}, '
              f'Val Loss: {val_loss:.4f}, Dice Score: {dice_score:.4f}')
    
    return train_losses, val_losses, dice_scores

def visualize_predictions(model, test_loader, num_samples=4):
    """Visualize U-Net predictions"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.eval()
    
    # Get a batch of test data
    data_iter = iter(test_loader)
    images, masks = next(data_iter)
    images, masks = images[:num_samples], masks[:num_samples]
    
    with torch.no_grad():
        images_gpu = images.to(device)
        outputs = model(images_gpu)
        predictions = torch.sigmoid(outputs) > 0.5
        predictions = predictions.cpu()
    
    # Plot results
    fig, axes = plt.subplots(num_samples, 3, figsize=(12, num_samples * 3))
    if num_samples == 1:
        axes = axes.reshape(1, -1)
    
    for i in range(num_samples):
        # Original image
        img = images[i].permute(1, 2, 0).numpy()
        axes[i, 0].imshow(img)
        axes[i, 0].set_title('Original Image')
        axes[i, 0].axis('off')
        
        # Ground truth mask
        axes[i, 1].imshow(masks[i].squeeze(), cmap='gray')
        axes[i, 1].set_title('Ground Truth')
        axes[i, 1].axis('off')
        
        # Prediction
        axes[i, 2].imshow(predictions[i].squeeze(), cmap='gray')
        axes[i, 2].set_title('Prediction')
        axes[i, 2].axis('off')
    
    plt.suptitle('U-Net Segmentation Results')
    plt.tight_layout()
    plt.show()

def visualize_feature_maps(model, test_loader, layer_name='down1'):
    """Visualize intermediate feature maps"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.eval()
    
    # Get a test image
    data_iter = iter(test_loader)
    images, _ = next(data_iter)
    test_image = images[0:1].to(device)
    
    # Register hook to capture feature maps
    feature_maps = {}
    def get_activation(name):
        def hook(model, input, output):
            feature_maps[name] = output.detach()
        return hook
    
    # Register hook on the specified layer
    if hasattr(model, layer_name):
        getattr(model, layer_name).register_forward_hook(get_activation(layer_name))
    
    # Forward pass
    with torch.no_grad():
        _ = model(test_image)
    
    if layer_name in feature_maps:
        features = feature_maps[layer_name].cpu().numpy()[0]  # Remove batch dimension
        
        # Plot feature maps
        num_features = min(16, features.shape[0])
        fig, axes = plt.subplots(4, 4, figsize=(12, 12))
        
        for i in range(num_features):
            row, col = i // 4, i % 4
            axes[row, col].imshow(features[i], cmap='viridis')
            axes[row, col].set_title(f'Feature {i}')
            axes[row, col].axis('off')
        
        plt.suptitle(f'Feature Maps from {layer_name}')
        plt.tight_layout()
        plt.show()
    else:
        print(f"Layer {layer_name} not found!")

def compare_architectures():
    """Compare U-Net with and without skip connections"""
    print("=== Comparing U-Net Architectures ===")
    
    class UNetNoSkip(nn.Module):
        """U-Net without skip connections for comparison"""
        def __init__(self, n_channels=3, n_classes=1):
            super(UNetNoSkip, self).__init__()
            
            # Encoder
            self.inc = DoubleConv(n_channels, 64)
            self.down1 = DownBlock(64, 128)
            self.down2 = DownBlock(128, 256)
            self.down3 = DownBlock(256, 512)
            self.down4 = DownBlock(512, 1024)
            
            # Decoder (no skip connections)
            self.up1 = nn.ConvTranspose2d(1024, 512, kernel_size=2, stride=2)
            self.conv1 = DoubleConv(512, 512)
            self.up2 = nn.ConvTranspose2d(512, 256, kernel_size=2, stride=2)
            self.conv2 = DoubleConv(256, 256)
            self.up3 = nn.ConvTranspose2d(256, 128, kernel_size=2, stride=2)
            self.conv3 = DoubleConv(128, 128)
            self.up4 = nn.ConvTranspose2d(128, 64, kernel_size=2, stride=2)
            self.conv4 = DoubleConv(64, 64)
            
            self.outc = nn.Conv2d(64, n_classes, kernel_size=1)
        
        def forward(self, x):
            # Encoder
            x1 = self.inc(x)
            x2 = self.down1(x1)
            x3 = self.down2(x2)
            x4 = self.down3(x3)
            x5 = self.down4(x4)
            
            # Decoder (no skip connections)
            x = self.conv1(self.up1(x5))
            x = self.conv2(self.up2(x))
            x = self.conv3(self.up3(x))
            x = self.conv4(self.up4(x))
            
            return self.outc(x)
    
    # Create datasets
    train_dataset = SimpleSegmentationDataset(num_samples=800, image_size=128)
    val_dataset = SimpleSegmentationDataset(num_samples=200, image_size=128)
    
    train_loader = DataLoader(train_dataset, batch_size=8, shuffle=True)
    val_loader = DataLoader(val_dataset, batch_size=8, shuffle=False)
    
    # Models to compare
    models = {
        'U-Net (with skip)': UNet(n_channels=3, n_classes=1),
        'U-Net (no skip)': UNetNoSkip(n_channels=3, n_classes=1)
    }
    
    results = {}
    
    for name, model in models.items():
        print(f"\nTraining {name}...")
        
        # Count parameters
        total_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
        print(f"Parameters: {total_params:,}")
        
        # Train model
        train_losses, val_losses, dice_scores = train_unet(
            model, train_loader, val_loader, num_epochs=20
        )
        
        results[name] = {
            'model': model,
            'train_losses': train_losses,
            'val_losses': val_losses,
            'dice_scores': dice_scores,
            'final_dice': dice_scores[-1],
            'parameters': total_params
        }
        
        print(f"Final Dice Score: {dice_scores[-1]:.4f}")
    
    # Plot comparison
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    
    # Training loss
    for name, data in results.items():
        axes[0].plot(data['train_losses'], label=name, linewidth=2)
    axes[0].set_title('Training Loss')
    axes[0].set_xlabel('Epoch')
    axes[0].set_ylabel('Loss')
    axes[0].legend()
    axes[0].grid(True)
    
    # Validation loss
    for name, data in results.items():
        axes[1].plot(data['val_losses'], label=name, linewidth=2)
    axes[1].set_title('Validation Loss')
    axes[1].set_xlabel('Epoch')
    axes[1].set_ylabel('Loss')
    axes[1].legend()
    axes[1].grid(True)
    
    # Dice scores
    for name, data in results.items():
        axes[2].plot(data['dice_scores'], label=name, linewidth=2)
    axes[2].set_title('Dice Score')
    axes[2].set_xlabel('Epoch')
    axes[2].set_ylabel('Dice Score')
    axes[2].legend()
    axes[2].grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Show sample predictions
    for name, data in results.items():
        print(f"\n{name} Predictions:")
        visualize_predictions(data['model'], val_loader, num_samples=2)
    
    return results

def analyze_skip_connections():
    """Analyze the importance of skip connections in U-Net"""
    print("\n=== Skip Connection Analysis ===")
    
    # Create a simple visualization showing information flow
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 8))
    
    # U-Net with skip connections
    ax1.set_title('U-Net with Skip Connections')
    
    # Draw encoder path
    encoder_y = [0.9, 0.7, 0.5, 0.3, 0.1]
    decoder_y = [0.1, 0.3, 0.5, 0.7, 0.9]
    
    for i, y in enumerate(encoder_y):
        ax1.add_patch(plt.Rectangle((0.1, y-0.05), 0.15, 0.1, facecolor='lightblue', edgecolor='black'))
        ax1.text(0.175, y, f'E{i+1}', ha='center', va='center', fontweight='bold')
    
    for i, y in enumerate(decoder_y):
        ax1.add_patch(plt.Rectangle((0.75, y-0.05), 0.15, 0.1, facecolor='lightgreen', edgecolor='black'))
        ax1.text(0.825, y, f'D{i+1}', ha='center', va='center', fontweight='bold')
    
    # Draw connections
    for i in range(4):
        # Down connections
        ax1.arrow(0.175, encoder_y[i]-0.05, 0, -0.1, head_width=0.02, head_length=0.02, fc='blue', ec='blue')
        # Up connections
        ax1.arrow(0.825, decoder_y[i]+0.05, 0, 0.1, head_width=0.02, head_length=0.02, fc='green', ec='green')
        # Skip connections
        ax1.arrow(0.25, encoder_y[i], 0.5, decoder_y[3-i]-encoder_y[i], head_width=0.02, head_length=0.02, fc='red', ec='red', linestyle='--')
    
    ax1.text(0.5, 0.95, 'Skip connections preserve\nhigh-resolution features', ha='center', fontsize=12, fontweight='bold')
    ax1.set_xlim(0, 1)
    ax1.set_ylim(0, 1)
    ax1.axis('off')
    
    # Traditional encoder-decoder
    ax2.set_title('Traditional Encoder-Decoder')
    
    for i, y in enumerate(encoder_y):
        ax2.add_patch(plt.Rectangle((0.1, y-0.05), 0.15, 0.1, facecolor='lightblue', edgecolor='black'))
        ax2.text(0.175, y, f'E{i+1}', ha='center', va='center', fontweight='bold')
    
    for i, y in enumerate(decoder_y):
        ax2.add_patch(plt.Rectangle((0.75, y-0.05), 0.15, 0.1, facecolor='lightgreen', edgecolor='black'))
        ax2.text(0.825, y, f'D{i+1}', ha='center', va='center', fontweight='bold')
    
    # Only down and up connections
    for i in range(4):
        ax2.arrow(0.175, encoder_y[i]-0.05, 0, -0.1, head_width=0.02, head_length=0.02, fc='blue', ec='blue')
        ax2.arrow(0.825, decoder_y[i]+0.05, 0, 0.1, head_width=0.02, head_length=0.02, fc='green', ec='green')
    
    # Bottleneck connection
    ax2.arrow(0.25, encoder_y[-1], 0.5, 0, head_width=0.02, head_length=0.02, fc='purple', ec='purple')
    
    ax2.text(0.5, 0.95, 'Information flows only\nthrough bottleneck', ha='center', fontsize=12, fontweight='bold')
    ax2.set_xlim(0, 1)
    ax2.set_ylim(0, 1)
    ax2.axis('off')
    
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== U-Net Implementation for Semantic Segmentation ===")
    
    # Create synthetic dataset
    train_dataset = SimpleSegmentationDataset(num_samples=1000, image_size=128)
    val_dataset = SimpleSegmentationDataset(num_samples=200, image_size=128)
    test_dataset = SimpleSegmentationDataset(num_samples=100, image_size=128)
    
    train_loader = DataLoader(train_dataset, batch_size=16, shuffle=True)
    val_loader = DataLoader(val_dataset, batch_size=16, shuffle=False)
    test_loader = DataLoader(test_dataset, batch_size=4, shuffle=False)
    
    print(f"Training samples: {len(train_dataset)}")
    print(f"Validation samples: {len(val_dataset)}")
    print(f"Test samples: {len(test_dataset)}")
    
    # Train U-Net
    print("\n=== Training U-Net ===")
    model = UNet(n_channels=3, n_classes=1, bilinear=True)
    train_losses, val_losses, dice_scores = train_unet(
        model, train_loader, val_loader, num_epochs=25
    )
    
    # Plot training curves
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    
    axes[0].plot(train_losses, label='Training', linewidth=2)
    axes[0].plot(val_losses, label='Validation', linewidth=2)
    axes[0].set_title('Loss Curves')
    axes[0].set_xlabel('Epoch')
    axes[0].set_ylabel('Loss')
    axes[0].legend()
    axes[0].grid(True)
    
    axes[1].plot(dice_scores, 'g-', linewidth=2)
    axes[1].set_title('Dice Score')
    axes[1].set_xlabel('Epoch')
    axes[1].set_ylabel('Dice Score')
    axes[1].grid(True)
    
    axes[2].bar(['Final Dice'], [dice_scores[-1]])
    axes[2].set_title('Final Performance')
    axes[2].set_ylabel('Dice Score')
    axes[2].set_ylim(0, 1)
    
    plt.tight_layout()
    plt.show()
    
    # Visualize predictions
    print("\n=== Prediction Visualization ===")
    visualize_predictions(model, test_loader, num_samples=4)
    
    # Visualize feature maps
    print("\n=== Feature Map Visualization ===")
    visualize_feature_maps(model, test_loader, 'down2')
    
    # Compare architectures
    comparison_results = compare_architectures()
    
    # Analyze skip connections
    analyze_skip_connections()
    
    print("\n=== U-Net Key Concepts ===")
    print("1. ENCODER-DECODER ARCHITECTURE:")
    print("   - Encoder: Captures context through downsampling")
    print("   - Decoder: Enables precise localization through upsampling")
    print("   - Symmetric design with contracting and expansive paths")
    
    print("\n2. SKIP CONNECTIONS:")
    print("   - Connect encoder and decoder at same resolution levels")
    print("   - Preserve fine-grained spatial information")
    print("   - Enable gradient flow and faster convergence")
    print("   - Concatenation provides both context and detail")
    
    print("\n3. SEGMENTATION-SPECIFIC DESIGN:")
    print("   - Pixel-wise classification")
    print("   - Preserves spatial dimensions")
    print("   - Handles variable input sizes")
    print("   - Suitable for dense prediction tasks")
    
    print("\n4. LOSS FUNCTIONS FOR SEGMENTATION:")
    print("   - Dice Loss: Overlap-based metric")
    print("   - IoU Loss: Intersection over Union")
    print("   - Focal Loss: Handles class imbalance")
    print("   - Combined losses for better performance")
    
    print("\n=== U-Net Advantages ===")
    print("✓ Works with small datasets")
    print("✓ Precise localization through skip connections")
    print("✓ End-to-end training")
    print("✓ Handles variable input sizes")
    print("✓ State-of-the-art segmentation performance")
    print("✓ Data augmentation friendly")
    
    print("\n=== Applications ===")
    print("• Medical image segmentation")
    print("• Satellite image analysis")
    print("• Autonomous driving (road segmentation)")
    print("• Object detection and localization")
    print("• Image restoration and denoising")
    print("• Super-resolution")
    print("• Style transfer")
    
    print("\n=== U-Net Variants ===")
    print("• Attention U-Net: Attention gates for better feature selection")
    print("• U-Net++: Nested skip connections")
    print("• 3D U-Net: Volumetric segmentation")
    print("• ResU-Net: Residual connections in encoder/decoder")
    print("• Dense U-Net: Dense connections")
    print("• TransUNet: Transformer-based U-Net")
    
    print("\n=== Performance Metrics ===")
    print("• Dice Coefficient: 2|A∩B|/(|A|+|B|)")
    print("• IoU (Jaccard): |A∩B|/|A∪B|")
    print("• Pixel Accuracy: Correct pixels / Total pixels")
    print("• Sensitivity (Recall): TP/(TP+FN)")
    print("• Specificity: TN/(TN+FP)")
    print("• Hausdorff Distance: Boundary accuracy")