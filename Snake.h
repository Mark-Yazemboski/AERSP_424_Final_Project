#ifndef SNAKE_H
#define SNAKE_H

#include "Body_Part.h"
#include <deque>
#include <memory>
#include <string>
#include <utility> // for std::pair
#include <iostream>
#include <vector> // for std::vector

class Snake {
public:
    using Position = Body_Part::Position;

    // Struct to hold both position and the type of the body part
    struct PositionWithType {
        Position position;
        std::string type;

        PositionWithType(Position pos, std::string objType)
            : position(pos), type(objType) {}
    };

    // Constructor with initial position, direction, and length
    Snake(Position initialPosition, int initialDirection = 1, int initialLength = 1, float Score = 0, float Growth_Interval = 0.5) 
        : direction(initialDirection), growLength(0), Score(Score), Growth_Interval(Growth_Interval), Length(0){
        // Initialize snake with a head and body parts
        parts.push_back(std::make_unique<Head>(initialPosition, initialDirection));

        Position currentPosition = initialPosition;
        for (int i = 1; i < initialLength; ++i) {
            currentPosition = movePosition(currentPosition, initialDirection, -1);
            parts.push_back(std::make_unique<Body>(currentPosition, initialDirection));
        }

        currentPosition = movePosition(currentPosition, initialDirection, -1);
        parts.push_back(std::make_unique<Tail>(currentPosition, initialDirection));
    }

    // Get a const reference to the deque of body parts
    const std::deque<std::unique_ptr<Body_Part>>& getParts() const {
        return parts;
    }


    bool checkCollision(int boardRows, int boardCols) const {
        // Get the head's position
        Position headPosition = parts.front()->getPosition();

        // Check collision with walls
        if (headPosition.first < 0 || headPosition.first >= boardRows || 
            headPosition.second < 0 || headPosition.second >= boardCols) {
            return true; // Collision with wall
        }

        // Check self-collision (head intersects with the body)
        for (size_t i = 1; i < parts.size(); ++i) {
            if (headPosition == parts[i]->getPosition()) {
                return true; // Collision with itself
            }
        }

        // No collision detected
        return false;
    }

    // Function to return a list of positions with their types
    std::vector<PositionWithType> getPositionsWithTypes() const {
        std::vector<PositionWithType> positionsWithTypes;

        for (const auto& part : parts) {
            std::string partType = part->getType();  // Get the type like "Head", "Body", "Tail"
            positionsWithTypes.push_back(PositionWithType(part->getPosition(), partType));
        }

        return positionsWithTypes;
    }

    void Change_Score(float Added_Score) {
        Score += Added_Score;
        if (int((Score - Length * Growth_Interval) / Growth_Interval + 0.001)>=1) {
            grow(int((Score - Length * Growth_Interval) / Growth_Interval + 0.001));
        }
    }

    float Get_Score() {
        return Score;
    }

    // Function to grow the snake
    void grow(int length = 1) {
        growLength += length; // Increment the grow length by the given value
        Length += length;
    }

    void Set_Direction(int New_Direction = 1){
        direction = New_Direction;
    }

    int Get_Direction() {
        return direction;
    }

    // Function to move the snake
    void move() {
        Position headPosition = parts.front()->getPosition();
        Position newHeadPosition = movePosition(headPosition, direction, 1);

        parts[0] = std::make_unique<Body>(headPosition, parts.front()->getDirection());

        parts.push_front(std::make_unique<Head>(newHeadPosition, direction));

        if (growLength > 0) {
            --growLength;
        } else if (parts.size() > 1) {
            parts.pop_back();
            parts.back() = std::make_unique<Tail>(parts.back()->getPosition(), parts.back()->getDirection());
        }
    }

    // Print the snake parts
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

    // Move the position based on the direction
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
