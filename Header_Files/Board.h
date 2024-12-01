#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <utility> // For std::pair
#include "Snake.h"
#include <random>
#include "Coin.h"

class Board {
public:

    struct Coin_For_Picking {
        std::string name;  // Coin name
        float weight;        // Weight (rarity)
    };

    using Position = std::pair<int, int>;

    Board(int rows, int cols,Snake& Current_Snake, int Max_Number_Of_Coins)
        : rows(rows), cols(cols), grid(rows, std::vector<char>(cols, '.')),Current_Snake(Current_Snake), Max_Number_Of_Coins(Max_Number_Of_Coins) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));  // Seed for random food placement
        coin_options = {
        {"Penny", 33.9},  
        {"Nickle", 30},  
        {"Dime", 20},     
        {"Quarter", 10}, 
        { "Dollar", 5 },
        {"Five_Dollar", 1},
        {"Jackpot", 0.1}
        };
        
        Spawn_Coins();
        Update_Board();
    }

    void Print_Coins() {
        for (auto coin : Coins) {
            std::cout << coin.getType() << std::endl;
        }
    }

    void Spawn_Coins() {
        std::string Picked_Coin;
        Position Coin_Pos;
        while (Coins.size() < Max_Number_Of_Coins) {
            Picked_Coin = Pick_Coin();
            Coin_Pos  = New_Coin_Spot();
            if (Picked_Coin == "Penny") {
                Coins.push_back(Penny(Coin_Pos));
            }
            else if (Picked_Coin == "Nickle") {
                Coins.push_back(Nickle(Coin_Pos));
            }
            else if (Picked_Coin == "Dime") {
                Coins.push_back(Dime(Coin_Pos));
            }
            else if (Picked_Coin == "Quarter") {
                Coins.push_back(Quarter(Coin_Pos));
            }
            else if (Picked_Coin == "Dollar") {
                Coins.push_back(Dollar(Coin_Pos));
            }
            else if (Picked_Coin == "Five_Dollar") {
                Coins.push_back(Five_Dollar(Coin_Pos));
            }
            else if (Picked_Coin == "Jackpot") {
                Coins.push_back(Jackpot(Coin_Pos));
            }
            else {
                std::cout << "ERROR" << std::endl;
            }
        }
        setCell(Coin_Pos, 'C');
    }


    std::string Pick_Coin() {
        // Calculate the total weight (as a float)
        float totalWeight = 0.0f;
        for (const auto& coin : coin_options) {
            totalWeight += coin.weight;
        }

        // Check for invalid state
        if (totalWeight <= 0.0f) {
            std::cerr << "Error: Total weight is zero or coin_options is empty." << std::endl;
            return ""; // Return an empty string or handle this case appropriately
        }

        // Generate a random number in the range [0, totalWeight)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(0.0f, totalWeight);
        float randomWeight = dist(gen);

        // Determine which coin corresponds to the random weight
        float cumulativeWeight = 0.0f;
        for (const auto& coin : coin_options) {
            cumulativeWeight += coin.weight;
            if (randomWeight < cumulativeWeight) {
                return coin.name;  // Return the selected coin's name
            }
        }

        return "";  // Default return, shouldn't happen if totalWeight > 0
    }


    std::vector<Coin> Get_Coins() {
        return Coins;
    }

    std::vector<std::vector<char>> get_Board(){
        return grid;
    }

    void initializeBoard() {
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), '.'); // Empty cells
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
                for (auto it = Coins.begin(); it != Coins.end(); ) {
                    if (it->Eaten(posWithType.position)) {
                        Food_Eaten = true;
                        Current_Snake.Change_Score(it->Get_Value());
                        it = Coins.erase(it);  // Erase the coin and get a valid iterator to the next element
                    }
                    else {
                        ++it;  // Advance the iterator
                    }
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
            Spawn_Coins();
        }

        for (Coin coin : Coins) {
            setCell(coin.getPosition(), 'C');
        }
        
    }

    Position New_Coin_Spot() {
        Position Coin_Pos;
        while (true) {
            int x = std::rand() % rows;
            int y = std::rand() % cols;

            // Ensure food is placed on an empty space
            if (grid[x][y] == '.' || grid[x][y] == ' ') { 
                Coin_Pos = { x, y };
                return Coin_Pos;
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

    /*Position get_Food_Pos(){
        std::cout<<Food_Position.first<<","<<Food_Position.second<<std::endl;
        return Food_Position;
    }*/

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }

private:
    int rows;
    int cols;
    int Max_Number_Of_Coins;
    std::vector<Coin> Coins;
    std::vector<Coin_For_Picking> coin_options;
    std::vector<std::vector<char>> grid;
    Snake& Current_Snake;
};

#endif // BOARD_H
