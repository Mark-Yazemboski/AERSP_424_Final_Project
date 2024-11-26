#include <iostream>
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <utility> // For std::pair
#include "Snake.h"

class Board {
public:
    using Position = std::pair<int, int>;

    Board(int rows, int cols, Snake Current_Snake)
        : Current_Snake(Current_Snake), rows(rows), cols(cols), grid(rows, std::vector<char>(cols, ' ')) {
        std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed for random food placement
        initializeBoard();
    }

    void initializeBoard() {
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), ' '); // Empty cells
        }
    }

    void setCell(Position pos, char value) {
        if (pos.first >= 0 && pos.first < rows && pos.second >= 0 && pos.second < cols) {
            grid[pos.first][pos.second] = value;
        }
    }

    char getCell(Position pos) const {
        if (pos.first >= 0 && pos.first < rows && pos.second >= 0 && pos.second < cols) {
            return grid[pos.first][pos.second];
        }
        return '\0'; // Return null character if out of bounds
    }

    void Update_Board() {
        // Clear the board before updating the snake's position
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), ' '); // Reset each cell to empty
        }

        bool Food_Eaten = false;

        // Iterate through the snake's parts and place appropriate symbols
        const auto& parts = Current_Snake.getParts();
        for (size_t i = 0; i < parts.size(); ++i) {
            auto position = parts[i]->getPosition();
            int row = position.first;  // Row coordinate
            int col = position.second; // Column coordinate

            if (i == 0) {
                // Head
                grid[row][col] = 'H';
                if (row == Food_Position.first && col == Food_Position.second){
                    Food_Eaten = true;
                }
            } else if (i == parts.size() - 1) {
                // Tail
                grid[row][col] = 'T';
            } else {
                // Body
                grid[row][col] = 'B';
            }
        }
        if (Food_Eaten){
            Current_Snake.grow();
            New_Food_Spot();
            setCell(Food_Position, 'F');
        }
    }

    void New_Food_Spot() {
        while (true) {
            int x = std::rand() % rows;
            int y = std::rand() % cols;

            if (grid[x][y] == ' ') { // Place food only on empty cells
                Food_Position = {x,y};
                break;
            }
        }
    }

    void printBoard() const {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                std::cout << cell << ' ';
            }
            std::cout << std::endl;
        }
    }

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }

private:
    int rows;
    int cols;
    Position Food_Position;
    std::vector<std::vector<char>> grid;
    Snake Current_Snake;
};
