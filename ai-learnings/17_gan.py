# author: csgod cmps studies
# GAN - Generative Adversarial Networks
# GANs use two competing neural networks (generator and discriminator) in a minimax game
# to learn to generate realistic data through adversarial training.

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.utils.data import DataLoader
import torchvision
import torchvision.transforms as transforms
import torchvision.utils as vutils
import matplotlib.pyplot as plt
import numpy as np

class Generator(nn.Module):
    """Generator network: maps noise to fake data"""
    def __init__(self, noise_dim=100, output_dim=784, hidden_dim=256):
        super(Generator, self).__init__()
        
        self.model = nn.Sequential(
            # First layer
            nn.Linear(noise_dim, hidden_dim),
            nn.ReLU(True),
            
            # Second layer
            nn.Linear(hidden_dim, hidden_dim * 2),
            nn.BatchNorm1d(hidden_dim * 2),
            nn.ReLU(True),
            
            # Third layer
            nn.Linear(hidden_dim * 2, hidden_dim * 4),
            nn.BatchNorm1d(hidden_dim * 4),
            nn.ReLU(True),
            
            # Output layer
            nn.Linear(hidden_dim * 4, output_dim),
            nn.Tanh()  # Output in range [-1, 1]
        )
    
    def forward(self, z):
        return self.model(z)

class Discriminator(nn.Module):
    """Discriminator network: distinguishes real from fake data"""
    def __init__(self, input_dim=784, hidden_dim=256):
        super(Discriminator, self).__init__()
        
        self.model = nn.Sequential(
            # First layer
            nn.Linear(input_dim, hidden_dim * 4),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Dropout(0.3),
            
            # Second layer
            nn.Linear(hidden_dim * 4, hidden_dim * 2),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Dropout(0.3),
            
            # Third layer
            nn.Linear(hidden_dim * 2, hidden_dim),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Dropout(0.3),
            
            # Output layer
            nn.Linear(hidden_dim, 1),
            nn.Sigmoid()  # Output probability [0, 1]
        )
    
    def forward(self, x):
        return self.model(x)

class DCGAN_Generator(nn.Module):
    """Deep Convolutional GAN Generator"""
    def __init__(self, noise_dim=100, channels=1, feature_maps=64):
        super(DCGAN_Generator, self).__init__()
        self.noise_dim = noise_dim
        
        self.main = nn.Sequential(
            # Input: noise_dim -> 1024 (4x4x64*4)
            nn.ConvTranspose2d(noise_dim, feature_maps * 8, 4, 1, 0, bias=False),
            nn.BatchNorm2d(feature_maps * 8),
            nn.ReLU(True),
            
            # 4x4 -> 8x8
            nn.ConvTranspose2d(feature_maps * 8, feature_maps * 4, 4, 2, 1, bias=False),
            nn.BatchNorm2d(feature_maps * 4),
            nn.ReLU(True),
            
            # 8x8 -> 16x16
            nn.ConvTranspose2d(feature_maps * 4, feature_maps * 2, 4, 2, 1, bias=False),
            nn.BatchNorm2d(feature_maps * 2),
            nn.ReLU(True),
            
            # 16x16 -> 32x32 (we'll crop to 28x28 for MNIST)
            nn.ConvTranspose2d(feature_maps * 2, feature_maps, 4, 2, 1, bias=False),
            nn.BatchNorm2d(feature_maps),
            nn.ReLU(True),
            
            # 32x32 -> 32x32 (final layer)
            nn.ConvTranspose2d(feature_maps, channels, 4, 2, 1, bias=False),
            nn.Tanh()
        )
    
    def forward(self, z):
        # Reshape noise to (batch_size, noise_dim, 1, 1)
        z = z.view(z.size(0), z.size(1), 1, 1)
        output = self.main(z)
        
        # Crop to 28x28 for MNIST
        if output.size(-1) > 28:
            output = output[:, :, 2:30, 2:30]  # Crop from 32x32 to 28x28
        
        return output

