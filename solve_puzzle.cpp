#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

// Return the i and j values of a given tile (including empty space) in the 8-puzzle
// 00 01 02
// 10 11 12
// 20 21 22
std::pair<int, int> find_tile(const std::vector<std::vector<int>>& puzzle, int tile) {
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(1).size(); j++) {
            if (puzzle.at(i).at(j) == tile) {
                return {i, j};
            }
        }
    }

    return {-1, -1};
}

// h(n) function: returns different values depending on which heuristic strategy to use
int heuristic(const std::vector<std::vector<int>>& puzzle, int strategy) {
    const std::vector<std::vector<int>> solution = {{1, 2, 3}, 
                                                    {4, 5, 6}, 
                                                    {7, 8, 0}};
    // Manhattan Distance
    if (strategy == 1) {
        int count = 0;
        for (unsigned i = 1; i <= 8; i++) {
            std::pair p = find_tile(puzzle, i);
            std::pair s = find_tile(solution, i);
            count += abs(p.first-s.first) + abs(p.second-s.second);
        }
        return count;
    } 
    // Misplaced Tile
    else if (strategy == 2) {
        int count = 0;
        for (int i = 0; i < puzzle.size(); i++) {
            for (int j = 0; j < puzzle.at(0).size(); j++) { 
                if (puzzle.at(i).at(j) != solution.at(i).at(j) && puzzle.at(i).at(j) != 0) {
                    count++;
                }
            }
        }
        return count;
    }

    // Uniform Cost
    return 0;
}

// Structure to store puzzle, depth g(n), and heuristic h(n)
struct node {
    std::vector<std::vector<int>> puzzle;
    int depth;
    int strategy;
    int heuristic_val;
    node(std::vector<std::vector<int>> puzzle, int depth, char strategy) : puzzle(puzzle), strategy(strategy), depth(depth) {
        heuristic_val = heuristic(puzzle, strategy);
    }
    // Function to compare A* values
    bool operator>(const node second) const {
        // If n1's A* > n2 A*
        if (this->depth + this->heuristic_val > second.depth + second.heuristic_val) {
            return true;
        }
        // If both A* values are equal, prioritize the node with least depth
        else if (this->depth + this->heuristic_val == second.depth + second.heuristic_val && this->depth > second.depth) {
            return true;
        }

        return false;
    }
};

void print(const std::vector<std::vector<int>>& puzzle) {
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(0).size(); j++) {
            if (puzzle.at(i).at(j) == 0) {
                std::cout << "  ";
            }
            else {
                std::cout << puzzle.at(i).at(j) << " ";
            }
        }
        std::cout << std::endl;
    }
}

// Hashing function to put into unique node: if we find a number that's already in the vector, we found a repeated state
int encode(const std::vector<std::vector<int>>& puzzle) {
    int sum = 0;
    int factor = 100000000;

    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(0).size(); j++) {
            sum += puzzle.at(i).at(j)*factor;
            factor /= 10;
        }
    }

    return sum;
}

