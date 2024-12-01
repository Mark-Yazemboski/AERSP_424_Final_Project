#ifndef BODY_PART_H
#define BODY_PART_H

#include <string>
#include <utility>
#include <iostream>
#include <GLFW/glfw3.h>  // or whatever OpenGL headers you're using
#include "stb/stb_image.h" // Include stb_image.h for texture loading

class Body_Part {
public:
    using Position = std::pair<int, int>;

    Body_Part(Position position, int direction, const std::string& imageFile)
        : position(position), direction(direction), imageFile(imageFile) {
        textureID = loadTexture(imageFile);  // Load the texture when the body part is created
    }

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

    GLuint getTextureID() const {
        return textureID;
    }

    virtual std::string getType() const {
        return "Body_Part";
    }

protected:
    GLuint textureID; // Store texture ID
    Position position;
    int direction; // 1=Up, 2=Right, 3=Down, 4=Left
    std::string imageFile;

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

class Head : public Body_Part {
public:
    Head(Position position, int direction)
        : Body_Part(position, direction, "PICS/head.png") {}

    std::string getType() const override {
        return "Head";
    }
};

class Body : public Body_Part {
public:
    Body(Position position, int direction)
        : Body_Part(position, direction, "PICS/body.png") {}

    std::string getType() const override {
        return "Body";
    }
};

class Tail : public Body_Part {
public:
    Tail(Position position, int direction)
        : Body_Part(position, direction, "PICS/tail.png") {}

    std::string getType() const override {
        return "Tail";
    }
};

#endif // BODY_PART_H
