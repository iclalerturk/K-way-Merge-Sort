import pandas as pd
import matplotlib.pyplot as plt

# Load the data (adjust the filename and delimiter as needed)
# The file should have columns like 'N', 'K', and 'time'
data = pd.read_csv('output.csv')  # Replace 'your_file.csv' with your actual file path

# Group data by 'K' values and plot
plt.figure(figsize=(10, 6))
for k_value, group in data.groupby('k'):
    plt.plot(group['n'], group['sure'], marker='o', label=f'k={k_value}')

# Labeling the plot
plt.xscale('log')  # N eksenini logaritmik ölçeğe ayarla
plt.xlabel('N Değeri (Eleman Sayısı)')
plt.ylabel('Süre')
plt.title('N ve K Değerlerine Göre Süre Değişimi')
plt.legend(title='K Değeri')

plt.grid(True, which="both", linestyle='--', linewidth=0.5)

# Grafiği gösterme
plt.show()
# Veriyi yükleme (dosya adını ve ayırıcısını ihtiyaca göre ayarlayın)

# Her N değeri için ayrı grafik oluşturma
n_values = data['n'].unique()

# Grafik sayfası ve alt grafikler için yapı (3 sütunlu, yeterli satır sayısı)
n_cols = 3
n_rows = 2  # Satır sayısını ayarla

fig, axes = plt.subplots(n_rows, n_cols, figsize=(10 * n_cols, 6 * n_rows))

# Eğer yalnızca bir grafik varsa, axes'ı tek bir öğeye dönüştür
axes = axes.flatten() if len(n_values) > 1 else [axes]

for ax, n_val in zip(axes, n_values):
    # N değeri için filtreleme
    n_data = data[data['n'] == n_val]
    
    # Aynı n ve k değerlerine sahip sürelerin ortalamasını al
    n_data_avg = n_data.groupby(['n', 'k'], as_index=False)['sure'].mean()

    # Grafik oluşturma
    ax.plot(n_data_avg['k'], n_data_avg['sure'], marker='o', label=f'N={n_val}')
    
    # Grafik etiketleme
    #ax.set_xlabel('k Değeri')
    ax.set_ylabel('Süre')
    ax.set_title(f'N={n_val} için K ve Süre Değişimi')
    ax.legend(title='K Değeri')

    ax.grid(True, which="both", linestyle='--', linewidth=0.5)

plt.subplots_adjust(wspace=300, hspace=500)

# Eğer eksik alt grafik varsa, onları boş bırak
for i in range(len(n_values), len(axes)):
    fig.delaxes(axes[i])

# Tüm grafikleri aynı sayfada göster
plt.tight_layout()
plt.show()

