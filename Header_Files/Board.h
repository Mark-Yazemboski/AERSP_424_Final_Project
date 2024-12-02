#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>   
#include <utility> 
#include "Snake.h"
#include <random>
#include "Coin.h"

//This board class will handel most of the game logic. It will perform all of the actoins nessesary
//To know the positions of the snake and food, and record them in a 2d array.
class Board {
public:

    //This is a structure, that was just easier inordr to perform the weighted random picking of the different coins.
    struct Coin_For_Picking {
        std::string name; 
        float weight;       
    };

    //This is a pair that will hold grid position information
    using Position = std::pair<int, int>;

    //This will initilize the board object, it will take in how big the board is, the snake thats playing on the board,
    //And the max number of coins that should be on the board at a time
    Board(int rows, int cols,Snake& Current_Snake, int Max_Number_Of_Coins)
        : rows(rows), cols(cols), grid(rows, std::vector<char>(cols, '.')),Current_Snake(Current_Snake), Max_Number_Of_Coins(Max_Number_Of_Coins) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));  // Seed for random food placement

        //Sets the weights for the different coin options
        coin_options = {
        {"Penny", 33.9},  
        {"Nickle", 30},  
        {"Dime", 20},     
        {"Quarter", 10}, 
        { "Dollar", 5 },
        {"Five_Dollar", 1},
        {"Jackpot", 0.1}
        };
        
        //Spawns the initial coins, and updates the board.
        Spawn_Coins();
        Update_Board();
    }

    //Will print out all of the coins that are on the board.
    void Print_Coins() {
        for (auto coin : Coins) {
            std::cout << coin.getType() << std::endl;
        }
    }

    //This will make sure there are the apropriot amount of coins on the board, and if not
    // It will pick a coin, and add it to the Coins list, and give it a valid position.
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

    //This will go through the different coins weights and choose a randomly weighted coin.
    //Chat GPT cooked this one up
    std::string Pick_Coin() {
        //Calculate the total weight
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

        return "";  
    }

    //Returns the Coins vector
    std::vector<Coin> Get_Coins() {
        return Coins;
    }
    
    //Returns the 2D array of the board
    std::vector<std::vector<char>> get_Board(){
        return grid;
    }

    //Initilizes the board as a bunch of "."'s
    void initializeBoard() {
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), '.'); // Empty cells
        }
    }

    //Sets a certain cell in the board to a specific value
    void setCell(Position pos, char value) {
        if (pos.first >= 0 && pos.first < rows && pos.second >= 0 && pos.second < cols) {
            grid[pos.first][pos.second] = value;
        }
    }

    //Returns the value of a specific cell in the board.
    char getCell(Position pos) const {
        if (pos.first >= 0 && pos.first < rows && pos.second >= 0 && pos.second < cols) {
            return grid[pos.first][pos.second];
        }
        return '\0';
    }


    //This is the main function of this class. This will take in all of the snake and coin information
    //and put it all on the board. It will also keep track if a coin was eaten.
    void Update_Board() {

        //Unique pointers are stupid so this is just a bypass to trying to just get acess to the Snakes body list.
        //the unique pointers didn't like being "Copied" whatever that means
        auto positionsWithTypes = Current_Snake.getPositionsWithTypes();

        //Clears the board before updating the snake's position
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), '.'); // Reset each cell to empty
        }

        //Initilizes the featen state to false
        bool Food_Eaten = false;


        // Iterate through the list of the different snake body parts and will mark them on the board.
        for (size_t i = 0; i < positionsWithTypes.size(); ++i) {

            const auto& posWithType = positionsWithTypes[i];
            int row = posWithType.position.first;  // Row coordinate
            int col = posWithType.position.second; // Column coordinate

            //Sees which one is the head, and willalso check to see if the head is in a coin's position
            if (posWithType.type == "Head") {
                
                grid[row][col] = 'H';

                //The Coins list was also being stupid for no reason so chat gpt summoned this unholy it thing. dont ask me
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
                
                grid[row][col] = 'T';
            } else if (posWithType.type == "Body") {
                
                grid[row][col] = 'B';
            }
        }

        //Makes sure to mark the coins positions on the board.
        for (Coin coin : Coins) {
            setCell(coin.getPosition(), 'C');
        }

        //If food is eaten, grow the snake and place new food
        if (Food_Eaten) {
            Spawn_Coins();
        }

        
        
    }

    //This will look throught the board and make sure to find an apropriate place to put a new coin.
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

    //This will just print the board
    void printBoard() const {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                std::cout << cell << ' ';
            }
            std::cout << std::endl;
        }
    }

    //returns row
    int getRows() const {
        return rows;
    }

    //returns col
    int getCols() const {
        return cols;
    }

private:

    //Bunch of variables
    int rows;
    int cols;
    int Max_Number_Of_Coins;
    std::vector<Coin> Coins;
    std::vector<Coin_For_Picking> coin_options;
    std::vector<std::vector<char>> grid;
    Snake& Current_Snake;
};

#endif // BOARD_H
