#ifndef BODY_PART_H
#define BODY_PART_H

#include <string>
#include <utility>
#include <iostream>
#include <GLFW/glfw3.h>  // or whatever OpenGL headers you're using
#include "stb/stb_image.h" // Include stb_image.h for texture loading

//This clas s is the base class for all of the snake body parts. It holds the essential functions like texture generation
// And holds a lot of the getter functions
class Body_Part {
public:

    //Pair that hold position information
    using Position = std::pair<int, int>;

    //Initilizes the bodypart object
    Body_Part(Position position, int direction, const std::string& imageFile)
        : position(position), direction(direction), imageFile(imageFile) {
        textureID = loadTexture(imageFile);  // Load the texture when the body part is created
    }

    //Deletes the bodypart
    virtual ~Body_Part() = default;

    //Updates the position of the bodypart
    virtual void updatePosition(const Position& newPos) {
        position = newPos;
    }

    //returns the position
    Position getPosition() const {
        return position;
    }

    //returns the direction
    int getDirection() const {
        return direction;
    }

    //Returns the image file of the object as a string
    const std::string& getImageFile() const {
        return imageFile;
    }

    //returns the texture ID
    GLuint getTextureID() const {
        return textureID;
    }

    //Returns the bodyparts type
    virtual std::string getType() const {
        return "Body_Part";
    }

protected:
    GLuint textureID;
    Position position;
    int direction; 
    std::string imageFile;

    //Loads the texture from the file and returns the texture ID
    //Artist: Chat GPT
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

//This is the head subclass, this kinda just holds the different image file, and type, but functionality could be added
class Head : public Body_Part {
public:
    Head(Position position, int direction)
        : Body_Part(position, direction, "PICS/head.png") {}

    std::string getType() const override {
        return "Head";
    }
};

//This is the body subclass, this kinda just holds the different image file, and type, but functionality could be added
class Body : public Body_Part {
public:
    Body(Position position, int direction)
        : Body_Part(position, direction, "PICS/body.png") {}

    std::string getType() const override {
        return "Body";
    }
};

//This is the tail subclass, this kinda just holds the different image file, and type, but functionality could be added
class Tail : public Body_Part {
public:
    Tail(Position position, int direction)
        : Body_Part(position, direction, "PICS/tail.png") {}

    std::string getType() const override {
        return "Tail";
    }
};

#endif // BODY_PART_H
