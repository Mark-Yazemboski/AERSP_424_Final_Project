#ifndef COIN_H
#define COIN_H

#include <string>
#include <utility>
#include <iostream>
#include <GLFW/glfw3.h>  // or whatever OpenGL headers you're using
#include "stb/stb_image.h" // Include stb_image.h for texture loading

class Coin {
public:
    using Position = std::pair<int, int>;

    Coin(Position position, const std::string& imageFile, float value)
        : position(position),  imageFile(imageFile), value(value) {
        textureID = loadTexture(imageFile);  // Load the texture when the body part is created
    }

    virtual ~Coin() = default;

    virtual void updatePosition(const Position& newPos) {
        position = newPos;
    }

    Position getPosition() const {
        return position;
    }


    const std::string& getImageFile() const {
        return imageFile;
    }

    GLuint getTextureID() const {
        return textureID;
    }

    float Get_Value() {
        return value;
    }

    virtual std::string getType() const {
        return "Coin";
    }
    
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

    // Load texture from file and return the texture ID
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

class Penny : public Coin {
public:
    Penny(Position position)
        : Coin(position, "PICS/Penny.png", 0.01) {}

    std::string getType() const override {
        return "Penny";
    }

};

class Nickle : public Coin {
public:
    Nickle(Position position)
        : Coin(position, "PICS/Nickle.png", 0.05) {}

    std::string getType() const override {
        return "Nickle";
    }

};

class Dime : public Coin {
public:
    Dime(Position position)
        : Coin(position, "PICS/Dime.png", 0.10) {}

    std::string getType() const override {
        return "Dime";
    }

};

class Quarter : public Coin {
public:
    Quarter(Position position)
        : Coin(position, "PICS/Quarter.png", 0.25) {}

    std::string getType() const override {
        return "Quarter";
    }

};

class Dollar : public Coin {
public:
    Dollar(Position position)
        : Coin(position, "PICS/Dollar.png", 1.00) {}

    std::string getType() const override {
        return "Dollar";
    }

};

class Five_Dollar : public Coin {
public:
    Five_Dollar(Position position)
        : Coin(position, "PICS/Five_Dollar.png", 5.00) {}

    std::string getType() const override {
        return "Five_Dollar";
    }

};

class Jackpot : public Coin {
public:
    Jackpot(Position position)
        : Coin(position, "PICS/Jackpot.png", 9999.00) {}

    std::string getType() const override {
        return "Jackpot";
    }

};

#endif // COIN_H
