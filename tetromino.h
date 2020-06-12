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

#ifndef _TETROMINO_
#define _TETROMINO_

#include <Arduino.h>

#define NUM_CELLS 5

enum Shape {
	STRAIGHT = 1,
	SQUARE = 2,
	T = 3,
	L = 4,
	L_MIRRORED = 5,
	SKEW = 6,
	SKEW_MIRRORED = 7,
};

/*
	A tetromino is a geometric shape composed of four squares, connected orthogonally (i.e. at the
	edges and not the corners).

	These are the geometric shapes that make up the game.
 */
class Tetromino {
	public:
		Tetromino();
		Tetromino(Shape shape);
		void Rotate();
		Shape shape;
		bool cells[NUM_CELLS][NUM_CELLS] = {};
};

#endif // _TETROMINO_
