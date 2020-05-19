/*
    Copyright Matthew Fisher <matt.fisher@fishworks.io>.
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */

#include <stdlib.h>
#include "game.h"

Game::Game() {
    ClearBoard();
}

void Game::ClearBoard() {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            this->board[x][y] = POS_FREE;
        }
    }
    NewTetromino();
    NewTetromino();
}

bool Game::IsGameOver() {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (this->board[x][0] == POS_FILLED) {
            return true;
        }
    }
    return false;
}

void Game::StoreTetromino() {
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            if (this->tet.cells[x][y] > 0) {
                this->board[this->mPosX+x][this->mPosY+y] = POS_FILLED;
            }
        }
    }
    NewTetromino();
}

void Game::DeleteLines() {
    int numFilledRows;
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        bool rowFilled = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (this->board[x][y] == POS_FREE) {
                rowFilled = false;
            }
        }
        if (rowFilled) {
            // increment a counter; the number of lines cleared in one shot will multiply the score.
            numFilledRows++;

            // clear the row
            for (int x = 0; x < BOARD_WIDTH; x++) {
                this->board[x][y] = POS_FREE;
            }

            // drop each row above down
            // base case: we can safely assume that y != 0 here as IsGameOver() would've returned true, and the game would end.
            for (int yy = y-1; yy >= 0; yy--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    this->board[x][yy] = this->board[x][yy+1];
                    this->board[x][yy] = POS_FREE;
                }
            }
        }
    }

    // calculate the score. The maximum rows that can be cleared at once is 4 (a Tetris). Score is multiplied by the level.
    switch(numFilledRows) {
        case 1:
            this->score += 40 * this->level;
            break;
        case 2:
            this->score += 100 * this->level;
            break;
        case 3:
            this->score += 300 * this->level;
            break;
        default:
            this->score += 1200 * this->level;
            break;
    }
}

void Game::MoveLeft() {
    if (!IsCollision(this->mPosX-1, this->mPosY)) {
        this->mPosX--;
    }
}

void Game::MoveRight() {
    if (!IsCollision(this->mPosX+1, this->mPosY)) {
        this->mPosX++;
    }
}

void Game::SoftDrop() {
    if (!IsCollision(this->mPosX, this->mPosY+1)) {
        this->mPosY++;
        return;
    }
    // Score 1 point for each soft drop.
    this->score++;

    StoreTetromino();
}

void Game::HardDrop() {
    int counter;
    while (!IsCollision(this->mPosX, this->mPosY+1)) {
        this->mPosY++;
        counter++;
    }
    // A hard drop is more risky than a soft drop, so reward the player for pulling one off.
    this->score += 5*counter;

    StoreTetromino();
}

bool Game::IsCollision(int posX, int posY) {
    // base case: if the tile will be clearly out of the board's boundaries, we return true
    if (posX < -3 || posX >= BOARD_WIDTH-2 || posY < -3 || posY >= BOARD_HEIGHT-2) {
        return true;
    }
    /*
        For Tetris, a simple collision detector would be to make a game board surrounded with a
        solid border. We need a wall with a width of 2, a ceiling with a height of 2, and a floor
        (hence why we need our collosion board to be +4 for length and +3 for height).

        When we add the tetromino to the game board, we add 1 to the cells where a filled position
        in the teronimo matrix would fit in this collision matrix. We can then check and compare the
        value of each row/column. If any value is greater than 1, we have a collision.
     */
    int collisionBoard[BOARD_WIDTH+4][BOARD_HEIGHT+3];
    for (int y = 0; y < BOARD_HEIGHT+3; y++) {
        for (int x = 0; x < BOARD_WIDTH+4; x++) {
            // fill the border
            if (x == 0 || x == 1 || x == BOARD_HEIGHT+2 || x == BOARD_HEIGHT+3 || y == 0 || y == 1 || y == BOARD_HEIGHT+2) {
                collisionBoard[x][y] = 1;
            }

            // fill the collision board with all the tiles filled from the game board
            if (x > 1 && x <= BOARD_WIDTH+1 && y > 1 && y <= BOARD_HEIGHT+1) {
                collisionBoard[x][y] = this->board[x-2][y-2];
            }
        }
    }

    // add the tetromino to the collision board
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            if (this->tet.cells[x][y] > 0) {
                collisionBoard[posX+x][posY+y]++;
            }
        }
    }

    // one last iteration over the collision board. If a value is greater than 1, we have detected a collision.
    for (int y = 0; y < BOARD_HEIGHT+3; y++) {
        for (int x = 0; x < BOARD_WIDTH+4; x++) {
            if (collisionBoard[x][y] > 1) {
                return true;
            }
        }
    }
    return false;
}

void Game::NewTetromino() {
    this->tet = this->next;
    this->next = Tetromino(static_cast<Shape>((rand() % (SHAPE_MAX-1))+1));
    // set mPosX and mPosY to offset the tetromino.
    switch(this->tet.shape) {
        default:
            this->mPosX = 0;
            this->mPosY = 0;
            break;
    }
}
