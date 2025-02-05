#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <cmath>

// Return the i and j values of a given tile (including empty space) in the 8-puzzle
// 00 01 02
// 10 11 12
// 20 21 22
std::pair<int, int> find_tile(std::vector<std::vector<int>> puzzle, int tile) {
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(1).size(); j++) {
            if (puzzle.at(i).at(j) == tile) {
                return {i, j};
            }
        }
    }

    return {-1, -1};
}

int heuristic(std::vector<std::vector<int>>& puzzle, int strategy) {
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
    // Misplaced Tile
    } else if (strategy == 2) {
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

struct node {
    std::vector<std::vector<int>> puzzle;
    int depth;
    int strategy;
    int heuristic_val;
    node(std::vector<std::vector<int>> puzzle, int depth, char strategy) : puzzle(puzzle), strategy(strategy), depth(depth) {
        heuristic_val = heuristic(puzzle, strategy);
    }
    bool operator>(const node second) const {
        if (this->depth + this->heuristic_val > second.depth + second.heuristic_val) {
            return true;
        }
        else {
            return false;
        }
    }
};

// Test function to print out every node
void print(const std::vector<std::vector<int>>& puzzle) {
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(0).size(); j++) {
            std::cout << puzzle.at(i).at(j) << " ";
        }
    }
    std::cout << std::endl;
}

// Hashing function to put into unique node: if we find a number that's already in the vector, we found a repeated state
int encode(std::vector<std::vector<int>> puzzle) {
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

// Main searching function
void search(node start, const std::vector<std::vector<int>>& solution) {
    std::priority_queue<node, std::vector<node>, std::greater<node>> queue;
    std::unordered_map<int, std::vector<std::vector<int>>> unique_nodes;
    int max_size = queue.size();

    // Push the puzzle in as the first node
    queue.push(start);
    unique_nodes[encode(start.puzzle)] = start.puzzle;

    // Main loop: if we break out of the loop, something went wrong.
    while (queue.size() != 0) {
 
        // print(nodes.front());

        // If solution found, return how many nodes we had to expand
        if (queue.top().puzzle == solution) {
            std::cout << "Solution depth: " << queue.top().depth << std::endl;
            std::cout << "Nodes expanded: " << unique_nodes.size() << std::endl;
            std::cout << "Max queue size: " << max_size << std::endl;
            std::cout << "Nodes in front: " << queue.size() << std::endl;
            return;
        }

        node curr = queue.top();
        queue.pop();

        std::pair<int, int> space = find_tile(curr.puzzle, 0);
        
        // Search top: if a tile exists above the space, swap
        if (space.first - 1 >= 0) {
            std::vector<std::vector<int>> top_puzzle = curr.puzzle;
            std::swap(top_puzzle.at(space.first).at(space.second), top_puzzle.at(space.first - 1).at(space.second));
            node top(top_puzzle, curr.depth + 1, curr.strategy);

            // Check if the created node is already expanded
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

            // Check if the created node is already expanded
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

            // Check if the created node is already expanded
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

            // Check if the created node is already expanded
            int key = encode(right.puzzle);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = right.puzzle;
                queue.push(right);
            }
        }

        if (max_size < queue.size()) {
            max_size = queue.size();
        }
    }

    std::cout << "Solution not found." << std::endl;
}

int main(int arg, char* argv[]) {
    std::vector<std::vector<int>> puzzle = {{std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3])},
                                            {std::stoi(argv[4]), std::stoi(argv[5]), std::stoi(argv[6])},
                                            {std::stoi(argv[7]), std::stoi(argv[8]), std::stoi(argv[9])}};
                
    const std::vector<std::vector<int>> solution = {{1, 2, 3}, 
                                                    {4, 5, 6}, 
                                                    {7, 8, 0}};

    // input 1 for manhattan, 2 for misplaced, 3 for uniform
    int strategy = std::stoi(argv[10]);
    node start(puzzle, 0, strategy);
    search(start, solution);

    return 0;
}