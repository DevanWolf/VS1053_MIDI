/**
 * Realime USB Serial MIDI Synth
 * 
 * Turns your Arduino UNO into a fully working standalone USB/Serial General MIDI synth!
 * Use the RESET button on the Arduino UNO to re-initialize the MIDI synth.
 * 
 * Hardware:
 * - Arduino UNO (or compatible)
 * - VS1053b Module (e.g. Waveshare Music Shield)
 * 
 * Pin Connections (Arduino UNO):
 * - MISO: Pin 12
 * - MOSI: Pin 11	
 * - SCK: Pin 13
 * - DREQ: Pin A1
 * - XRST: Pin A0
 * - XCS: Pin A3
 * - XDCS: Pin A2
 *
 * Software:
 * - EA Serial MIDI Bridge (https://github.com/ezequielabregu/EA-serialmidi-bridge/releases),
 * Hairless MIDI<->Serial (https://github.com/tyan0/hairless-midiserial/releases),
 * or Serial <> MIDI Converter (https://www.spikenzielabs.com/learn/serial_midi.html)
 * - loopMIDI (for Windows users) (https://tobias-erichsen.de/software/loopmidi.html)
 *
 * Author: Sobble Entertainment Inc.
 */

#include <SPI.h>
#include <VS1053_MIDI.h>

#define REVERB_TOGGLE 5 // mapped to Play/Pause button
#define VOLUME_UP 7
#define VOLUME_DOWN 3
#define LED 8 // use LED_BUILTIN (13) if the music shield doesn't have an LED

byte cmd;
char value = -1;
byte volume;
bool reverb;
bool buttonPressed = !digitalRead(REVERB_TOGGLE);

// Create VS1053_MIDI instance
VS1053_MIDI midi(A3, A2, A1, A0); // Pins for XCS, XDCS, DREQ, and XRESET respectively

void setup() {
	pinMode(LED, OUTPUT);

	// Initialize SPI
	SPI.begin();
	SPI.setClockDivider(buttonPressed?SPI_CLOCK_DIV16:SPI_CLOCK_DIV8); // hold the button down upon powering on to enter safe mode
	midi.begin(true);

	Serial.begin(38400); // Serial MIDI standard baud rate
}

void loop() {
	while (Serial.available()) {
		byte data = Serial.read();
		if ((data & 0xE0) == 0x80 or (data & 0xF0) == 0xB0 or (data & 0xF0) == 0xC0 or (data & 0xF0) == 0xE0) cmd = data;
		else if ((data & 0x80) == 0 and ((cmd & 0xE0) == 0x80 or (cmd & 0xF0) == 0xB0 or (cmd & 0xF0) == 0xC0 or (cmd & 0xF0) == 0xE0)) {
				if ((cmd & 0xF0) == 0xC0) midi.sendMIDI(cmd, data);
				else if (value < 0) value = data;
				else {
           if ((cmd & 0xF0) == 0x80) midi.sendMIDI(cmd, value, 0); // no note off velocity support
					else if (!((cmd & 0xF0) == 0xB0 and ((value >= 1 and value <= 5) or value == 8 or value == 9 or (value >= 13 and value <= 37) or value == 65 or (value >= 67 and value <= 90) or (value >= 92 and value <= 97) or (value >= 102 and value <= 119) or value == 122 or value == 126 or value == 127))) midi.sendMIDI(cmd, value, data); // filter out unassigned CCs
					value = -1;
				}
		} else if (cmd > 0) {
			cmd = 0;
			value = -1;
		}
	}
	if (!digitalRead(REVERB_TOGGLE) and !buttonPressed) {
		buttonPressed = true;
		digitalWrite(LED, HIGH);
		reverb = !reverb;
		midi.writeRegister(7, 0x1e03);
		midi.writeRegister(6, (byte)reverb);
		digitalWrite(LED, LOW);
	} else if (digitalRead(REVERB_TOGGLE) and buttonPressed) buttonPressed = false;
	else if (!digitalRead(VOLUME_UP) and volume < 255) {
		digitalWrite(LED, HIGH);
		++volume;
		midi.setMasterVolume(volume, volume);
		digitalWrite(LED, LOW);
		delay(10);
	} else if (!digitalRead(VOLUME_DOWN) and volume > 0) {
		digitalWrite(LED, HIGH);
		--volume;
		midi.setMasterVolume(volume, volume);
		digitalWrite(LED, LOW);
		delay(10);
	}
}