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

#include <Arduino.h>
#include "game.h"
#include "tetromino.h"

Game::Game() {
	ClearBoard();
}

void Game::ClearBoard() {
	for (byte x = 0; x < BOARD_WIDTH; x++) {
		for (byte y = 0; y < BOARD_HEIGHT; y++) {
			board[x][y] = false;
		}
	}
	NewTetromino();
	NewTetromino();
}

bool Game::IsGameOver() {
	for (byte x = 0; x < BOARD_WIDTH; x++) {
		if (board[x][0]) {
			return true;
		}
	}
	return false;
}

void Game::StoreTetromino() {
	for (byte x = 0; x < NUM_CELLS; x++) {
		for (byte y = 0; y < NUM_CELLS; y++) {
			if (tet.cells[x][y]) {
				board[mPosX+x][mPosY+y] = true;
				score++;
			}
		}
	}
	NewTetromino();
}

void Game::DeleteLines() {
	byte numFilledRows;
	for (byte y = 0; y < BOARD_HEIGHT; y++) {
		bool rowFilled = true;
		for (byte x = 0; x < BOARD_WIDTH; x++) {
			if (!board[x][y]) {
				rowFilled = false;
			}
		}
		if (rowFilled) {
			// increment a counter; the number of lines cleared in one shot will multiply the score.
			numFilledRows++;

			// clear the row
			for (int x = 0; x < BOARD_WIDTH; x++) {
				board[x][y] = false;
			}

			// drop each row above down
			// base case: we can safely assume that y != 0 here as IsGameOver() would've returned true, and the game would end.
			for (int yy = y-1; yy >= 0; yy--) {
				for (byte x = 0; x < BOARD_WIDTH; x++) {
					board[x][yy+1] = board[x][yy];
					board[x][yy] = false;
				}
			}
		}
	}

	// calculate the score. The maximum rows that can be cleared at once is 4 (a Tetris). Score is multiplied by the level.
	switch(numFilledRows) {
		case 1:
			score += 40 * level;
			break;
		case 2:
			score += 100 * level;
			break;
		case 3:
			score += 300 * level;
			break;
		default:
			score += 1200 * level;
			break;
	}
}

void Game::MoveLeft() {
	if (!IsCollision(mPosX-1, mPosY)) {
		mPosX--;
	}
}

void Game::MoveRight() {
	if (!IsCollision(mPosX+1, mPosY)) {
		mPosX++;
	}
}

void Game::RotateTetromino() {
	tet.Rotate();
	if (IsCollision(mPosX, mPosY)) {
		// rotate back to its original position.
		tet.Rotate();
		tet.Rotate();
		tet.Rotate();
	}
}

void Game::SoftDrop() {
	// Score 1 point for each soft drop.
	score++;

	if (!IsCollision(mPosX, mPosY+1)) {
		mPosY++;
		return;
	}

	StoreTetromino();
	DeleteLines();
}

void Game::HardDrop() {
	while (!IsCollision(mPosX, mPosY+1)) {
		mPosY++;
		// A hard drop is more risky than a soft drop, so reward the player for pulling one off.
		score += 2;
	}

	StoreTetromino();
	DeleteLines();
}

bool Game::IsCollision(int posX, int posY) {
	// try adding each cell of the tetromino to the board.
	// If the value at the cell in the board is true or we found a cell that is out of bounds,
	// we found a collision.
	for (byte x = 0; x < NUM_CELLS; x++) {
		for (byte y = 0; y < NUM_CELLS; y++) {
			if(tet.cells[x][y]) {
				if (posX+x < 0 || posX+x >= BOARD_WIDTH || posY+y < -2 || posY+y >= BOARD_HEIGHT) {
					return true;
				}
				if (board[posX+x][posY+y]) {
					return true;
				}
			}
		}
	}
	return false;
}

void Game::NewTetromino() {
	tet = next;
	next = Tetromino(static_cast<Shape>(random(7)+1));
	mPosX = 2;
	mPosY = -2;
}
