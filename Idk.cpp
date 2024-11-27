#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include <deque>
#include <stb/stb_image.h>
#include "Snake.h"
#include "Board.h"
#include "Body_Part.h"
#include <chrono>

// Window dimensions
const int windowWidth = 800;
const int windowHeight = 800;

// Board dimensions
const int boardRows = 20;
const int boardCols = 20;

// Cell dimensions
const float cellWidth = windowWidth / static_cast<float>(boardCols);
const float cellHeight = windowHeight / static_cast<float>(boardRows);

void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void render(Body_Part& part) {
    glBindTexture(GL_TEXTURE_2D, part.getTextureID());

    // Push the current matrix onto the stack
    glPushMatrix();

    // Translate to the correct grid position
    float x = part.getPosition().second * cellWidth;  // Column
    float y = part.getPosition().first * cellHeight;  // Row
    glTranslatef(x + cellWidth / 2, y + cellHeight / 2, 0.0f);

    // Apply rotation if needed
    switch (part.getDirection()) {
    case 1: glRotatef(0.0f, 0.0f, 0.0f, 1.0f); break;   // Up
    case 2: glRotatef(90.0f, 0.0f, 0.0f, 1.0f); break;  // Right
    case 3: glRotatef(180.0f, 0.0f, 0.0f, 1.0f); break; // Down
    case 4: glRotatef(270.0f, 0.0f, 0.0f, 1.0f); break; // Left
    }

    // Render the quad with the texture applied
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-cellWidth / 2, -cellHeight / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(cellWidth / 2, -cellHeight / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(cellWidth / 2, cellHeight / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-cellWidth / 2, cellHeight / 2);
    glEnd();

    // Restore the matrix
    glPopMatrix();
}

void renderBoard(Board& board,Snake& snake) {
    std::vector<std::vector<char>> Snake_Board = board.get_Board();

    // Draw board background
    glBindTexture(GL_TEXTURE_2D, 0);  // Disable any active textures
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(windowWidth, 0);
    glVertex2f(windowWidth, windowHeight);
    glVertex2f(0, windowHeight);
    glEnd();

    
    
    for (int row = 0; row < Snake_Board.size(); ++row) {
        for (int col = 0; col < Snake_Board[row].size(); ++col) {
            char cell = Snake_Board[row][col];

            if (cell == 'F') {  // Food
                drawRect(col * cellWidth, row * cellHeight, cellWidth, cellHeight, 1,0,0);
            }

            // Draw the cell
            
        }
    }

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    const std::deque<std::unique_ptr<Body_Part>>& Snake_Body_Parts = snake.getParts();
    for (const auto& Snake_Piece : Snake_Body_Parts) {
        render(*Snake_Piece);
    }
    

    
}



int main() {
    Snake snake({ boardRows / 2, boardCols / 2 }, 2, 3);
    Board board(boardRows, boardCols, snake);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Snake Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f); 
    glOrtho(0.0f, windowWidth, windowHeight, 0.0f, -1.0f, 1.0f);

    bool gameRunning = true;

    // Timers for game logic and rendering
    auto lastRenderTime = std::chrono::high_resolution_clock::now();
    auto lastUpdateTime = std::chrono::high_resolution_clock::now();

    const double renderInterval = 1.0 / 12.0; // 60 FPS
    const double updateInterval = 1.0 / 20.0; // Snake updates at 15 FPS
    int Current_Selected_Direction = snake.Get_Direction();
    while (!glfwWindowShouldClose(window) && gameRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();

        // Handle rendering at 60 FPS
        auto renderElapsed = std::chrono::duration<double>(currentTime - lastRenderTime).count();
        if (renderElapsed >= renderInterval) {
            // Game logic
            snake.Set_Direction(Current_Selected_Direction);
            snake.move();
            if (snake.checkCollision(boardRows, boardCols)) {
                std::cout << "Game Over! Collision detected." << std::endl;
                gameRunning = false;
                break;
            }
            
            board.Update_Board();
            
            glClear(GL_COLOR_BUFFER_BIT);
            renderBoard(board,snake);
            glfwSwapBuffers(window);
            lastRenderTime = currentTime;
            
        }

        // Handle game logic (snake movement) at the desired update rate
        auto updateElapsed = std::chrono::duration<double>(currentTime - lastUpdateTime).count();
        if (updateElapsed >= updateInterval) {
            // Input handling
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

        // Poll for input events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
