# author: csgod cmps studies
# Autoencoder - Unsupervised representation learning
# Autoencoders learn to compress data into a lower-dimensional representation
# and then reconstruct the original data from this compressed representation.

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.utils.data import DataLoader, Dataset
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np
from sklearn.manifold import TSNE
from sklearn.decomposition import PCA

class BasicAutoencoder(nn.Module):
    """Basic fully-connected autoencoder"""
    def __init__(self, input_size, hidden_sizes):
        super(BasicAutoencoder, self).__init__()
        
        self.input_size = input_size
        self.hidden_sizes = hidden_sizes
        
        # Encoder layers
        encoder_layers = []
        prev_size = input_size
        
        for hidden_size in hidden_sizes:
            encoder_layers.extend([
                nn.Linear(prev_size, hidden_size),
                nn.ReLU()
            ])
            prev_size = hidden_size
        
        self.encoder = nn.Sequential(*encoder_layers)
        
        # Decoder layers (reverse of encoder)
        decoder_layers = []
        reversed_sizes = hidden_sizes[::-1]  # Reverse the hidden sizes
        
        for i, hidden_size in enumerate(reversed_sizes[1:]):
            decoder_layers.extend([
                nn.Linear(reversed_sizes[i], hidden_size),
                nn.ReLU()
            ])
        
        # Final layer back to input size
        decoder_layers.extend([
            nn.Linear(reversed_sizes[-1], input_size),
            nn.Sigmoid()  # For normalized input [0,1]
        ])
        
        self.decoder = nn.Sequential(*decoder_layers)
    
    def encode(self, x):
        return self.encoder(x)
    
    def decode(self, z):
        return self.decoder(z)
    
    def forward(self, x):
        z = self.encode(x)
        x_reconstructed = self.decode(z)
        return x_reconstructed, z

