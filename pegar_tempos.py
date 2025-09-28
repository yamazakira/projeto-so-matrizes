import os
import glob
import pandas as pd

def pegar_maiores_tempos(pasta, csv_saida):
    arquivos = glob.glob(os.path.join(pasta, "*.csv"), recursive=True)
    tempos = []

    for arq in arquivos:
        try:
            with open(arq, "r") as f:
                ultima_linha = f.readlines()[-1].strip()

            tempo = float(ultima_linha)  # garante que seja número
            tempos.append(tempo)

        except Exception as e:
            print(f"Atenção: não foi possível ler {arq}: {e}")

    if tempos:
        maior_tempo = max(tempos)
        # Salvar no CSV, adicionando no final se existir
        df_novo = pd.DataFrame([maior_tempo])
        if os.path.exists(csv_saida):
            df_novo.to_csv(csv_saida, mode='a', header=False, index=False)
        else:
            df_novo.to_csv(csv_saida, index=False)
        print(f"Maior tempo salvo em {csv_saida}: {maior_tempo} ms")
    else:
        print(f"Nenhum tempo encontrado na pasta {pasta}")


# Pastas de entrada e arquivos de saída
pasta_paralelo = "resultados/paralelo/3200x3200"
pasta_processos = "resultados/processos/3200x3200"

csv_saida_paralelo = "maiores_tempos_paralelo_EX1.csv"
csv_saida_processos = "maiores_tempos_processos_EX1.csv"

# pegar_maiores_tempos(pasta_paralelo, csv_saida_paralelo)
pegar_maiores_tempos(pasta_processos, csv_saida_processos)
