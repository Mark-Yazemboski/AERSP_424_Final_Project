#ifndef SNAKE_H
#define SNAKE_H

#include "Body_Part.h"
#include <deque>
#include <memory>
#include <string>
#include <utility> 
#include <iostream>
#include <vector> 

//This is the snake class, which will keep track of all of its body parts, and will also handel its own movment
//And appending and deletion of bodyparts.
class Snake {
public:

    //redefines Position so the code is cleaner
    using Position = Body_Part::Position;

    //Struct to hold both position and the type of the body part
    struct PositionWithType {
        Position position;
        std::string type;

        PositionWithType(Position pos, std::string objType)
            : position(pos), type(objType) {}
    };

    //Snake object initilizer, that will tke in an initial position,length, score, and growth interval, and direction
    //A growth interval is what score interval will the snake grow
    Snake(Position initialPosition, int initialDirection = 1, int initialLength = 1, float Score = 0, float Growth_Interval = 0.5) 
        : direction(initialDirection), growLength(0), Score(Score), Growth_Interval(Growth_Interval), Length(0){
        
        
        //Initialize snake with a head and body parts
        parts.push_back(std::make_unique<Head>(initialPosition, initialDirection));
        Position currentPosition = initialPosition;
        for (int i = 1; i < initialLength; ++i) {
            currentPosition = movePosition(currentPosition, initialDirection, -1);
            parts.push_back(std::make_unique<Body>(currentPosition, initialDirection));
        }

        currentPosition = movePosition(currentPosition, initialDirection, -1);
        parts.push_back(std::make_unique<Tail>(currentPosition, initialDirection));
    }

    //Gets a const reference to the deque of body parts
    const std::deque<std::unique_ptr<Body_Part>>& getParts() const {
        return parts;
    }

    //Checks to see if thee snake hit the edge of the board, or itsself
    bool checkCollision(int boardRows, int boardCols) const {
        //Gets the head's position
        Position headPosition = parts.front()->getPosition();

        //Checks collision with walls
        if (headPosition.first < 0 || headPosition.first >= boardRows || 
            headPosition.second < 0 || headPosition.second >= boardCols) {
            return true;
        }

        //Checks if the snake hit itsself
        for (size_t i = 1; i < parts.size(); ++i) {
            if (headPosition == parts[i]->getPosition()) {
                return true; 
            }
        }

        //No collision detected
        return false;
    }

    //Returns a list of positions with their types
    std::vector<PositionWithType> getPositionsWithTypes() const {
        std::vector<PositionWithType> positionsWithTypes;

        for (const auto& part : parts) {
            std::string partType = part->getType(); 
            positionsWithTypes.push_back(PositionWithType(part->getPosition(), partType));
        }

        return positionsWithTypes;
    }

    //Updates the score
    void Change_Score(float Added_Score) {
        Score += Added_Score;
        if (int((Score - Length * Growth_Interval) / Growth_Interval + 0.001)>=1) {
            grow(int((Score - Length * Growth_Interval) / Growth_Interval + 0.001));
        }
    }

    //returns the score
    float Get_Score() {
        return Score;
    }

    //Grows the snake, and updates the length of the snake.
    void grow(int length = 1) {
        growLength += length; 
        Length += length;
    }

    //Sets the direction of the snake
    void Set_Direction(int New_Direction = 1){
        direction = New_Direction;
    }

    //returns the direction of the snake
    int Get_Direction() {
        return direction;
    }

    //This is the key function of the snake class.
    //This will move the snake, and will make sure the snakes body parts move in the correct direction
    //It will append the head in the correct direction, delete the old head and make it a body,
    //Then delete the old tail and make the old past body the new tail.
    void move() {
        //Gets the current head position
        Position headPosition = parts.front()->getPosition();

        //Calculates the new head position
        Position newHeadPosition = movePosition(headPosition, direction, 1);

        //Sets the current head to a body peice
        parts[0] = std::make_unique<Body>(headPosition, parts.front()->getDirection());

        //Adds a new head
        parts.push_front(std::make_unique<Head>(newHeadPosition, direction));

        //Skips the deleting the tail portion if the snake is growing.
        if (growLength > 0) {
            --growLength;
        } else if (parts.size() > 1) {
            parts.pop_back();
            parts.back() = std::make_unique<Tail>(parts.back()->getPosition(), parts.back()->getDirection());
        }
    }

    //Prints the snake parts
    void print() const {
        for (const auto& part : parts) {
            std::cout << part->getType() << " at "
                << "(" << part->getPosition().first << ", " << part->getPosition().second << ")"
                << " Body ID " << &part
                << std::endl;
        }
    }

private:
    std::deque<std::unique_ptr<Body_Part>> parts;
    int direction;
    int growLength;
    float Score;
    float Growth_Interval;
    int Length;

    //Uses cases to easily turn the direction data into a new position.
    Position movePosition(Position pos, int direction, int step) const {
        switch (direction) {
        case 1: return { pos.first, pos.second + step }; // Right
        case 2: return { pos.first + step, pos.second }; // Down
        case 3: return { pos.first, pos.second - step }; // Left
        case 4: return { pos.first - step, pos.second }; // Up
        default: return pos;
        }
    }
};

#endif // SNAKE_H
