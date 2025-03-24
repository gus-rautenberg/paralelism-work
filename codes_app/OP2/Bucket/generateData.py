import numpy as np

quantidades = [150000]

arquivos = {}
for qtd in quantidades:
    valores = np.random.rand(qtd)
    nome_arquivo = f"data_{qtd}.txt"
    with open(nome_arquivo, "w") as f:
        for valor in valores:
            f.write(f"{valor:.4f}\n")
    arquivos[qtd] = nome_arquivo

arquivos
