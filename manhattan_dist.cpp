#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <cmath>

// Test function to print out every node
void print(const std::vector<std::vector<int>>& puzzle) {
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(0).size(); j++) {
            std::cout << puzzle.at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Return the i and j values of the empty space in the 8-puzzle
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

int manhattan_dist(std::vector<std::vector<int>>& puzzle, const std::vector<std::vector<int>>& solution) {
    int count = 0;
    for (unsigned i = 1; i <= 8; i++) {
        std::pair p = find_tile(puzzle, i);
        std::pair s = find_tile(solution, i);
        count += abs(p.first-s.first) + abs(p.second-s.second);
    }
    return count;
}

// Hashing function to put into unique node: if we find a number that's already in the vector, we found a repeated state
int encode_vector(std::vector<std::vector<int>> puzzle) {
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
int search(std::vector<std::vector<int>>& puzzle, const std::vector<std::vector<int>>& solution) {
    std::queue<std::vector<std::vector<int>>> nodes;
    std::unordered_map<int, std::vector<std::vector<int>>> unique_nodes;

    // Push the puzzle in as the first node
    nodes.push(puzzle);
    unique_nodes[encode_vector(puzzle)] = puzzle;

    // Main loop: if we break out of the loop, something went wrong.
    while (nodes.size() != 0) {
 
        // print(nodes.front());

        // If solution found, return how many nodes we had to expand
        if (nodes.front() == solution) {
            return unique_nodes.size();
        }

        std::vector<std::vector<int>> curr = nodes.front();
        nodes.pop();

        // std::cout << manhattan_dist(curr, solution) << std::endl;

        std::pair<int, int> space = find_tile(curr, 0);

        std::vector<std::vector<int>> top = curr;
        std::vector<std::vector<int>> bottom = curr;
        std::vector<std::vector<int>> left = curr;
        std::vector<std::vector<int>> right = curr;

        std::vector<std::vector<std::vector<int>>> node_queue;

        // Search top: if a tile exists above the space, swap
        if (space.first - 1 >= 0) {
            std::swap(top.at(space.first).at(space.second), top.at(space.first - 1).at(space.second));

            // Check if the created node is already expanded
            int key = encode_vector(top);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = top;
                node_queue.push_back(top);
                // nodes.push(top);
            }
        }

        // Search bottom: if a tile exists below the space, swap
        if (space.first + 1 < curr.size()) {
            std::swap(bottom.at(space.first).at(space.second), bottom.at(space.first + 1).at(space.second));

            // Check if the created node is already expanded
            int key = encode_vector(bottom);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = bottom;
                node_queue.push_back(bottom);
                // nodes.push(bottom);
            }
        }

        // Search left: if a tile exists left of the space, swap
        if (space.second - 1 >= 0) {
            std::swap(left.at(space.first).at(space.second), left.at(space.first).at(space.second - 1));

            // Check if the created node is already expanded
            int key = encode_vector(left);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = left;
                node_queue.push_back(left);
                // nodes.push(left);
            }
        }

        // Search right: if a tile exists right of the space, swap
        if (space.second + 1 < curr.size()) {
            std::swap(right.at(space.first).at(space.second), right.at(space.first).at(space.second + 1));

            // Check if the created node is already expanded
            int key = encode_vector(right);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = right;
                node_queue.push_back(right);
                // nodes.push(right);
            }
        }

        for (unsigned i = 0; i < node_queue.size(); i++) {
            unsigned optimal_idx = i;
            for (unsigned j = i+1; j < node_queue.size(); j++) {
                if (manhattan_dist(node_queue.at(optimal_idx), solution) > manhattan_dist(node_queue.at(j), solution)) {
                    optimal_idx = j;
                }
            }
            std::vector<std::vector<int>> temp = node_queue.at(i);
            node_queue.at(i) = node_queue.at(optimal_idx);
            node_queue.at(optimal_idx) = temp;
        }

        for (unsigned i = 0; i < node_queue.size(); i++) {
            nodes.push(node_queue.at(i));
        }

    }

    // At this point, this would return the maximum possible states, which is 181,440
    return unique_nodes.size();
}

int main(int arg, char* argv[]) {
    std::vector<std::vector<int>> puzzle = {{std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3])},
                                            {std::stoi(argv[4]), std::stoi(argv[5]), std::stoi(argv[6])},
                                            {std::stoi(argv[7]), std::stoi(argv[8]), std::stoi(argv[9])}};
                
    const std::vector<std::vector<int>> solution = {{1, 2, 3}, 
                                                    {4, 5, 6}, 
                                                    {7, 8, 0}};
    
    int sol = search(puzzle, solution);

    std::cout << "Expanded a total of " << sol << " nodes for the solution." << std::endl;

    return 0;
}