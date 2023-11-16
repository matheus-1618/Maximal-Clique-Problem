
import networkx as nx
import matplotlib.pyplot as plt
import random
import sys

plot = False
if len(sys.argv) == 1:
    print("Usage: python script.py <number_of_vertex> <plot[1 to plot]> ...")
    sys.exit(1)  # Exit with an error code
if len(sys.argv) > 2:
    plot = bool(sys.argv[2])

# Parâmetros
num_vertices = sys.argv[1]  # Número de vértices no grafo
probabilidade_conexao = 0.7  # Probabilidade de haver uma aresta entre dois vértices (ajuste conforme necessário)

# Crie um grafo aleatório densamente conectado
grafo = nx.fast_gnp_random_graph(num_vertices, probabilidade_conexao)

# Nome do arquivo de saída
nome_arquivo = "implementations/graph.txt"
# Abra o arquivo para escrita
with open(nome_arquivo, 'w') as arquivo:
    # Escreva a quantidade de vértices e número de arestas na primeira linha
    arquivo.write(f"{num_vertices} {grafo.number_of_edges()}\n")

    # Escreva as arestas no formato de lista de adjacência
    for aresta in grafo.edges():
        arquivo.write(f"{aresta[0]+1} {aresta[1]+1}\n")  # +1 para ajustar os índices (começando em 1)

print(f"Grafo densamente conectado gerado e salvo em '{nome_arquivo}'.")

if plot:
    # Plot the graph
    pos = nx.spring_layout(grafo)  # Layout algorithm (you can choose other layouts)
    nx.draw(grafo, pos, with_labels=True, font_weight='bold', node_size=700, node_color='skyblue', font_color='black', font_size=10, edge_color='gray', linewidths=1, alpha=0.7)

    # Show the plot
    plt.show()
