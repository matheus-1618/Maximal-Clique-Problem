python3 generate_graph.py
g++ -Wall -O3 -g get_matrix.cpp -o get_matrix
./get_matrix
g++ -Wall -O3 -g first_heuristics.cpp -o first_heuristics
./first_heuristics
python3 verify_clique.py