class DCGAN_Discriminator(nn.Module):
    """Deep Convolutional GAN Discriminator"""
    def __init__(self, channels=1, feature_maps=64):
        super(DCGAN_Discriminator, self).__init__()
        
        self.main = nn.Sequential(
            # Input: 28x28 or 32x32
            nn.Conv2d(channels, feature_maps, 4, 2, 1, bias=False),
            nn.LeakyReLU(0.2, inplace=True),
            
            # 14x14 -> 7x7
            nn.Conv2d(feature_maps, feature_maps * 2, 4, 2, 1, bias=False),
            nn.BatchNorm2d(feature_maps * 2),
            nn.LeakyReLU(0.2, inplace=True),
            
            # 7x7 -> 3x3
            nn.Conv2d(feature_maps * 2, feature_maps * 4, 4, 2, 1, bias=False),
            nn.BatchNorm2d(feature_maps * 4),
            nn.LeakyReLU(0.2, inplace=True),
            
            # 3x3 -> 1x1 (global)
            nn.Conv2d(feature_maps * 4, 1, 3, 1, 0, bias=False),
            nn.Sigmoid()
        )
    
    def forward(self, x):
        output = self.main(x)
        return output.view(output.size(0), -1)

def weights_init(m):
    """Initialize network weights"""
    classname = m.__class__.__name__
    if classname.find('Conv') != -1:
        nn.init.normal_(m.weight.data, 0.0, 0.02)
    elif classname.find('BatchNorm') != -1:
        nn.init.normal_(m.weight.data, 1.0, 0.02)
        nn.init.constant_(m.bias.data, 0)

def train_gan(generator, discriminator, train_loader, num_epochs=50, lr=0.0002, 
              noise_dim=100, device=None, gan_type='basic'):
    """Train GAN using adversarial loss"""
    if device is None:
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    generator = generator.to(device)
    discriminator = discriminator.to(device)
    
    # Initialize weights
    generator.apply(weights_init)
    discriminator.apply(weights_init)
    
    # Loss function
    criterion = nn.BCELoss()
    
    # Optimizers
    optimizer_G = optim.Adam(generator.parameters(), lr=lr, betas=(0.5, 0.999))
    optimizer_D = optim.Adam(discriminator.parameters(), lr=lr, betas=(0.5, 0.999))
    
    # Training history
    G_losses = []
    D_losses = []
    
    # Fixed noise for tracking progress
    fixed_noise = torch.randn(64, noise_dim).to(device)
    
    print(f"Training {gan_type.upper()} GAN...")
    print(f"Generator parameters: {sum(p.numel() for p in generator.parameters()):,}")
    print(f"Discriminator parameters: {sum(p.numel() for p in discriminator.parameters()):,}")
    
    for epoch in range(num_epochs):
        for batch_idx, (real_data, _) in enumerate(train_loader):
            batch_size = real_data.size(0)
            
            # Prepare data
            if gan_type == 'basic':
                real_data = real_data.view(batch_size, -1).to(device)
            else:  # DCGAN
                real_data = real_data.to(device)
            
            real_labels = torch.ones(batch_size, 1).to(device)
            fake_labels = torch.zeros(batch_size, 1).to(device)
            
            # =====================================
            # Train Discriminator: max log(D(x)) + log(1 - D(G(z)))
            # =====================================
            discriminator.zero_grad()
            
            # Train on real data
            output_real = discriminator(real_data)
            loss_D_real = criterion(output_real, real_labels)
            
            # Train on fake data
            noise = torch.randn(batch_size, noise_dim).to(device)
            fake_data = generator(noise)
            output_fake = discriminator(fake_data.detach())  # Detach to avoid training G
            loss_D_fake = criterion(output_fake, fake_labels)
            
            # Total discriminator loss
            loss_D = loss_D_real + loss_D_fake
            loss_D.backward()
            optimizer_D.step()
            
            # =====================================
            # Train Generator: max log(D(G(z)))
            # =====================================
            generator.zero_grad()
            
            # Generate fake data and get discriminator's opinion
            output_fake = discriminator(fake_data)
            loss_G = criterion(output_fake, real_labels)  # Want discriminator to think it's real
            
            loss_G.backward()
            optimizer_G.step()
            
            # Save losses
            G_losses.append(loss_G.item())
            D_losses.append(loss_D.item())
            
            # Print statistics
            if batch_idx % 100 == 0:
                print(f'Epoch [{epoch+1}/{num_epochs}] Batch [{batch_idx}/{len(train_loader)}] '
                      f'Loss_D: {loss_D.item():.4f} Loss_G: {loss_G.item():.4f} '
                      f'D(x): {output_real.mean().item():.4f} D(G(z)): {output_fake.mean().item():.4f}')
        
        # Generate samples for visualization
        if epoch % 10 == 0:
            generator.eval()
            with torch.no_grad():
                fake_samples = generator(fixed_noise)
                
                # Save sample images
                if gan_type == 'basic':
                    fake_samples = fake_samples.view(-1, 1, 28, 28)
                
                print(f'Epoch [{epoch+1}/{num_epochs}] - Generated samples saved')
            generator.train()
    
    return G_losses, D_losses, fixed_noise

