# Eight Puzzle

Please compile this program using g++. 
- g++ solve_puzzle.cpp -o solve_puzzle

When running the executable, you will need 10 arguments in the following form:
- ./solve_puzzle a b c d e f g h i heuristic

Arguments a-i create the 8-puzzle, and the heuristic value determines which kind of heuristic function we are using.
- For the A* algorithm with the Manhattan Distance heuristic, type 1.
- For the A* algorithm with the Misplaced Tile heuristic, type 2.
- For the Uniform Cost blind search algorithm, type 3.

The program will return the following information:
- Program tracing from initial state to goal state
- Depth of the solution
- Total nodes expanded
- Maximum queue size
- Nodes currently in fronter
- Search runtime
