#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <utility> // For std::pair
#include "Snake.h"

class Board {
public:
    using Position = std::pair<int, int>;

    Board(int rows, int cols,Snake& Current_Snake)
        : rows(rows), cols(cols), grid(rows, std::vector<char>(cols, '.')),Current_Snake(Current_Snake) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));  // Seed for random food placement
        Update_Board();
        New_Food_Spot();
        setCell(Food_Position, 'F');
    }


    void initializeBoard() {
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), '.'); // Empty cells
        }
    }

    void setCell(Position pos, char value) {
        std::cout<<"FOOD "<<pos.first<<","<<pos.second<<std::endl;
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

    bool isFoodEaten(Position headPosition) const {
        return headPosition == Food_Position;
    }

    void Update_Board() {
        auto positionsWithTypes = Current_Snake.getPositionsWithTypes();
        // Clear the board before updating the snake's position
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), '.'); // Reset each cell to empty
        }

        bool Food_Eaten = false;


        // Iterate through the list of positions and types
        for (size_t i = 0; i < positionsWithTypes.size(); ++i) {
            const auto& posWithType = positionsWithTypes[i];
            int row = posWithType.position.first;  // Row coordinate
            int col = posWithType.position.second; // Column coordinate

            if (posWithType.type == "Head") {
                // Head
                grid[row][col] = 'H';
                if (isFoodEaten(posWithType.position)) {
                    Food_Eaten = true;
                }
            } else if (posWithType.type == "Tail") {
                // Tail
                grid[row][col] = 'T';
            } else if (posWithType.type == "Body") {
                // Body
                grid[row][col] = 'B';
            }
        }

        // If food is eaten, grow the snake and place new food
        if (Food_Eaten) {
            Current_Snake.grow();
            New_Food_Spot();
            
        }
        setCell(Food_Position, 'F');
    }

    void New_Food_Spot() {
        while (true) {
            int x = std::rand() % rows;
            int y = std::rand() % cols;

            // Ensure food is placed on an empty space
            if (grid[x][y] == '.' || grid[x][y] == ' ') { 
                Food_Position = {x, y};
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

    Position get_Food_Pos(){
        std::cout<<Food_Position.first<<","<<Food_Position.second<<std::endl;
        return Food_Position;
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
    Snake& Current_Snake;
};

#endif // BOARD_H
