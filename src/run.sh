python3 python/generate_graph.py
g++ -Wall -O3 -g implementations/0_search_clique.cpp -o 0_search_clique
./0_search_clique
g++ -Wall -O3 -g implementations/1_edges_heuristic.cpp -o 1_edges_heuristic
./1_edges_heuristic
g++ -Wall -O3 -g implementations/2_recursive_clique.cpp -o 2_recursive_clique
./2_recursive_clique
g++ -Wall -O3 -g implementations/3_dynamic_programming.cpp -o 3_dynamic_programming
./3_dynamic_programming
g++ -Wall -O3 -g -fopenmp implementations/4_recursive_clique_parallel.cpp -o 4_recursive_clique_parallel
./4_recursive_clique_parallel
g++ -Wall -O3 -g -fopenmp implementations/5_dynamic_programming_parallel.cpp -o 5_dynamic_programming_parallel
./5_dynamic_programming_parallel
python3 python/verify_clique.py
