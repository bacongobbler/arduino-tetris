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

#include "tetromino.h"

Tetromino::Tetromino() {
    Tetromino(SQUARE);
}

Tetromino::Tetromino(Shape shape) {
    this->shape = shape;
    switch(this->shape) {
        case STRAIGHT:
            this->cells[1][2] = 1;
            this->cells[2][2] = 2;
            this->cells[3][2] = 1;
            this->cells[4][2] = 1;
            break;
        case SQUARE:
            // NOTE: there's no pivot point for a square.
            this->cells[1][1] = 1;
            this->cells[2][1] = 1;
            this->cells[1][2] = 1;
            this->cells[2][2] = 1;
            break;
        case T:
            this->cells[2][1] = 1;
            this->cells[1][3] = 1;
            this->cells[2][3] = 2;
            this->cells[3][3] = 1;
            break;
        case L:
            this->cells[2][1] = 1;
            this->cells[2][2] = 2;
            this->cells[2][3] = 1;
            this->cells[3][3] = 1;
            break;
        case L_MIRRORED:
            this->cells[2][1] = 1;
            this->cells[2][2] = 2;
            this->cells[2][3] = 1;
            this->cells[1][3] = 1;
            break;
        case SKEW:
            this->cells[1][2] = 1;
            this->cells[2][2] = 2;
            this->cells[2][3] = 1;
            this->cells[3][3] = 1;
            break;
        case SKEW_MIRRORED:
            this->cells[1][3] = 1;
            this->cells[2][3] = 1;
            this->cells[2][2] = 2;
            this->cells[3][2] = 1;
            break;
    }
}

// TODO: rotate pieces using the pivot point.
void Tetromino::Rotate() {
    int newCells[5][5];
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            newCells[y][x] = this->cells[5-x-1][y];
        }
    }

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            this->cells[x][y] = newCells[x][y];
        }
    }
}