def visualize_training_progress(generator, fixed_noise, gan_type='basic', num_samples=16):
    """Visualize generated samples"""
    generator.eval()
    device = next(generator.parameters()).device
    
    with torch.no_grad():
        fake_samples = generator(fixed_noise[:num_samples])
        
        if gan_type == 'basic':
            fake_samples = fake_samples.view(-1, 1, 28, 28)
        
        # Denormalize from [-1, 1] to [0, 1]
        fake_samples = (fake_samples + 1) / 2
        
        # Plot samples
        fig, axes = plt.subplots(4, 4, figsize=(8, 8))
        for i, ax in enumerate(axes.flat):
            ax.imshow(fake_samples[i].cpu().squeeze(), cmap='gray')
            ax.axis('off')
        
        plt.suptitle(f'Generated Samples ({gan_type.upper()} GAN)')
        plt.tight_layout()
        plt.show()

def plot_losses(G_losses, D_losses):
    """Plot training losses"""
    plt.figure(figsize=(12, 5))
    
    # Loss curves
    plt.subplot(1, 2, 1)
    plt.plot(G_losses, label='Generator', alpha=0.7)
    plt.plot(D_losses, label='Discriminator', alpha=0.7)
    plt.title('Training Losses')
    plt.xlabel('Iteration')
    plt.ylabel('Loss')
    plt.legend()
    plt.grid(True)
    
    # Smoothed losses (moving average)
    plt.subplot(1, 2, 2)
    window_size = 50
    if len(G_losses) > window_size:
        G_smooth = np.convolve(G_losses, np.ones(window_size)/window_size, mode='valid')
        D_smooth = np.convolve(D_losses, np.ones(window_size)/window_size, mode='valid')
        plt.plot(G_smooth, label='Generator (smoothed)', linewidth=2)
        plt.plot(D_smooth, label='Discriminator (smoothed)', linewidth=2)
    else:
        plt.plot(G_losses, label='Generator')
        plt.plot(D_losses, label='Discriminator')
    
    plt.title('Smoothed Training Losses')
    plt.xlabel('Iteration')
    plt.ylabel('Loss')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.show()

def compare_real_vs_fake(generator, test_loader, noise_dim=100, gan_type='basic'):
    """Compare real and generated samples side by side"""
    device = next(generator.parameters()).device
    generator.eval()
    
    # Get real samples
    real_batch = next(iter(test_loader))[0][:8]
    
    # Generate fake samples
    with torch.no_grad():
        noise = torch.randn(8, noise_dim).to(device)
        fake_batch = generator(noise)
        
        if gan_type == 'basic':
            fake_batch = fake_batch.view(-1, 1, 28, 28)
        
        # Denormalize
        fake_batch = (fake_batch + 1) / 2
    
    # Plot comparison
    fig, axes = plt.subplots(2, 8, figsize=(16, 4))
    
    # Real samples
    for i in range(8):
        axes[0, i].imshow(real_batch[i].squeeze(), cmap='gray')
        axes[0, i].set_title('Real')
        axes[0, i].axis('off')
    
    # Fake samples
    for i in range(8):
        axes[1, i].imshow(fake_batch[i].cpu().squeeze(), cmap='gray')
        axes[1, i].set_title('Generated')
        axes[1, i].axis('off')
    
    plt.suptitle(f'Real vs Generated Samples ({gan_type.upper()} GAN)')
    plt.tight_layout()
    plt.show()

