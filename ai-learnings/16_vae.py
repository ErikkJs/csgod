# author: csgod cmps studies
# VAE - Variational Autoencoder
# VAEs learn probabilistic latent representations by encoding data into distributions
# rather than fixed points, enabling principled data generation and interpolation.

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.data import DataLoader
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np
from sklearn.manifold import TSNE

class VAE(nn.Module):
    """Variational Autoencoder implementation"""
    def __init__(self, input_dim=784, latent_dim=20, hidden_dim=400):
        super(VAE, self).__init__()
        self.latent_dim = latent_dim
        
        # Encoder
        self.fc1 = nn.Linear(input_dim, hidden_dim)
        self.fc2_mu = nn.Linear(hidden_dim, latent_dim)      # Mean of latent distribution
        self.fc2_logvar = nn.Linear(hidden_dim, latent_dim)  # Log variance of latent distribution
        
        # Decoder
        self.fc3 = nn.Linear(latent_dim, hidden_dim)
        self.fc4 = nn.Linear(hidden_dim, input_dim)
    
    def encode(self, x):
        """Encode input to latent distribution parameters"""
        h = F.relu(self.fc1(x))
        mu = self.fc2_mu(h)
        logvar = self.fc2_logvar(h)
        return mu, logvar
    
    def reparameterize(self, mu, logvar):
        """Reparameterization trick: sample from N(mu, var) using N(0,1)"""
        if self.training:
            std = torch.exp(0.5 * logvar)
            eps = torch.randn_like(std)
            return mu + eps * std
        else:
            return mu  # Use mean during evaluation
    
    def decode(self, z):
        """Decode latent sample to reconstruction"""
        h = F.relu(self.fc3(z))
        return torch.sigmoid(self.fc4(h))
    
    def forward(self, x):
        mu, logvar = self.encode(x)
        z = self.reparameterize(mu, logvar)
        x_recon = self.decode(z)
        return x_recon, mu, logvar, z

class ConvVAE(nn.Module):
    """Convolutional Variational Autoencoder for image data"""
    def __init__(self, input_channels=1, latent_dim=128):
        super(ConvVAE, self).__init__()
        self.latent_dim = latent_dim
        
        # Encoder
        self.encoder = nn.Sequential(
            nn.Conv2d(input_channels, 32, 4, stride=2, padding=1),  # 28x28 -> 14x14
            nn.ReLU(),
            nn.Conv2d(32, 64, 4, stride=2, padding=1),              # 14x14 -> 7x7
            nn.ReLU(),
            nn.Conv2d(64, 128, 4, stride=2, padding=1),             # 7x7 -> 4x4 (with padding)
            nn.ReLU(),
        )
        
        # Calculate the size after convolutions
        self.conv_output_size = 128 * 4 * 4  # Adjust based on your input size
        
        # Latent space
        self.fc_mu = nn.Linear(self.conv_output_size, latent_dim)
        self.fc_logvar = nn.Linear(self.conv_output_size, latent_dim)
        
        # Decoder
        self.fc_decode = nn.Linear(latent_dim, self.conv_output_size)
        
        self.decoder = nn.Sequential(
            nn.ConvTranspose2d(128, 64, 4, stride=2, padding=1),    # 4x4 -> 7x7
            nn.ReLU(),
            nn.ConvTranspose2d(64, 32, 4, stride=2, padding=1),     # 7x7 -> 14x14
            nn.ReLU(),
            nn.ConvTranspose2d(32, input_channels, 4, stride=2, padding=1),  # 14x14 -> 28x28
            nn.Sigmoid()
        )
    
    def encode(self, x):
        h = self.encoder(x)
        h = h.view(h.size(0), -1)  # Flatten
        mu = self.fc_mu(h)
        logvar = self.fc_logvar(h)
        return mu, logvar
    
    def reparameterize(self, mu, logvar):
        if self.training:
            std = torch.exp(0.5 * logvar)
            eps = torch.randn_like(std)
            return mu + eps * std
        else:
            return mu
    
    def decode(self, z):
        h = self.fc_decode(z)
        h = h.view(h.size(0), 128, 4, 4)  # Reshape for convolutions
        return self.decoder(h)
    
    def forward(self, x):
        mu, logvar = self.encode(x)
        z = self.reparameterize(mu, logvar)
        x_recon = self.decode(z)
        return x_recon, mu, logvar, z

