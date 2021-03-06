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

	This is a Tetris clone using a few push buttons, an OLED SSD1306(I2C) and an ATMega328P
	(Arduino Uno/Nano).
 */

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "button.h"
#include "game.h"
#include "tetromino.h"

#define SCREEN_WIDTH  128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_ROTATION 1
#define I2C_ADDRESS   0x3C

#define BUTTON_UP 2
#define BUTTON_DOWN 3
#define BUTTON_LEFT 4
#define BUTTON_RIGHT 5
#define BUTTON_A 8

// The display we will be working with to show the contents of the game.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Button b_up(BUTTON_UP);
Button b_down(BUTTON_DOWN);
Button b_left(BUTTON_LEFT);
Button b_right(BUTTON_RIGHT);
Button b_a(BUTTON_A);

bool b_up_last_state;
bool b_down_last_state;
bool b_left_last_state;
bool b_right_last_state;
bool b_a_last_state;

Game game;

unsigned long previousMillis = 0;
const PROGMEM unsigned int interval = 500;

// fishworks logo, converted using https://github.com/javl/image2cpp
const PROGMEM unsigned char fishworksLogo [] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0xff, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf2, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00,
	0x00, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x01, 0xf8, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff,
	0x00, 0x0f, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xfe,
	0x00, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xfc, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xf8,
	0x00, 0x3e, 0x1f, 0xff, 0xff, 0xfe, 0x7f, 0xf8, 0x00, 0x38, 0x03, 0xff, 0xff, 0xdf, 0x3f, 0xf0,
	0x00, 0x21, 0xf0, 0xff, 0xff, 0x0f, 0x3f, 0xe0, 0x00, 0x47, 0xe0, 0x3f, 0xff, 0x0f, 0x3f, 0xc0,
	0x00, 0x4f, 0x80, 0x1f, 0xff, 0x07, 0x3f, 0xc0, 0x00, 0x1f, 0x1f, 0x0f, 0xff, 0x07, 0x3f, 0x80,
	0x00, 0x3f, 0x3f, 0xc3, 0xff, 0x0f, 0x3f, 0x80, 0x00, 0x3e, 0x7f, 0xe1, 0xff, 0x9f, 0x3f, 0x00,
	0x00, 0x7e, 0x7f, 0xf8, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x7c, 0xff, 0xfc, 0x3f, 0xff, 0x3e, 0x00,
	0x00, 0xfc, 0xff, 0xfe, 0x1f, 0xfe, 0x7e, 0x00, 0x00, 0xfc, 0xf9, 0xff, 0x87, 0xfe, 0x7c, 0x00,
	0x01, 0xfc, 0xf0, 0xff, 0xc3, 0xfc, 0xfc, 0x00, 0x01, 0xfc, 0xe0, 0xff, 0xe1, 0xf8, 0xf8, 0x00,
	0x03, 0xfc, 0xe0, 0xff, 0xf8, 0x01, 0xf2, 0x00, 0x03, 0xfc, 0xe0, 0xff, 0xfc, 0x07, 0xe2, 0x00,
	0x07, 0xfc, 0xf0, 0xff, 0xff, 0x0f, 0x84, 0x00, 0x0f, 0xfc, 0xfb, 0xff, 0xff, 0xc0, 0x1c, 0x00,
	0x0f, 0xfe, 0x7f, 0xff, 0xff, 0xf0, 0x7c, 0x00, 0x1f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xf8, 0x00,
	0x3f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x00,
	0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xe0, 0x00,
	0x1f, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0x80, 0x00,
	0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xfe, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x63, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void displayGame() {
	display.clearDisplay();

	// draw surrounding border
	display.drawLine(6, 18, display.width()-6, 18, SSD1306_WHITE);
	display.drawLine(6, 18, 6, display.height()-1, SSD1306_WHITE);
	display.drawLine(display.width()-6, display.height()-1, 6, display.height()-1, SSD1306_WHITE);
	display.drawLine(display.width()-6, display.height()-1, display.width()-6, 18, SSD1306_WHITE);

	// draw scoreboard
	char buf[10];
	sprintf(buf, "%010d", game.score);
	display.setCursor(2, 2);
	display.print(buf);

	// There's a 15px border. 8px to the left, 14px above, 1px below, and 7px to the right.
	// each block is 3px*3px with a 1px margin.
	for (byte x = 0; x < BOARD_WIDTH; x++) {
		for (byte y = 0; y < BOARD_HEIGHT; y++) {
			if (game.board[x][y]) {
				display.fillRect(8+(x*5), 26+(y*5), 4, 4, SSD1306_WHITE);
			}
		}
	}

	for (byte x = 0; x < NUM_CELLS; x++) {
		for (byte y = 0; y < NUM_CELLS; y++) {
			if (game.tet.cells[x][y]) {
				display.fillRect(8+((game.mPosX+x)*5), 26+((game.mPosY+y)*5), 4, 4, SSD1306_WHITE);
			}
		}
	}

	display.display();
}

void setup() {
	randomSeed(analogRead(0));
	game.ClearBoard();

	display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);
	display.setRotation(OLED_ROTATION);
	display.setTextSize(1);
	display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
	display.clearDisplay();

	display.drawBitmap(0, 0, fishworksLogo, display.width(), display.height(), 1);
	display.setCursor(6, display.height()-16);
	display.println(F("FISHWORKS"));
	display.display();
	delay(2000);
	displayGame();
}

void loop() {
	unsigned long currentMillis = millis();
	b_up.Update();
	b_down.Update();
	b_left.Update();
	b_right.Update();
	b_a.Update();

	if (game.IsGameOver()) {
		display.setCursor(6, display.height()-16);
		display.print(F("GAME OVER"));
		display.display();
		delay(60000);
		return;
	}

	if (b_up.IsPressed() && b_up_last_state == LOW) {
		game.RotateTetromino();
		displayGame();
	}
	// TODO: allow a soft drop if button continues to be held down
	if (b_down.IsPressed() && b_down_last_state == LOW) {
		game.SoftDrop();
		displayGame();
	}
	if (b_left.IsPressed() && b_left_last_state == LOW) {
		game.MoveLeft();
		displayGame();
	}
	if (b_right.IsPressed() && b_right_last_state == LOW) {
		game.MoveRight();
		displayGame();
	}
	if (b_a.IsPressed() && b_a_last_state == LOW) {
		game.HardDrop();
		displayGame();
	}

	if (currentMillis - previousMillis > interval) {
		previousMillis = currentMillis;
		game.SoftDrop();
		displayGame();
	}

	// update *_last_state for the next iteration
	if (b_up.IsPressed() != b_up_last_state) {
		b_up_last_state = !b_up_last_state;
	}
	if (b_down.IsPressed() != b_down_last_state) {
		b_down_last_state = !b_down_last_state;
	}
	if (b_left.IsPressed() != b_left_last_state) {
		b_left_last_state = !b_left_last_state;
	}
	if (b_right.IsPressed() != b_right_last_state) {
		b_right_last_state = !b_right_last_state;
	}
	if (b_a.IsPressed() != b_a_last_state) {
		b_a_last_state = !b_a_last_state;
	}
}
