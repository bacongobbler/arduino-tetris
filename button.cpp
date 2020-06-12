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
#include "button.h"

Button::Button(byte pin) {
	this->pin = pin;
	pinMode(pin, INPUT);
}

void Button::Update() {
	byte reader = digitalRead(this->pin);

	// filter out any noise by using a time buffer
	if ( (millis() - this->lastDebounceTime) > DEBOUNCE_DELAY) {
		this->lastState = this->currentState;
		this->currentState = reader;
		this->lastDebounceTime = millis();
	}
}

bool Button::IsPressed() {
	return (this->currentState == HIGH);
}
