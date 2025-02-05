#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <queue>
#include <algorithm>
#include <unordered_map>

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

// Return the i and j values of the empty space in the 8-puzzle
// 00 01 02
// 10 11 12
// 20 21 22
std::pair<int, int> find_space(std::vector<std::vector<int>> puzzle) {
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(1).size(); j++) {
            if (puzzle.at(i).at(j) == 0) {
                return {i, j};
            }
        }
    }

    return {-1, -1};
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

        std::pair<int, int> space = find_space(curr);
        
        // Search top: if a tile exists above the space, swap
        if (space.first - 1 >= 0) {
            std::vector<std::vector<int>> top = curr;
            std::swap(top.at(space.first).at(space.second), top.at(space.first - 1).at(space.second));

            // Check if the created node is already expanded
            int key = encode_vector(top);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = top;
                nodes.push(top);
            }
        }

        // Search bottom: if a tile exists below the space, swap
        if (space.first + 1 < curr.size()) {
            std::vector<std::vector<int>> bottom = curr;
            std::swap(bottom.at(space.first).at(space.second), bottom.at(space.first + 1).at(space.second));

            // Check if the created node is already expanded
            int key = encode_vector(bottom);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = bottom;
                nodes.push(bottom);
            }
        }

        // Search left: if a tile exists left of the space, swap
        if (space.second - 1 >= 0) {
            std::vector<std::vector<int>> left = curr;
            std::swap(left.at(space.first).at(space.second), left.at(space.first).at(space.second - 1));

            // Check if the created node is already expanded
            int key = encode_vector(left);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = left;
                nodes.push(left);
            }
        }

        // Search right: if a tile exists right of the space, swap
        if (space.second + 1 < curr.size()) {
            std::vector<std::vector<int>> right = curr;
            std::swap(right.at(space.first).at(space.second), right.at(space.first).at(space.second + 1));

            // Check if the created node is already expanded
            int key = encode_vector(right);
            if (unique_nodes.find(key) == unique_nodes.end()) {
                unique_nodes[key] = right;
                nodes.push(right);
            }
        }
    }

    // At this point, this would return the maximum possible states, which is 181,400
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

int misplaced_tiles(std::vector<std::vector<int>>& puzzle, const std::vector<std::vector<int>>& solution) {
    int count = 0;
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(0).size(); j++) {
            if (puzzle.at(i).at(j) != solution.at(i).at(j) && puzzle.at(i).at(j) != 0) {
                count++;
            }
        }
    }
}