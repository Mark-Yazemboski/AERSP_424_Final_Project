#ifndef COIN_H
#define COIN_H

#include <string>
#include <utility>
#include <iostream>
#include <GLFW/glfw3.h>  // or whatever OpenGL headers you're using
#include "stb/stb_image.h" // Include stb_image.h for texture loading

//This is the coin base class. This will set up a lot of the functionallity for the different coins.
//We made this a subclass incase we wanted to make the coins each have their own ability, but rn they just have 
//different values, so this really isn't needed.
class Coin {
public:

    //Holds the coins grid position
    using Position = std::pair<int, int>;

    //Initilizes the coin object, with a position, image file, and value.
    Coin(Position position, const std::string& imageFile, float value)
        : position(position),  imageFile(imageFile), value(value) {
        textureID = loadTexture(imageFile);  // Load the texture when the body part is created
    }

    //Deletes the coin
    virtual ~Coin() = default;

    //Updates the position
    virtual void updatePosition(const Position& newPos) {
        position = newPos;
    }

    //returns the position
    Position getPosition() const {
        return position;
    }

    //Returns the imagefile
    const std::string& getImageFile() const {
        return imageFile;
    }

    //Returns the textureID
    GLuint getTextureID() const {
        return textureID;
    }

    //Returns the coins value
    float Get_Value() {
        return value;
    }

    //Returns the type of the coin
    virtual std::string getType() const {
        return "Coin";
    }
    
    //Checks to see if the coin was eaten, by being passed the snakeheads position
    bool Eaten(Position Snake_Head) {
        if (Snake_Head.first == position.first && Snake_Head.second == position.second) {
            return true;
        }
        return false;
    }

protected:
    GLuint textureID; // Store texture ID
    Position position;
    std::string imageFile;
    float value;

    //Loads the texture from the file and returns the texture ID
    //Artist: The Great and Honorable Chat GPT
    GLuint loadTexture(const std::string& imagePath) {
        GLuint textureID;
        glGenTextures(1, &textureID);  // Generate texture ID
        if (textureID == 0) {
            std::cerr << "Failed to generate texture ID for: " << imagePath << std::endl;
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);  // Bind texture

        // Load the image using STB
        int width, height, channels;
        unsigned char* image = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
        if (image == nullptr) {
            std::cerr << "Error loading texture: " << imagePath << std::endl;
            return 0;  // Return 0 if loading failed
        }

        // Determine format based on channels (GL_RGB or GL_RGBA)
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        // Upload the texture to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glColor3f(1.0f, 1.0f, 1.0f);


        stbi_image_free(image);  // Free the image memory after loading
        return textureID;  // Return the texture ID
    }

};

//This is the Penny subclass, this is pretty basic beause we dont have the coins doing anything cool 
//But they are here incase we want to. student.wisper(We dont, it will take to much time :P)
//This subclass just holds the coin's value, and type, and imagefile.
class Penny : public Coin {
public:
    Penny(Position position)
        : Coin(position, "PICS/Penny.png", 0.01) {}

    std::string getType() const override {
        return "Penny";
    }

};

//This is the Nickle subclass, this is pretty basic beause we dont have the coins doing anything cool 
//But they are here incase we want to. student.wisper(We dont, it will take to much time :P)
//This subclass just holds the coin's value, and type, and imagefile.
class Nickle : public Coin {
public:
    Nickle(Position position)
        : Coin(position, "PICS/Nickle.png", 0.05) {}

    std::string getType() const override {
        return "Nickle";
    }

};

//This is the Dime subclass, this is pretty basic beause we dont have the coins doing anything cool 
//But they are here incase we want to. student.wisper(We dont, it will take to much time :P)
//This subclass just holds the coin's value, and type, and imagefile.
class Dime : public Coin {
public:
    Dime(Position position)
        : Coin(position, "PICS/Dime.png", 0.10) {}

    std::string getType() const override {
        return "Dime";
    }

};

//This is the Quarter subclass, this is pretty basic beause we dont have the coins doing anything cool 
//But they are here incase we want to. student.wisper(We dont, it will take to much time :P)
//This subclass just holds the coin's value, and type, and imagefile.
class Quarter : public Coin {
public:
    Quarter(Position position)
        : Coin(position, "PICS/Quarter.png", 0.25) {}

    std::string getType() const override {
        return "Quarter";
    }

};

//This is the Dollar subclass, this is pretty basic beause we dont have the coins doing anything cool 
//But they are here incase we want to. student.wisper(We dont, it will take to much time :P)
//This subclass just holds the coin's value, and type, and imagefile.
class Dollar : public Coin {
public:
    Dollar(Position position)
        : Coin(position, "PICS/Dollar.png", 1.00) {}

    std::string getType() const override {
        return "Dollar";
    }

};

//This is the Five_Dollar subclass, this is pretty basic beause we dont have the coins doing anything cool 
//But they are here incase we want to. student.wisper(We dont, it will take to much time :P)
//This subclass just holds the coin's value, and type, and imagefile.
class Five_Dollar : public Coin {
public:
    Five_Dollar(Position position)
        : Coin(position, "PICS/Five_Dollar.png", 5.00) {}

    std::string getType() const override {
        return "Five_Dollar";
    }

};

//This is the Jackpot subclass, this is pretty basic beause we dont have the coins doing anything cool 
//But they are here incase we want to. student.wisper(We dont, it will take to much time :P)
//This subclass just holds the coin's value, and type, and imagefile.
class Jackpot : public Coin {
public:
    Jackpot(Position position)
        : Coin(position, "PICS/Jackpot.png", 9999.00) {}

    std::string getType() const override {
        return "Jackpot";
    }

};

#endif // COIN_H