def interpolate_in_latent_space(generator, noise_dim=100, gan_type='basic', num_steps=8):
    """Interpolate between two points in noise space"""
    device = next(generator.parameters()).device
    generator.eval()
    
    with torch.no_grad():
        # Two random noise vectors
        z1 = torch.randn(1, noise_dim).to(device)
        z2 = torch.randn(1, noise_dim).to(device)
        
        # Interpolate
        interpolations = []
        for i in range(num_steps):
            alpha = i / (num_steps - 1)
            z_interp = (1 - alpha) * z1 + alpha * z2
            
            fake_sample = generator(z_interp)
            if gan_type == 'basic':
                fake_sample = fake_sample.view(1, 1, 28, 28)
            
            # Denormalize
            fake_sample = (fake_sample + 1) / 2
            interpolations.append(fake_sample.cpu())
    
    # Plot interpolation
    plt.figure(figsize=(12, 3))
    for i, sample in enumerate(interpolations):
        plt.subplot(1, num_steps, i + 1)
        plt.imshow(sample.squeeze(), cmap='gray')
        plt.title(f'α={i/(num_steps-1):.2f}')
        plt.axis('off')
    
    plt.suptitle(f'Latent Space Interpolation ({gan_type.upper()} GAN)')
    plt.tight_layout()
    plt.show()

def analyze_mode_collapse(generator, noise_dim=100, num_samples=100, gan_type='basic'):
    """Analyze potential mode collapse by examining sample diversity"""
    device = next(generator.parameters()).device
    generator.eval()
    
    with torch.no_grad():
        # Generate many samples
        noise = torch.randn(num_samples, noise_dim).to(device)
        fake_samples = generator(noise)
        
        if gan_type == 'basic':
            fake_samples = fake_samples.view(num_samples, -1)
        else:
            fake_samples = fake_samples.view(num_samples, -1)
        
        # Calculate pairwise distances
        samples_np = fake_samples.cpu().numpy()
        
        # Compute diversity metrics
        from scipy.spatial.distance import pdist
        distances = pdist(samples_np, metric='euclidean')
        
        mean_distance = np.mean(distances)
        std_distance = np.std(distances)
        min_distance = np.min(distances)
        
        print(f"\n=== Mode Collapse Analysis ({gan_type.upper()} GAN) ===")
        print(f"Mean pairwise distance: {mean_distance:.4f}")
        print(f"Std pairwise distance: {std_distance:.4f}")
        print(f"Min pairwise distance: {min_distance:.4f}")
        print(f"Diversity ratio (std/mean): {std_distance/mean_distance:.4f}")
        
        # Plot distance histogram
        plt.figure(figsize=(10, 6))
        plt.hist(distances, bins=50, alpha=0.7, density=True)
        plt.axvline(mean_distance, color='red', linestyle='--', 
                   label=f'Mean: {mean_distance:.2f}')
        plt.title(f'Pairwise Distance Distribution ({gan_type.upper()} GAN)')
        plt.xlabel('Euclidean Distance')
        plt.ylabel('Density')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.show()

