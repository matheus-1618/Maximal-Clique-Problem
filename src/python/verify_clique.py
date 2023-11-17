import networkx as nx
import os
# Nome do arquivo de entrada
nome_arquivo = "implementations/graph.txt"

# Abrir o arquivo e pular a primeira linha
with open(nome_arquivo, 'r') as arquivo:
    next(arquivo)  # Pula a primeira linha

    # Lê o grafo a partir das linhas restantes
    G = nx.parse_adjlist(arquivo)

# Encontrar todas as cliques maximais
cliques_maximais = list(nx.find_cliques(G))

# Encontrar a clique máxima (a maior)
clique_maxima = max(cliques_maximais, key=len)

# print("Cliques maximais encontradas:")
# for clique in cliques_maximais:
#     print(sorted(map(int,clique)))

print(f"[Verification] Clique's Size: {len(clique_maxima)} Maximal Clique: ", " ".join(map(str,sorted(map(int,clique_maxima)))))