def vae_loss_function(x_recon, x, mu, logvar, beta=1.0):
    """VAE loss = Reconstruction loss + KL divergence"""
    # Reconstruction loss (Binary Cross Entropy)
    BCE = F.binary_cross_entropy(x_recon, x, reduction='sum')
    
    # KL divergence loss
    # KL(q(z|x) || p(z)) where p(z) = N(0,1) and q(z|x) = N(mu, var)
    KL = -0.5 * torch.sum(1 + logvar - mu.pow(2) - logvar.exp())
    
    return BCE + beta * KL, BCE, KL

def train_vae(model, train_loader, test_loader, num_epochs=50, learning_rate=1e-3, beta=1.0):
    """Train VAE model"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    
    optimizer = optim.Adam(model.parameters(), lr=learning_rate)
    
    train_losses = []
    test_losses = []
    kl_losses = []
    recon_losses = []
    
    for epoch in range(num_epochs):
        model.train()
        train_loss = 0
        train_kl = 0
        train_recon = 0
        
        for batch_idx, (data, _) in enumerate(train_loader):
            data = data.to(device)
            
            # Flatten data for fully connected VAE
            if len(data.shape) == 4 and isinstance(model, VAE):
                data = data.view(data.size(0), -1)
            
            optimizer.zero_grad()
            x_recon, mu, logvar, z = model(data)
            loss, bce, kl = vae_loss_function(x_recon, data, mu, logvar, beta)
            
            loss.backward()
            optimizer.step()
            
            train_loss += loss.item()
            train_recon += bce.item()
            train_kl += kl.item()
        
        # Test evaluation
        model.eval()
        test_loss = 0
        with torch.no_grad():
            for data, _ in test_loader:
                data = data.to(device)
                if len(data.shape) == 4 and isinstance(model, VAE):
                    data = data.view(data.size(0), -1)
                
                x_recon, mu, logvar, z = model(data)
                loss, _, _ = vae_loss_function(x_recon, data, mu, logvar, beta)
                test_loss += loss.item()
        
        avg_train_loss = train_loss / len(train_loader.dataset)
        avg_test_loss = test_loss / len(test_loader.dataset)
        avg_recon = train_recon / len(train_loader.dataset)
        avg_kl = train_kl / len(train_loader.dataset)
        
        train_losses.append(avg_train_loss)
        test_losses.append(avg_test_loss)
        recon_losses.append(avg_recon)
        kl_losses.append(avg_kl)
        
        if epoch % 10 == 0:
            print(f'Epoch [{epoch+1}/{num_epochs}], Train Loss: {avg_train_loss:.4f}, '
                  f'Test Loss: {avg_test_loss:.4f}, Recon: {avg_recon:.4f}, KL: {avg_kl:.4f}')
    
    return train_losses, test_losses, recon_losses, kl_losses

def visualize_reconstructions(model, test_loader, num_samples=8):
    """Visualize original vs reconstructed images"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.eval()
    
    data_iter = iter(test_loader)
    data, _ = next(data_iter)
    data = data[:num_samples].to(device)
    
    with torch.no_grad():
        if isinstance(model, VAE):
            data_flat = data.view(data.size(0), -1)
            x_recon, mu, logvar, z = model(data_flat)
            x_recon = x_recon.view_as(data)
        else:
            x_recon, mu, logvar, z = model(data)
    
    # Plot original and reconstructed
    fig, axes = plt.subplots(2, num_samples, figsize=(15, 4))
    
    for i in range(num_samples):
        # Original
        axes[0, i].imshow(data[i].cpu().squeeze(), cmap='gray')
        axes[0, i].set_title('Original')
        axes[0, i].axis('off')
        
        # Reconstructed
        axes[1, i].imshow(x_recon[i].cpu().squeeze(), cmap='gray')
        axes[1, i].set_title('Reconstructed')
        axes[1, i].axis('off')
    
    plt.suptitle('VAE Reconstructions')
    plt.tight_layout()
    plt.show()