// General searching function
void search(node start, const std::vector<std::vector<int>>& solution) {
    // Store unexplored nodes in queueing function, keep track of already visited nodes using hashmap
    std::priority_queue<node, std::vector<node>, std::greater<node>> queue;
    std::unordered_map<int, std::vector<std::vector<int>>> unique_nodes;
    int max_size = queue.size();

    // Push the initial state as the first node
    queue.push(start);
    unique_nodes[encode(start.puzzle)] = start.puzzle;

    // Main loop: if we break out of the loop, something went wrong.
    while (queue.size() != 0) {

        std::cout << "Current Puzzle State:" << std::endl;

        print(queue.top().puzzle);

        std::cout << "g(n) = " << queue.top().depth << ", h(n) = " << queue.top().heuristic_val << std::endl << std::endl;

        // Keeps track of the largest queue size we had during search
        if (max_size < queue.size()) {
            max_size = queue.size();
        }

        // If solution found, return depth of goal node, nodes expanded, max queue, and nodes in fronter
        if (queue.top().puzzle == solution) {
            std::cout << "Solution depth:\t" << queue.top().depth << std::endl;
            std::cout << "Nodes expanded:\t" << unique_nodes.size() << std::endl;
            std::cout << "Max queue size:\t" << max_size << std::endl;
            std::cout << "Fronter nodes:\t" << queue.size() << std::endl;
            return;
        }

        node curr = queue.top();
        queue.pop();

        // Rest of the code is dedicated to node expanding
        std::pair<int, int> space = find_tile(curr.puzzle, 0);
        
        // Search top: if a tile exists above the space, swap
        if (space.first - 1 >= 0) {
            std::vector<std::vector<int>> top_puzzle = curr.puzzle;
            std::swap(top_puzzle.at(space.first).at(space.second), top_puzzle.at(space.first - 1).at(space.second));
            node top(top_puzzle, curr.depth + 1, curr.strategy);

            // Check if the created node is already reached
            int key = encode(top.puzzle);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = top.puzzle;
                queue.push(top);
            }
        }

        // Search bottom: if a tile exists below the space, swap
        if (space.first + 1 < curr.puzzle.size()) {
            std::vector<std::vector<int>> bottom_puzzle = curr.puzzle;
            std::swap(bottom_puzzle.at(space.first).at(space.second), bottom_puzzle.at(space.first + 1).at(space.second));
            node bottom(bottom_puzzle, curr.depth + 1, curr.strategy);

            // Check if the created node is already reached
            int key = encode(bottom.puzzle);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = bottom.puzzle;
                queue.push(bottom);
            }
        }

        // Search left: if a tile exists left of the space, swap
        if (space.second - 1 >= 0) {
            std::vector<std::vector<int>> left_puzzle = curr.puzzle;
            std::swap(left_puzzle.at(space.first).at(space.second), left_puzzle.at(space.first).at(space.second - 1));
            node left(left_puzzle, curr.depth + 1, curr.strategy);

            // Check if the created node is already reached
            int key = encode(left.puzzle);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = left.puzzle;
                queue.push(left);
            }
        }

        // Search right: if a tile exists right of the space, swap
        if (space.second + 1 < curr.puzzle.size()) {
            std::vector<std::vector<int>> right_puzzle = curr.puzzle;
            std::swap(right_puzzle.at(space.first).at(space.second), right_puzzle.at(space.first).at(space.second + 1));
            node right(right_puzzle, curr.depth + 1, curr.strategy);

            // Check if the created node is already reached
            int key = encode(right.puzzle);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = right.puzzle;
                queue.push(right);
            }
        }
    }

    // If reaching this point, we reached the failure state
    std::cout << "Solution not found." << std::endl;
}

int main(int arg, char* argv[]) {
    // Input 9 values (numbers from 0-8, no repeats)
    std::vector<std::vector<int>> puzzle = {{std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3])},
                                            {std::stoi(argv[4]), std::stoi(argv[5]), std::stoi(argv[6])},
                                            {std::stoi(argv[7]), std::stoi(argv[8]), std::stoi(argv[9])}};
    
    // Solution matrix (for 8-puzzle)
    const std::vector<std::vector<int>> solution = {{1, 2, 3}, 
                                                    {4, 5, 6}, 
                                                    {7, 8, 0}};

    // input 1 for manhattan, 2 for misplaced, 3 (or anything else) for uniform
    int strategy = std::stoi(argv[10]);

    std::cout << "----------------------------------------------------------" << std::endl;

    if (strategy == 1) {
        std::cout << "STRATEGY: Manhattan Distance Heuristic" << std::endl;
    }
    else if (strategy == 2) {
        std::cout << "STRATEGY: Misplaced Tiles Heuristic" << std::endl;
    }
    else {
        std::cout << "STRATEGY: Uniform Cost Search" << std::endl;
    }

    std::cout << std::endl;

    high_resolution_clock::time_point begin = high_resolution_clock::now();

    node start(puzzle, 0, strategy);
    search(start, solution);

    auto timeSpan = duration_cast<duration<double>>(high_resolution_clock::now() - begin);

    std::cout << "Search runtime:\t" << timeSpan.count() << " seconds" << std::endl;

    std::cout << "----------------------------------------------------------" << std::endl;

    return 0;
}