def train_and_compare_gans():
    """Train and compare basic GAN vs DCGAN"""
    print("=== Training and Comparing GANs ===")
    
    # Load MNIST data
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.5,), (0.5,))  # Normalize to [-1, 1]
    ])
    
    train_dataset = torchvision.datasets.MNIST(
        root='./data', train=True, download=True, transform=transform
    )
    test_dataset = torchvision.datasets.MNIST(
        root='./data', train=False, download=True, transform=transform
    )
    
    train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True, drop_last=True)
    test_loader = DataLoader(test_dataset, batch_size=128, shuffle=False)
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"Using device: {device}")
    
    # Train Basic GAN
    print("\n" + "="*50)
    print("TRAINING BASIC GAN")
    print("="*50)
    
    basic_gen = Generator(noise_dim=100, output_dim=784, hidden_dim=256)
    basic_disc = Discriminator(input_dim=784, hidden_dim=256)
    
    G_losses_basic, D_losses_basic, fixed_noise_basic = train_gan(
        basic_gen, basic_disc, train_loader, num_epochs=30, 
        noise_dim=100, device=device, gan_type='basic'
    )
    
    # Train DCGAN
    print("\n" + "="*50)
    print("TRAINING DCGAN")
    print("="*50)
    
    dcgan_gen = DCGAN_Generator(noise_dim=100, channels=1, feature_maps=64)
    dcgan_disc = DCGAN_Discriminator(channels=1, feature_maps=64)
    
    G_losses_dcgan, D_losses_dcgan, fixed_noise_dcgan = train_gan(
        dcgan_gen, dcgan_disc, train_loader, num_epochs=30,
        noise_dim=100, device=device, gan_type='dcgan'
    )
    
    # Visualizations and comparisons
    print("\n" + "="*50)
    print("RESULTS AND ANALYSIS")
    print("="*50)
    
    # Plot losses
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    
    # Basic GAN losses
    axes[0, 0].plot(G_losses_basic, label='Generator', alpha=0.7)
    axes[0, 0].plot(D_losses_basic, label='Discriminator', alpha=0.7)
    axes[0, 0].set_title('Basic GAN Losses')
    axes[0, 0].legend()
    axes[0, 0].grid(True)
    
    # DCGAN losses
    axes[0, 1].plot(G_losses_dcgan, label='Generator', alpha=0.7)
    axes[0, 1].plot(D_losses_dcgan, label='Discriminator', alpha=0.7)
    axes[0, 1].set_title('DCGAN Losses')
    axes[0, 1].legend()
    axes[0, 1].grid(True)
    
    # Loss comparison
    window = 50
    if len(G_losses_basic) > window:
        G_basic_smooth = np.convolve(G_losses_basic, np.ones(window)/window, mode='valid')
        G_dcgan_smooth = np.convolve(G_losses_dcgan, np.ones(window)/window, mode='valid')
        
        axes[1, 0].plot(G_basic_smooth, label='Basic GAN', linewidth=2)
        axes[1, 0].plot(G_dcgan_smooth, label='DCGAN', linewidth=2)
        axes[1, 0].set_title('Generator Loss Comparison (Smoothed)')
        axes[1, 0].legend()
        axes[1, 0].grid(True)
        
        D_basic_smooth = np.convolve(D_losses_basic, np.ones(window)/window, mode='valid')
        D_dcgan_smooth = np.convolve(D_losses_dcgan, np.ones(window)/window, mode='valid')
        
        axes[1, 1].plot(D_basic_smooth, label='Basic GAN', linewidth=2)
        axes[1, 1].plot(D_dcgan_smooth, label='DCGAN', linewidth=2)
        axes[1, 1].set_title('Discriminator Loss Comparison (Smoothed)')
        axes[1, 1].legend()
        axes[1, 1].grid(True)
    
    plt.tight_layout()
    plt.show()
    
    # Show generated samples
    visualize_training_progress(basic_gen, fixed_noise_basic, 'basic', 16)
    visualize_training_progress(dcgan_gen, fixed_noise_dcgan, 'dcgan', 16)
    
    # Real vs fake comparisons
    compare_real_vs_fake(basic_gen, test_loader, 100, 'basic')
    compare_real_vs_fake(dcgan_gen, test_loader, 100, 'dcgan')
    
    # Interpolations
    interpolate_in_latent_space(basic_gen, 100, 'basic')
    interpolate_in_latent_space(dcgan_gen, 100, 'dcgan')
    
    # Mode collapse analysis
    analyze_mode_collapse(basic_gen, 100, 100, 'basic')
    analyze_mode_collapse(dcgan_gen, 100, 100, 'dcgan')
    
    return {
        'basic_gen': basic_gen,
        'basic_disc': basic_disc,
        'dcgan_gen': dcgan_gen,
        'dcgan_disc': dcgan_disc,
        'basic_losses': (G_losses_basic, D_losses_basic),
        'dcgan_losses': (G_losses_dcgan, D_losses_dcgan)
    }