class ConvolutionalAutoencoder(nn.Module):
    """Convolutional autoencoder for image data"""
    def __init__(self, input_channels=1, latent_dim=128):
        super(ConvolutionalAutoencoder, self).__init__()
        
        # Encoder
        self.encoder = nn.Sequential(
            # 28x28 -> 14x14
            nn.Conv2d(input_channels, 32, 3, stride=2, padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(32),
            
            # 14x14 -> 7x7
            nn.Conv2d(32, 64, 3, stride=2, padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(64),
            
            # 7x7 -> 4x4 (with padding adjustment)
            nn.Conv2d(64, 128, 3, stride=2, padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(128),
        )
        
        # Latent representation
        self.flatten = nn.Flatten()
        self.encode_fc = nn.Linear(128 * 4 * 4, latent_dim)
        self.decode_fc = nn.Linear(latent_dim, 128 * 4 * 4)
        self.unflatten = nn.Unflatten(1, (128, 4, 4))
        
        # Decoder
        self.decoder = nn.Sequential(
            # 4x4 -> 7x7
            nn.ConvTranspose2d(128, 64, 3, stride=2, padding=1, output_padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(64),
            
            # 7x7 -> 14x14
            nn.ConvTranspose2d(64, 32, 3, stride=2, padding=1, output_padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(32),
            
            # 14x14 -> 28x28
            nn.ConvTranspose2d(32, input_channels, 3, stride=2, padding=1, output_padding=1),
            nn.Sigmoid()
        )
    
    def encode(self, x):
        x = self.encoder(x)
        x = self.flatten(x)
        z = self.encode_fc(x)
        return z
    
    def decode(self, z):
        x = self.decode_fc(z)
        x = self.unflatten(x)
        x = self.decoder(x)
        return x
    
    def forward(self, x):
        z = self.encode(x)
        x_reconstructed = self.decode(z)
        return x_reconstructed, z

class DenoisingAutoencoder(nn.Module):
    """Denoising autoencoder that learns to remove noise"""
    def __init__(self, input_channels=1, latent_dim=128):
        super(DenoisingAutoencoder, self).__init__()
        
        # Same architecture as ConvolutionalAutoencoder
        self.encoder = nn.Sequential(
            nn.Conv2d(input_channels, 32, 3, stride=2, padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(32),
            
            nn.Conv2d(32, 64, 3, stride=2, padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(64),
            
            nn.Conv2d(64, 128, 3, stride=2, padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(128),
        )
        
        self.flatten = nn.Flatten()
        self.encode_fc = nn.Linear(128 * 4 * 4, latent_dim)
        self.decode_fc = nn.Linear(latent_dim, 128 * 4 * 4)
        self.unflatten = nn.Unflatten(1, (128, 4, 4))
        
        self.decoder = nn.Sequential(
            nn.ConvTranspose2d(128, 64, 3, stride=2, padding=1, output_padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(64),
            
            nn.ConvTranspose2d(64, 32, 3, stride=2, padding=1, output_padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(32),
            
            nn.ConvTranspose2d(32, input_channels, 3, stride=2, padding=1, output_padding=1),
            nn.Sigmoid()
        )
    
    def encode(self, x):
        x = self.encoder(x)
        x = self.flatten(x)
        z = self.encode_fc(x)
        return z
    
    def decode(self, z):
        x = self.decode_fc(z)
        x = self.unflatten(x)
        x = self.decoder(x)
        return x
    
    def forward(self, x):
        z = self.encode(x)
        x_reconstructed = self.decode(z)
        return x_reconstructed, z

def add_noise(data, noise_factor=0.3):
    """Add Gaussian noise to data"""
    noise = torch.randn_like(data) * noise_factor
    noisy_data = data + noise
    return torch.clamp(noisy_data, 0, 1)

def train_autoencoder(model, train_loader, test_loader, num_epochs=20, learning_rate=0.001, noise_factor=0.0):
    """Train autoencoder"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=learning_rate, weight_decay=1e-5)
    
    train_losses = []
    test_losses = []
    
    for epoch in range(num_epochs):
        model.train()
        running_loss = 0.0
        
        for batch_idx, (data, _) in enumerate(train_loader):
            data = data.to(device)
            
            # Add noise for denoising autoencoder
            if noise_factor > 0:
                noisy_data = add_noise(data, noise_factor)
                optimizer.zero_grad()
                reconstructed, _ = model(noisy_data)
                loss = criterion(reconstructed, data)  # Reconstruct clean from noisy
            else:
                optimizer.zero_grad()
                reconstructed, _ = model(data)
                loss = criterion(reconstructed, data)
            
            loss.backward()
            optimizer.step()
            
            running_loss += loss.item()
        
        train_loss = running_loss / len(train_loader)
        
        # Test loss
        model.eval()
        test_loss = 0.0
        with torch.no_grad():
            for data, _ in test_loader:
                data = data.to(device)
                if noise_factor > 0:
                    noisy_data = add_noise(data, noise_factor)
                    reconstructed, _ = model(noisy_data)
                    loss = criterion(reconstructed, data)
                else:
                    reconstructed, _ = model(data)
                    loss = criterion(reconstructed, data)
                test_loss += loss.item()
        
        test_loss /= len(test_loader)
        train_losses.append(train_loss)
        test_losses.append(test_loss)
        
        print(f'Epoch [{epoch+1}/{num_epochs}], Train Loss: {train_loss:.4f}, Test Loss: {test_loss:.4f}')
    
    return train_losses, test_losses

def visualize_reconstructions(model, test_loader, num_samples=8, noise_factor=0.0):
    """Visualize original and reconstructed images"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.eval()
    
    # Get a batch of test data
    data_iter = iter(test_loader)
    data, _ = next(data_iter)
    data = data[:num_samples].to(device)
    
    with torch.no_grad():
        if noise_factor > 0:
            noisy_data = add_noise(data, noise_factor)
            reconstructed, _ = model(noisy_data)
            
            # Plot original, noisy, and reconstructed
            fig, axes = plt.subplots(3, num_samples, figsize=(15, 6))
            
            for i in range(num_samples):
                # Original
                axes[0, i].imshow(data[i].cpu().squeeze(), cmap='gray')
                axes[0, i].set_title('Original')
                axes[0, i].axis('off')
                
                # Noisy
                axes[1, i].imshow(noisy_data[i].cpu().squeeze(), cmap='gray')
                axes[1, i].set_title('Noisy')
                axes[1, i].axis('off')
                
                # Reconstructed
                axes[2, i].imshow(reconstructed[i].cpu().squeeze(), cmap='gray')
                axes[2, i].set_title('Reconstructed')
                axes[2, i].axis('off')
        else:
            reconstructed, _ = model(data)
            
            # Plot original and reconstructed
            fig, axes = plt.subplots(2, num_samples, figsize=(15, 4))
            
            for i in range(num_samples):
                # Original
                axes[0, i].imshow(data[i].cpu().squeeze(), cmap='gray')
                axes[0, i].set_title('Original')
                axes[0, i].axis('off')
                
                # Reconstructed
                axes[1, i].imshow(reconstructed[i].cpu().squeeze(), cmap='gray')
                axes[1, i].set_title('Reconstructed')
                axes[1, i].axis('off')
    
    plt.tight_layout()
    plt.show()

def visualize_latent_space(model, test_loader, num_samples=1000):
    """Visualize latent space using t-SNE"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.eval()
    
    latent_vectors = []
    labels = []
    
    with torch.no_grad():
        sample_count = 0
        for data, target in test_loader:
            if sample_count >= num_samples:
                break
            
            data = data.to(device)
            _, z = model(data)
            
            latent_vectors.append(z.cpu().numpy())
            labels.append(target.numpy())
            
            sample_count += data.size(0)
    
    # Concatenate all latent vectors
    latent_vectors = np.concatenate(latent_vectors, axis=0)[:num_samples]
    labels = np.concatenate(labels, axis=0)[:num_samples]
    
    print(f"Latent space dimension: {latent_vectors.shape[1]}")
    
    # Apply t-SNE for 2D visualization
    print("Applying t-SNE...")
    tsne = TSNE(n_components=2, random_state=42, perplexity=30)
    latent_2d = tsne.fit_transform(latent_vectors)
    
    # Plot
    plt.figure(figsize=(12, 5))
    
    # t-SNE plot
    plt.subplot(1, 2, 1)
    scatter = plt.scatter(latent_2d[:, 0], latent_2d[:, 1], c=labels, cmap='tab10', alpha=0.7)
    plt.colorbar(scatter)
    plt.title('t-SNE of Latent Space')
    plt.xlabel('t-SNE 1')
    plt.ylabel('t-SNE 2')
    
    # PCA plot
    plt.subplot(1, 2, 2)
    pca = PCA(n_components=2)
    latent_pca = pca.fit_transform(latent_vectors)
    scatter = plt.scatter(latent_pca[:, 0], latent_pca[:, 1], c=labels, cmap='tab10', alpha=0.7)
    plt.colorbar(scatter)
    plt.title('PCA of Latent Space')
    plt.xlabel(f'PC1 ({pca.explained_variance_ratio_[0]:.2%} variance)')
    plt.ylabel(f'PC2 ({pca.explained_variance_ratio_[1]:.2%} variance)')
    
    plt.tight_layout()
    plt.show()

def demonstrate_interpolation(model, test_loader, num_steps=10):
    """Demonstrate interpolation in latent space"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.eval()
    
    # Get two random samples
    data_iter = iter(test_loader)
    data, _ = next(data_iter)
    
    img1 = data[0:1].to(device)
    img2 = data[1:2].to(device)
    
    with torch.no_grad():
        # Encode both images
        z1 = model.encode(img1)
        z2 = model.encode(img2)
        
        # Interpolate in latent space
        interpolated_images = []
        
        for i in range(num_steps):
            alpha = i / (num_steps - 1)
            z_interp = (1 - alpha) * z1 + alpha * z2
            img_interp = model.decode(z_interp)
            interpolated_images.append(img_interp.cpu())
    
    # Plot interpolation
    plt.figure(figsize=(15, 3))
    for i, img in enumerate(interpolated_images):
        plt.subplot(1, num_steps, i + 1)
        plt.imshow(img.squeeze(), cmap='gray')
        plt.title(f'α={i/(num_steps-1):.1f}')
        plt.axis('off')
    
    plt.suptitle('Latent Space Interpolation')
    plt.tight_layout()
    plt.show()

def compare_autoencoder_types():
    """Compare different types of autoencoders"""
    print("=== Comparing Autoencoder Types ===")
    
    # Load MNIST data
    transform = transforms.Compose([
        transforms.ToTensor(),
    ])
    
    train_dataset = torchvision.datasets.MNIST(
        root='./data', train=True, download=True, transform=transform
    )
    test_dataset = torchvision.datasets.MNIST(
        root='./data', train=False, download=True, transform=transform
    )
    
    train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=128, shuffle=False)
    
    # Different autoencoder types
    autoencoders = {
        'Basic': BasicAutoencoder(784, [256, 128, 64]),
        'Convolutional': ConvolutionalAutoencoder(1, 64),
        'Denoising': DenoisingAutoencoder(1, 64)
    }
    
    results = {}
    
    for name, model in autoencoders.items():
        print(f"\nTraining {name} Autoencoder...")
        
        # Count parameters
        total_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
        print(f"Parameters: {total_params:,}")
        
        # Train model
        noise_factor = 0.3 if name == 'Denoising' else 0.0
        
        # For basic autoencoder, we need to flatten the data
        if name == 'Basic':
            # Create custom data loader for flattened data
            class FlattenDataset(Dataset):
                def __init__(self, dataset):
                    self.dataset = dataset
                
                def __len__(self):
                    return len(self.dataset)
                
                def __getitem__(self, idx):
                    data, target = self.dataset[idx]
                    return data.view(-1), target
            
            flat_train = FlattenDataset(train_dataset)
            flat_test = FlattenDataset(test_dataset)
            flat_train_loader = DataLoader(flat_train, batch_size=128, shuffle=True)
            flat_test_loader = DataLoader(flat_test, batch_size=128, shuffle=False)
            
            train_losses, test_losses = train_autoencoder(
                model, flat_train_loader, flat_test_loader, 
                num_epochs=15, noise_factor=noise_factor
            )
        else:
            train_losses, test_losses = train_autoencoder(
                model, train_loader, test_loader, 
                num_epochs=15, noise_factor=noise_factor
            )
        
        results[name] = {
            'model': model,
            'train_losses': train_losses,
            'test_losses': test_losses,
            'parameters': total_params
        }
        
        print(f"Final test loss: {test_losses[-1]:.4f}")
    
    # Plot training curves
    plt.figure(figsize=(12, 5))
    
    plt.subplot(1, 2, 1)
    for name, data in results.items():
        plt.plot(data['train_losses'], label=f'{name}', linewidth=2)
    plt.title('Training Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.grid(True)
    
    plt.subplot(1, 2, 2)
    for name, data in results.items():
        plt.plot(data['test_losses'], label=f'{name}', linewidth=2)
    plt.title('Test Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Visualize reconstructions for each type
    for name, data in results.items():
        if name != 'Basic':  # Skip basic for image visualization
            print(f"\n{name} Autoencoder Reconstructions:")
            noise_factor = 0.3 if name == 'Denoising' else 0.0
            visualize_reconstructions(data['model'], test_loader, 
                                   num_samples=8, noise_factor=noise_factor)
    
    return results

def analyze_compression_quality():
    """Analyze compression quality at different bottleneck sizes"""
    print("\n=== Compression Quality Analysis ===")
    
    # Load data
    transform = transforms.Compose([transforms.ToTensor()])
    train_dataset = torchvision.datasets.MNIST(root='./data', train=True, download=True, transform=transform)
    test_dataset = torchvision.datasets.MNIST(root='./data', train=False, download=True, transform=transform)
    
    train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=128, shuffle=False)
    
    # Different latent dimensions
    latent_dims = [8, 16, 32, 64, 128]
    results = {}
    
    for latent_dim in latent_dims:
        print(f"\nTraining autoencoder with latent dim {latent_dim}...")
        
        model = ConvolutionalAutoencoder(1, latent_dim)
        train_losses, test_losses = train_autoencoder(
            model, train_loader, test_loader, num_epochs=10
        )
        
        # Calculate compression ratio
        input_size = 28 * 28  # MNIST image size
        compression_ratio = input_size / latent_dim
        
        results[latent_dim] = {
            'model': model,
            'final_loss': test_losses[-1],
            'compression_ratio': compression_ratio
        }
        
        print(f"Latent dim: {latent_dim}, Compression ratio: {compression_ratio:.1f}x, "
              f"Final loss: {test_losses[-1]:.4f}")
    
    # Plot compression vs quality trade-off
    dims = list(results.keys())
    losses = [results[d]['final_loss'] for d in dims]
    ratios = [results[d]['compression_ratio'] for d in dims]
    
    plt.figure(figsize=(10, 6))
    plt.plot(ratios, losses, 'bo-', linewidth=2, markersize=8)
    plt.xlabel('Compression Ratio')
    plt.ylabel('Reconstruction Loss')
    plt.title('Compression vs Quality Trade-off')
    plt.grid(True)
    
    # Add labels for each point
    for dim, ratio, loss in zip(dims, ratios, losses):
        plt.annotate(f'{dim}D', (ratio, loss), textcoords="offset points", 
                    xytext=(0,10), ha='center')
    
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Autoencoder Implementation and Analysis ===")
    
    # Compare different autoencoder types
    comparison_results = compare_autoencoder_types()
    
    # Get the convolutional autoencoder for further analysis
    conv_model = comparison_results['Convolutional']['model']
    
    # Load test data for visualization
    transform = transforms.Compose([transforms.ToTensor()])
    test_dataset = torchvision.datasets.MNIST(root='./data', train=False, transform=transform)
    test_loader = DataLoader(test_dataset, batch_size=64, shuffle=False)
    
    # Visualize latent space
    print("\nVisualizing latent space...")
    visualize_latent_space(conv_model, test_loader, num_samples=1000)
    
    # Demonstrate interpolation
    print("\nDemonstrating latent space interpolation...")
    demonstrate_interpolation(conv_model, test_loader)
    
    # Analyze compression quality
    analyze_compression_quality()
    
    print("\n=== Autoencoder Key Concepts ===")
    print("1. UNSUPERVISED LEARNING: Learn representations without labels")
    print("2. ENCODER-DECODER ARCHITECTURE:")
    print("   - Encoder: x → z (compress to latent space)")
    print("   - Decoder: z → x̂ (reconstruct from latent space)")
    print("3. BOTTLENECK: Forces learning of compressed representations")
    print("4. RECONSTRUCTION LOSS: L(x, x̂) = ||x - x̂||²")
    
    print("\n=== Types of Autoencoders ===")
    print("• BASIC: Fully connected layers")
    print("• CONVOLUTIONAL: CNNs for spatial data")
    print("• DENOISING: Learn to remove noise")
    print("• SPARSE: Encourage sparse activations")
    print("• VARIATIONAL: Probabilistic latent space")
    print("• CONTRACTIVE: Robust to small perturbations")
    
    print("\n=== Applications ===")
    print("✓ Dimensionality reduction")
    print("✓ Data compression")
    print("✓ Anomaly detection")
    print("✓ Image denoising")
    print("✓ Feature learning")
    print("✓ Data generation")
    print("✓ Pretraining for supervised tasks")
    
    print("\n=== Advantages & Limitations ===")
    print("ADVANTAGES:")
    print("• No labels required")
    print("• Learn useful representations")
    print("• Flexible architecture")
    print("• Good for visualization")
    
    print("\nLIMITATIONS:")
    print("• Blurry reconstructions")
    print("• Mode collapse in generation")
    print("• Difficult to sample new data")
    print("• May not capture all data variations")