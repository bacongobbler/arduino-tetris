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

#ifndef _BUTTON_
#define _BUTTON_

#include <Arduino.h>

// button debounce delay; increase if the output flickers.
#define DEBOUNCE_DELAY 50

class Button {
	public:
		Button(byte pin);
		void Update();
		bool IsPressed();
	private:
		byte pin;
		byte currentState = LOW;
		byte lastState = LOW;
		unsigned long lastDebounceTime;
};

#endif // _BUTTON_
