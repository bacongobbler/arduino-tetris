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
#include "tetromino.h"

Tetromino::Tetromino() {
	this->shape = Shape::SQUARE;
	this->cells[1][1] = true;
	this->cells[2][1] = true;
	this->cells[1][2] = true;
	this->cells[2][2] = true;
}

Tetromino::Tetromino(Shape shape) {
	this->shape = shape;
	switch(this->shape) {
		case STRAIGHT:
			this->cells[1][2] = true;
			this->cells[2][2] = true;
			this->cells[3][2] = true;
			this->cells[4][2] = true;
			break;
		case SQUARE:
			this->cells[1][1] = true;
			this->cells[2][1] = true;
			this->cells[1][2] = true;
			this->cells[2][2] = true;
			break;
		case T:
			this->cells[2][2] = true;
			this->cells[1][3] = true;
			this->cells[2][3] = true;
			this->cells[3][3] = true;
			break;
		case L:
			this->cells[2][1] = true;
			this->cells[2][2] = true;
			this->cells[2][3] = true;
			this->cells[3][3] = true;
			break;
		case L_MIRRORED:
			this->cells[2][1] = true;
			this->cells[2][2] = true;
			this->cells[2][3] = true;
			this->cells[1][3] = true;
			break;
		case SKEW:
			this->cells[1][2] = true;
			this->cells[2][2] = true;
			this->cells[2][3] = true;
			this->cells[3][3] = true;
			break;
		case SKEW_MIRRORED:
			this->cells[1][3] = true;
			this->cells[2][3] = true;
			this->cells[2][2] = true;
			this->cells[3][2] = true;
			break;
	}
}

void Tetromino::Rotate() {
	bool newCells[NUM_CELLS][NUM_CELLS] = {};
	for (byte x = 0; x < NUM_CELLS; x++) {
		for (byte y = 0; y < NUM_CELLS; y++) {
			newCells[y][x] = this->cells[NUM_CELLS-x-1][y];
		}
	}

	for (byte x = 0; x < NUM_CELLS; x++) {
		for (byte y = 0; y < NUM_CELLS; y++) {
			this->cells[x][y] = newCells[x][y];
		}
	}
}