# Example usage and demonstration
if __name__ == "__main__":
    print("=== Generative Adversarial Networks (GANs) ===")
    
    # Train and compare different GAN architectures
    results = train_and_compare_gans()
    
    print("\n=== GAN Key Concepts ===")
    print("1. ADVERSARIAL TRAINING: Two networks compete in a minimax game")
    print("   - Generator: G(z) tries to fool discriminator")
    print("   - Discriminator: D(x) tries to distinguish real from fake")
    
    print("\n2. MINIMAX OBJECTIVE:")
    print("   min_G max_D V(D,G) = E[log D(x)] + E[log(1 - D(G(z)))]")
    print("   - D maximizes: wants to correctly classify real vs fake")
    print("   - G minimizes: wants to fool D into thinking fake is real")
    
    print("\n3. TRAINING DYNAMICS:")
    print("   - Alternating optimization: train D, then train G")
    print("   - Nash equilibrium: G generates realistic data, D cannot distinguish")
    print("   - Training instability: balance between G and D is crucial")
    
    print("\n4. GENERATOR TRAINING:")
    print("   - Input: Random noise z ~ p(z)")
    print("   - Output: Fake samples G(z)")
    print("   - Loss: Want D(G(z)) to be high (fool discriminator)")
    
    print("\n5. DISCRIMINATOR TRAINING:")
    print("   - Input: Real data x and fake data G(z)")
    print("   - Output: Probability of input being real")
    print("   - Loss: Maximize D(x) and minimize D(G(z))")
    
    print("\n=== GAN Variants ===")
    print("• BASIC GAN: Fully connected networks")
    print("• DCGAN: Deep convolutional architecture")
    print("• WGAN: Wasserstein distance instead of JS divergence")
    print("• StyleGAN: Style-based generator architecture")
    print("• CycleGAN: Unpaired image-to-image translation")
    print("• ProGAN: Progressive growing of GANs")
    
    print("\n=== Common Challenges ===")
    print("❌ MODE COLLAPSE: Generator produces limited diversity")
    print("❌ TRAINING INSTABILITY: Sensitive hyperparameters")
    print("❌ VANISHING GRADIENTS: Poor generator gradients")
    print("❌ EVALUATION: Hard to measure generation quality")
    print("❌ CONVERGENCE: No clear stopping criterion")
    
    print("\n=== Solutions & Improvements ===")
    print("✓ Batch normalization for training stability")
    print("✓ LeakyReLU instead of ReLU in discriminator")
    print("✓ Label smoothing and noise injection")
    print("✓ Feature matching and minibatch discrimination")
    print("✓ Spectral normalization for Lipschitz constraint")
    print("✓ Progressive growing for high-resolution images")
    
    print("\n=== Applications ===")
    print("• Image generation and editing")
    print("• Data augmentation")
    print("• Super-resolution")
    print("• Style transfer")
    print("• Domain adaptation")
    print("• Anomaly detection")
    print("• Drug discovery")
    print("• Art and creative applications")
    
    print("\n=== GAN vs VAE ===")
    print("GAN ADVANTAGES:")
    print("• Sharp, realistic samples")
    print("• No explicit likelihood model needed")
    print("• Can generate diverse samples")
    
    print("\nGAN DISADVANTAGES:")
    print("• Training instability")
    print("• Mode collapse")
    print("• Hard to evaluate")
    
    print("\nVAE ADVANTAGES:")
    print("• Stable training")
    print("• Principled probabilistic framework")
    print("• Good latent space structure")
    
    print("\nVAE DISADVANTAGES:")
    print("• Blurry samples")
    print("• Strong independence assumptions")