#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include <deque>
#include <stb/stb_image.h>
#include "Snake.h"
#include "Board.h"
#include <sstream>
#include "Body_Part.h"
#include <chrono>
#include <iomanip>

//This is the main play file. This is what will render and run the game. Most of this is 
//OpenGL visual voodoo, and I only understand it 50% just enough to fix simple chatGPT messups



//Sets the total window dimentions, and the board window dimentions
const int Total_windowWidth = 800;
const int Total_windowHeight = 900;

const int windowWidth = 800;
const int windowHeight = 800;

//sets the number of rows and cols for the snake board.
const int boardRows = 20;
const int boardCols = 20;

//Calculates the cell dimentions
const float cellWidth = windowWidth / static_cast<float>(boardCols);
const float cellHeight = windowHeight / static_cast<float>(boardRows);

//Same function thats in the classes, so this is very inefficient, but it works :P
//This will take in an image path, and will turn it into a texture ID
//Conducted by: Chatters GPTes the third.
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

//Draws a rectangle when given a position, dimentions, and a color
void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

//This will take a snake body part texture, and will display them in the correct position om the screen
void render_snake(Body_Part& part) {

    //Obtains the texture, and turns it into something
    glBindTexture(GL_TEXTURE_2D, part.getTextureID());

    //Push the current matrix onto the stack
    glPushMatrix();

    //Translate to the correct grid position
    float x = part.getPosition().second * cellWidth;  // Column
    float y = part.getPosition().first * cellHeight;  // Row
    glTranslatef(x + cellWidth / 2, y + cellHeight / 2, 0.0f);

    //Applies rotation so the image is in the correct direction
    switch (part.getDirection()) {
    case 4: glRotatef(0.0f, 0.0f, 0.0f, 1.0f); break;   // Up
    case 1: glRotatef(90.0f, 0.0f, 0.0f, 1.0f); break;  // Right
    case 2: glRotatef(180.0f, 0.0f, 0.0f, 1.0f); break; // Down
    case 3: glRotatef(270.0f, 0.0f, 0.0f, 1.0f); break; // Left
    }

    //Renders the quad with the texture applied
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-cellWidth / 2, -cellHeight / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(cellWidth / 2, -cellHeight / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(cellWidth / 2, cellHeight / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-cellWidth / 2, cellHeight / 2);
    glEnd();

    //Restores the matrix
    glPopMatrix();
}

//This will take in a coin object, and will render its texture in the correct position
void render_Coin(Coin coin) {

    //Obtains the texture, and turns it into something
    glBindTexture(GL_TEXTURE_2D, coin.getTextureID());

    //Push the current matrix onto the stack
    glPushMatrix();

    //Translate to the correct grid position
    float x = coin.getPosition().second * cellWidth;  // Column
    float y = coin.getPosition().first * cellHeight;  // Row
    glTranslatef(x + cellWidth / 2, y + cellHeight / 2, 0.0f);


    //Render the quad with the texture applied
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-cellWidth / 2, -cellHeight / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(cellWidth / 2, -cellHeight / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(cellWidth / 2, cellHeight / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-cellWidth / 2, cellHeight / 2);
    glEnd();

    //Restore the matrix
    glPopMatrix();
}

//This will look at the font atlas for the project, and will turn it into a texture.
GLuint loadFontTexture(const std::string& fontImagePath) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, channels;
    unsigned char* data = stbi_load(fontImagePath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load font texture: " << fontImagePath << std::endl;
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}

//This will render any text we want. It will take in a position, string, scale, and the font texture.
//All this does is goes through the string we give, and it will look at each character and will find the correct
//position in the font image that corispods to that character, and will then place it where the character should go.
void renderText(const std::string& text, float x, float y, float scale, GLuint fontTextureID) {

    //finds the cell width of the font texture
    const float glyphWidth = 1.0f / 16.0f; 
    const float glyphHeight = 1.0f / 16.0f;

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);

    //Runs through each character, and finds the font image to corispond
    for (char c : text) {
        int asciiCode = static_cast<int>(c);

        //Calculate texture coordinates
        float tx = (asciiCode % 16) * glyphWidth;
        float ty = (asciiCode / 16) * glyphHeight;


        //Draw quad for the character
        glTexCoord2f(tx, ty); glVertex2f(x, y);
        glTexCoord2f(tx + glyphWidth, ty ); glVertex2f(x + scale, y);
        glTexCoord2f(tx + glyphWidth, ty + glyphHeight); glVertex2f(x + scale, y + scale);
        glTexCoord2f(tx, ty + glyphHeight); glVertex2f(x, y + scale);

        x += scale; // Advance to the next character position
    }
    glEnd();
}


//This will make sure the score is only 2 decimal places
std::string formatScore(double rounded) {
    std::ostringstream oss;
    oss << "Score: $" << std::fixed << std::setprecision(2) << rounded;
    return oss.str();
}

//This will take in the current score, and will display in in the bottom left of the screen
void render_Score(Snake& snake, GLuint fontTextureID) {
    // Draw background for the score area
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, windowHeight);
    glVertex2f(windowWidth, windowHeight);
    glVertex2f(windowWidth, Total_windowHeight);
    glVertex2f(0, Total_windowHeight);
    glEnd();

    // Render the score text
    std::string scoreText = formatScore(snake.Get_Score());
    //std::string scoreText = "Score: $" + std::to_string(rounded);
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white
    renderText(scoreText, 10.0f, windowHeight + 20.0f, 40.0f, fontTextureID);
}