def generate_new_samples(model, num_samples=16, latent_dim=20):
    """Generate new samples from the prior distribution"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.eval()
    
    with torch.no_grad():
        # Sample from standard normal distribution
        z = torch.randn(num_samples, latent_dim).to(device)
        
        # Decode to generate new samples
        generated = model.decode(z)
        
        # Reshape if necessary
        if isinstance(model, VAE):
            generated = generated.view(num_samples, 1, 28, 28)
    
    # Plot generated samples
    fig, axes = plt.subplots(4, 4, figsize=(8, 8))
    
    for i, ax in enumerate(axes.flat):
        ax.imshow(generated[i].cpu().squeeze(), cmap='gray')
        ax.axis('off')
    
    plt.suptitle('Generated Samples from VAE')
    plt.tight_layout()
    plt.show()

def interpolate_in_latent_space(model, test_loader, num_steps=10):
    """Interpolate between two images in latent space"""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.eval()
    
    # Get two random samples
    data_iter = iter(test_loader)
    data, _ = next(data_iter)
    
    img1 = data[0:1].to(device)
    img2 = data[1:2].to(device)
    
    with torch.no_grad():
        # Encode both images
        if isinstance(model, VAE):
            mu1, _ = model.encode(img1.view(1, -1))
            mu2, _ = model.encode(img2.view(1, -1))
        else:
            mu1, _ = model.encode(img1)
            mu2, _ = model.encode(img2)
        
        # Interpolate in latent space
        interpolated_images = []
        
        for i in range(num_steps):
            alpha = i / (num_steps - 1)
            z_interp = (1 - alpha) * mu1 + alpha * mu2
            
            # Decode interpolated latent vector
            img_interp = model.decode(z_interp)
            
            if isinstance(model, VAE):
                img_interp = img_interp.view(1, 1, 28, 28)
            
            interpolated_images.append(img_interp.cpu())
    
    # Plot interpolation
    plt.figure(figsize=(15, 3))
    for i, img in enumerate(interpolated_images):
        plt.subplot(1, num_steps, i + 1)
        plt.imshow(img.squeeze(), cmap='gray')
        plt.title(f'α={i/(num_steps-1):.1f}')
        plt.axis('off')
    
    plt.suptitle('Latent Space Interpolation (VAE)')
    plt.tight_layout()
    plt.show()

def visualize_latent_space(model, test_loader, num_samples=1000):
    """Visualize the learned latent space"""
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
            
            if isinstance(model, VAE):
                mu, logvar = model.encode(data.view(data.size(0), -1))
            else:
                mu, logvar = model.encode(data)
            
            # Use mean of the distribution for visualization
            latent_vectors.append(mu.cpu().numpy())
            labels.append(target.numpy())
            
            sample_count += data.size(0)
    
    # Concatenate all latent vectors
    latent_vectors = np.concatenate(latent_vectors, axis=0)[:num_samples]
    labels = np.concatenate(labels, axis=0)[:num_samples]
    
    print(f"Latent space dimension: {latent_vectors.shape[1]}")
    
    # Apply t-SNE for 2D visualization
    if latent_vectors.shape[1] > 2:
        print("Applying t-SNE for dimensionality reduction...")
        tsne = TSNE(n_components=2, random_state=42, perplexity=30)
        latent_2d = tsne.fit_transform(latent_vectors)
    else:
        latent_2d = latent_vectors
    
    # Plot
    plt.figure(figsize=(10, 8))
    scatter = plt.scatter(latent_2d[:, 0], latent_2d[:, 1], c=labels, cmap='tab10', alpha=0.7)
    plt.colorbar(scatter)
    plt.title('VAE Latent Space Visualization (t-SNE)')
    plt.xlabel('Dimension 1')
    plt.ylabel('Dimension 2')
    plt.grid(True, alpha=0.3)
    plt.show()

def compare_beta_values():
    """Compare different beta values for β-VAE"""
    print("=== Comparing β-VAE with different β values ===")
    
    # Load data
    transform = transforms.Compose([transforms.ToTensor()])
    train_dataset = torchvision.datasets.MNIST(root='./data', train=True, download=True, transform=transform)
    test_dataset = torchvision.datasets.MNIST(root='./data', train=False, download=True, transform=transform)
    
    train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=128, shuffle=False)
    
    beta_values = [0.1, 1.0, 4.0, 10.0]
    results = {}
    
    for beta in beta_values:
        print(f"\nTraining VAE with β = {beta}")
        
        model = VAE(input_dim=784, latent_dim=20, hidden_dim=400)
        train_losses, test_losses, recon_losses, kl_losses = train_vae(
            model, train_loader, test_loader, num_epochs=30, beta=beta
        )
        
        results[beta] = {
            'model': model,
            'train_losses': train_losses,
            'test_losses': test_losses,
            'recon_losses': recon_losses,
            'kl_losses': kl_losses
        }
        
        print(f"β={beta} - Final train loss: {train_losses[-1]:.4f}, "
              f"Recon: {recon_losses[-1]:.4f}, KL: {kl_losses[-1]:.4f}")
    
    # Plot comparison
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    
    # Total loss
    axes[0, 0].set_title('Total Loss')
    for beta, data in results.items():
        axes[0, 0].plot(data['train_losses'], label=f'β={beta}', linewidth=2)
    axes[0, 0].set_xlabel('Epoch')
    axes[0, 0].set_ylabel('Loss')
    axes[0, 0].legend()
    axes[0, 0].grid(True)
    
    # Reconstruction loss
    axes[0, 1].set_title('Reconstruction Loss')
    for beta, data in results.items():
        axes[0, 1].plot(data['recon_losses'], label=f'β={beta}', linewidth=2)
    axes[0, 1].set_xlabel('Epoch')
    axes[0, 1].set_ylabel('Reconstruction Loss')
    axes[0, 1].legend()
    axes[0, 1].grid(True)
    
    # KL divergence
    axes[1, 0].set_title('KL Divergence')
    for beta, data in results.items():
        axes[1, 0].plot(data['kl_losses'], label=f'β={beta}', linewidth=2)
    axes[1, 0].set_xlabel('Epoch')
    axes[1, 0].set_ylabel('KL Divergence')
    axes[1, 0].legend()
    axes[1, 0].grid(True)
    
    # Final comparison
    betas = list(results.keys())
    final_recons = [results[beta]['recon_losses'][-1] for beta in betas]
    final_kls = [results[beta]['kl_losses'][-1] for beta in betas]
    
    axes[1, 1].plot(betas, final_recons, 'bo-', label='Reconstruction', linewidth=2)
    axes[1, 1].plot(betas, final_kls, 'ro-', label='KL Divergence', linewidth=2)
    axes[1, 1].set_title('Final Loss Components vs β')
    axes[1, 1].set_xlabel('β value')
    axes[1, 1].set_ylabel('Loss')
    axes[1, 1].legend()
    axes[1, 1].grid(True)
    axes[1, 1].set_xscale('log')
    
    plt.tight_layout()
    plt.show()
    
    # Show sample generations for different β values
    fig, axes = plt.subplots(len(beta_values), 4, figsize=(8, 2*len(beta_values)))
    
    for i, (beta, data) in enumerate(results.items()):
        model = data['model']
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        model = model.to(device)
        model.eval()
        
        with torch.no_grad():
            z = torch.randn(4, 20).to(device)
            generated = model.decode(z)
            generated = generated.view(4, 1, 28, 28)
        
        for j in range(4):
            axes[i, j].imshow(generated[j].cpu().squeeze(), cmap='gray')
            axes[i, j].axis('off')
            if j == 0:
                axes[i, j].set_ylabel(f'β={beta}', rotation=0, labelpad=30)
    
    plt.suptitle('Generated Samples for Different β Values')
    plt.tight_layout()
    plt.show()
    
    return results

def demonstrate_disentanglement():
    """Demonstrate disentangled representations in latent space"""
    print("\n=== Latent Space Disentanglement Demo ===")
    
    # Load data
    transform = transforms.Compose([transforms.ToTensor()])
    test_dataset = torchvision.datasets.MNIST(root='./data', train=False, transform=transform)
    test_loader = DataLoader(test_dataset, batch_size=64, shuffle=False)
    
    # Train a β-VAE with higher β for better disentanglement
    train_dataset = torchvision.datasets.MNIST(root='./data', train=True, transform=transform)
    train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True)
    
    print("Training β-VAE with β=4 for disentanglement...")
    model = ConvVAE(input_channels=1, latent_dim=10)  # Smaller latent dim for better disentanglement
    train_losses, _, _, _ = train_vae(model, train_loader, test_loader, num_epochs=20, beta=4.0)
    
    # Demonstrate latent traversal
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    model.eval()
    
    # Get a base image
    data_iter = iter(test_loader)
    data, _ = next(data_iter)
    base_image = data[0:1].to(device)
    
    with torch.no_grad():
        mu, logvar = model.encode(base_image)
        base_z = mu.clone()
        
        # Traverse each latent dimension
        fig, axes = plt.subplots(model.latent_dim, 11, figsize=(15, model.latent_dim * 1.5))
        
        for dim in range(model.latent_dim):
            for i, value in enumerate(np.linspace(-3, 3, 11)):
                z_modified = base_z.clone()
                z_modified[0, dim] = value
                
                generated = model.decode(z_modified)
                
                axes[dim, i].imshow(generated[0].cpu().squeeze(), cmap='gray')
                axes[dim, i].axis('off')
                
                if i == 5:  # Middle image
                    axes[dim, i].set_title(f'Dim {dim}')
    
    plt.suptitle('Latent Space Traversal (Each row = one latent dimension)')
    plt.tight_layout()
    plt.show()

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Variational Autoencoder (VAE) Implementation ===")
    
    # Load MNIST data
    transform = transforms.Compose([transforms.ToTensor()])
    
    train_dataset = torchvision.datasets.MNIST(root='./data', train=True, download=True, transform=transform)
    test_dataset = torchvision.datasets.MNIST(root='./data', train=False, download=True, transform=transform)
    
    train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=128, shuffle=False)
    
    print(f"Training samples: {len(train_dataset)}")
    print(f"Test samples: {len(test_dataset)}")
    
    # Train standard VAE
    print("\n=== Training Standard VAE ===")
    vae_model = VAE(input_dim=784, latent_dim=20, hidden_dim=400)
    train_losses, test_losses, recon_losses, kl_losses = train_vae(
        vae_model, train_loader, test_loader, num_epochs=30
    )
    
    # Plot training curves
    fig, axes = plt.subplots(2, 2, figsize=(12, 8))
    
    axes[0, 0].plot(train_losses, label='Train', linewidth=2)
    axes[0, 0].plot(test_losses, label='Test', linewidth=2)
    axes[0, 0].set_title('Total Loss')
    axes[0, 0].legend()
    axes[0, 0].grid(True)
    
    axes[0, 1].plot(recon_losses, 'g-', linewidth=2)
    axes[0, 1].set_title('Reconstruction Loss')
    axes[0, 1].grid(True)
    
    axes[1, 0].plot(kl_losses, 'r-', linewidth=2)
    axes[1, 0].set_title('KL Divergence')
    axes[1, 0].grid(True)
    
    axes[1, 1].plot(recon_losses, 'g-', label='Reconstruction', linewidth=2)
    axes[1, 1].plot(kl_losses, 'r-', label='KL Divergence', linewidth=2)
    axes[1, 1].set_title('Loss Components')
    axes[1, 1].legend()
    axes[1, 1].grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Visualizations
    print("\n=== VAE Visualizations ===")
    
    # Reconstructions
    visualize_reconstructions(vae_model, test_loader)
    
    # Generated samples
    generate_new_samples(vae_model, num_samples=16, latent_dim=20)
    
    # Interpolation
    interpolate_in_latent_space(vae_model, test_loader)
    
    # Latent space visualization
    visualize_latent_space(vae_model, test_loader)
    
    # Compare β values
    beta_comparison = compare_beta_values()
    
    # Demonstrate disentanglement
    demonstrate_disentanglement()
    
    print("\n=== VAE Key Concepts ===")
    print("1. PROBABILISTIC LATENT SPACE: Encode to distributions, not points")
    print("   - Encoder outputs μ and log(σ²) instead of fixed z")
    print("   - Sample z ~ N(μ, σ²) using reparameterization trick")
    
    print("\n2. REPARAMETERIZATION TRICK: Enable backprop through sampling")
    print("   - z = μ + σ ⊙ ε, where ε ~ N(0,1)")
    print("   - Allows gradient flow through stochastic sampling")
    
    print("\n3. VAE LOSS FUNCTION:")
    print("   - L = Reconstruction Loss + β × KL Divergence")
    print("   - Reconstruction: How well can we rebuild input")
    print("   - KL: How close is q(z|x) to prior p(z) = N(0,1)")
    
    print("\n4. β-VAE: Control reconstruction vs regularization")
    print("   - β > 1: More regularization, better disentanglement")
    print("   - β < 1: Better reconstruction, less regularization")
    print("   - β = 1: Standard VAE")
    
    print("\n=== VAE vs Autoencoder ===")
    print("AUTOENCODER:")
    print("• Deterministic encoding: x → z")
    print("• Hard to generate new samples")
    print("• May have 'holes' in latent space")
    
    print("\nVAE:")
    print("• Probabilistic encoding: x → p(z|x)")
    print("• Easy generation: sample z ~ p(z), decode")
    print("• Smooth, continuous latent space")
    print("• Principled framework with KL regularization")
    
    print("\n=== Applications ===")
    print("✓ Image generation and editing")
    print("✓ Data augmentation")
    print("✓ Anomaly detection")
    print("✓ Dimensionality reduction")
    print("✓ Semi-supervised learning")
    print("✓ Drug discovery (molecular generation)")
    print("✓ Text generation and style transfer")