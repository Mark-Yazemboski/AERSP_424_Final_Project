#include <windows.h> // For GetAsyncKeyState
#include "Snake.h"
#include "Board.h"
#include <iostream>
#include <thread> // For sleeping

int main() {
    const int boardRows = 20;
    const int boardCols = 20;

    Snake snake({boardRows / 2, boardCols / 2}, 2, 3); // Start in the middle, moving right
    Board board(boardRows, boardCols, snake);

    bool gameRunning = true;

    while (gameRunning) {
        // Check for key inputs (non-blocking)
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            snake.Set_Direction(1); // Up
        } else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            snake.Set_Direction(2); // Right
        } else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            snake.Set_Direction(3); // Down
        } else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            snake.Set_Direction(4); // Left
        }

        // Move the snake
        snake.move();

        // Check for collisions
        if (snake.checkCollision(boardRows, boardCols)) {
            std::cout << "Game Over! Collision detected." << std::endl;
            gameRunning = false;
            break;
        }

        // Update and display the board
        board.Update_Board();
        board.printBoard();

        // Delay to control game speed
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