//This will bring everything together and will render the whole board
void renderBoard(GLuint Background_Texture, Board& board,Snake& snake) {

    //Gets the 2D array board
    std::vector<std::vector<char>> Snake_Board = board.get_Board();

    //Sets the background texture
    glBindTexture(GL_TEXTURE_2D, Background_Texture);

    //Push the current matrix onto the stack
    glPushMatrix();



    //displays the background
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(windowWidth, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(windowWidth, windowHeight);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, windowHeight);
    glEnd();

    //Restore the matrix
    glPopMatrix();

    

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    //Will run through each body part in the snake, and renders it
    const std::deque<std::unique_ptr<Body_Part>>& Snake_Body_Parts = snake.getParts();
    std::vector<Coin> Coins = board.Get_Coins();
    for (const auto& Snake_Piece : Snake_Body_Parts) {
        
        render_snake(*Snake_Piece);
    }

    //Runs through each coin, and renders it
    for (Coin coin : Coins) {
        render_Coin(coin);
    }
    

    
}


//This will actually run the snake game
int main() {
    
    //This is a check to make sure you have the correct visual libraries
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "The Slots are Calling", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Creates the game window
    glfwMakeContextCurrent(window);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f); 
    glOrtho(0.0f, Total_windowWidth, Total_windowHeight, 0.0f, -1.0f, 1.0f);

    //Initilizes the snake and board objects
    Snake snake({ boardRows / 2, boardCols / 2 }, 1, 2, 0, 0.25);
    Board board(boardRows, boardCols, snake, 5);

    //We gamin
    bool gameRunning = true;

    //We need this so the inputs can run at a different speed than the visual, it 
    //makes the game so much smoother.
    auto lastRenderTime = std::chrono::high_resolution_clock::now();
    auto lastUpdateTime = std::chrono::high_resolution_clock::now();

    //Sets the snake to run at 10fps, and the inputs to update at 20fps
    const double renderInterval = 1.0 / 10.0; // 60 FPS
    const double updateInterval = 1.0 / 20.0; // Snake updates at 15 FPS

    int Current_Selected_Direction = snake.Get_Direction();

    //Creates the background, and font texture ID's
    GLuint Background_textureID = loadTexture("PICS/Background.png");
    GLuint fontTextureID = loadFontTexture("PICS/font_atlas.png");

    //Makes sure you hvave a font texture
    if (!fontTextureID) {
        std::cerr << "Failed to load font texture!" << std::endl;
    }

    //This runs the game and will go till the wondow is closed or the gameRunning variable is false
    while (!glfwWindowShouldClose(window) && gameRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();

        auto renderElapsed = std::chrono::duration<double>(currentTime - lastRenderTime).count();

        //Renders the screen
        if (renderElapsed >= renderInterval) {

            //Game logic
            snake.Set_Direction(Current_Selected_Direction);
            snake.move();

            //Sees if you died
            if (snake.checkCollision(boardRows, boardCols)) {
                std::cout << "Game Over! Collision detected." << std::endl;
                std::string scoreText = formatScore(snake.Get_Score());
                std::cout << "Your " << scoreText << std::endl;
                gameRunning = false;
                break;
            }
            board.Update_Board();
            
            glClear(GL_COLOR_BUFFER_BIT);
            renderBoard(Background_textureID,board,snake);
            render_Score(snake, fontTextureID);
            glfwSwapBuffers(window);
            lastRenderTime = currentTime;
            
        }

        //Gets user Input
        auto updateElapsed = std::chrono::duration<double>(currentTime - lastUpdateTime).count();
        if (updateElapsed >= updateInterval) {

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (snake.Get_Direction() != 3 && snake.Get_Direction() != 1)) {
                Current_Selected_Direction = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (snake.Get_Direction() != 4 && snake.Get_Direction() != 2)) {
                Current_Selected_Direction = 2;
            }
            else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (snake.Get_Direction() != 1 && snake.Get_Direction() != 3)) {
                Current_Selected_Direction = 3;
            }
            else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (snake.Get_Direction() != 2 && snake.Get_Direction() != 4)) {
                Current_Selected_Direction = 4;
            }
            lastUpdateTime = currentTime;
            
        }

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
