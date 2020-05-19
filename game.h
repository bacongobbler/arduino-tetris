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

#ifndef _GAME_
#define _GAME_

#include "tetromino.h"

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10

class Game {
    public:
        Game();
        void ClearBoard(); // free up the board, resetting for the next game/level.
        bool IsGameOver(); // call this before dropping the next tetromino. The game is considered over if a tetromino filled a block on row 0 (the top row).
        void StoreTetromino(); // fill board with the tetromino.
        void DeleteLines(); // clear the board of any rows which are filled and add to the score.
        void MoveLeft(); // move the tetromino one position to the left. If it would collide with the edge or with another tetromino, do nothing.
        void MoveRight(); // move the tetromino one position to the right. If it would collide with the edge or with another tetromino, do nothing.
        void SoftDrop(); // move the current tetromino one position down. If it collides with another tetromino or the edge, do not move the tetromino; instead SoftDrop() will call StoreTetromino().
        void HardDrop(); // move the current tetromino down to the bottom of the board. After it collides with another tetromino or the edge, this will call StoreTetromino().

    private:
        enum { POS_FREE, POS_FILLED };
        // The game board. Each position is either POS_FREE or POS_FILLED.
        //
        // internally, the board's origin point is the top-left of the board, such that 1,0 is the top row, second column to the left.
        int board[BOARD_WIDTH][BOARD_HEIGHT];
        unsigned long score;
        unsigned long level;
        int mPosX, mPosY; // position of the tetromino currently falling down
        Tetromino tet; // the tetromino currently falling down
        Tetromino next; // the next piece to be falling
        void NewTetromino();
        // check if moving the tetromino to position (x,y) on the board would result in a collision.
        //
        // A collision is considered when the tetromino touches a position that is already filled or if it moves off-screen.
        bool IsCollision(int posX, int posY);
};

#endif // _GAME_
