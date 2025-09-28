import pandas as pd
import matplotlib.pyplot as plt

# --- Ler os CSVs ---
csv_paralelo = "maiores_tempos_paralelo_EX1.csv"
csv_processos = "maiores_tempos_processos_EX1.csv"

df_paralelo = pd.read_csv(csv_paralelo, header=None, names=["tempo_ms"])
df_processos = pd.read_csv(csv_processos, header=None, names=["tempo_ms"])

# --- Calcular médias ---
media_paralelo = df_paralelo["tempo_ms"].mean()
media_processos = df_processos["tempo_ms"].mean()

print(f"Média paralelo: {media_paralelo:.2f} ms")
print(f"Média processos: {media_processos:.2f} ms")

# --- Criar gráfico ---
plt.figure(figsize=(10,6))

# Plotar pontos individuais
plt.plot(df_paralelo.index+1, df_paralelo["tempo_ms"], marker="o", label="Paralelo")
plt.plot(df_processos.index+1, df_processos["tempo_ms"], marker="s", label="Processos")

# Linhas de média
plt.axhline(y=media_paralelo, color="blue", linestyle="--", label=f"Média Paralelo ({media_paralelo:.1f} ms)")
plt.axhline(y=media_processos, color="orange", linestyle="--", label=f"Média Processos ({media_processos:.1f} ms)")

# Configurações do gráfico
plt.title("Comparação dos Tempos - Paralelo vs Processos")
plt.xlabel("Execução")
plt.ylabel("Tempo (ms)")
plt.legend()
plt.grid(True, linestyle="--", alpha=0.6)

# Salvar e mostrar
plt.savefig("grafico_tempos.png", dpi=150)
plt.show()
