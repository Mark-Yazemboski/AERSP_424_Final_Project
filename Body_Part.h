#ifndef BODY_PART_H
#define BODY_PART_H

#include <string>
#include <utility>

class Body_Part {
public:
    using Position = std::pair<int, int>;

    Body_Part(Position position, int direction, const std::string& imageFile)
        : position(position), direction(direction), imageFile(imageFile) {}

    virtual ~Body_Part() = default;

    virtual void updatePosition(const Position& newPos) {
        position = newPos;
    }

    Position getPosition() const {
        return position;
    }

    int getDirection() const {
        return direction;
    }

    const std::string& getImageFile() const {
        return imageFile;
    }

    virtual std::string getType() const {
        return "Body_Part";
    }

protected:
    Position position;
    int direction; // 1=Up, 2=Right, 3=Down, 4=Left
    std::string imageFile;
};

class Head : public Body_Part {
public:
    Head(Position position, int direction)
        : Body_Part(position, direction, "head.png") {}

    std::string getType() const override {
        return "Head";
    }
};

class Body : public Body_Part {
public:
    Body(Position position, int direction)
        : Body_Part(position, direction, "body.png") {}

    std::string getType() const override {
        return "Body";
    }
};

class Tail : public Body_Part {
public:
    Tail(Position position, int direction)
        : Body_Part(position, direction, "tail.png") {}

    std::string getType() const override {
        return "Tail";
    }
};

#endif // BODY_PART_H