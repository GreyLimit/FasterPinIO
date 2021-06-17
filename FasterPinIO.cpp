///
///	FasterPinIO - A faster system for dynamic access to Arduino pins
///
///	Copyright (C) 2021  Jeff Penfold, jeff.penfold@googlemail.com
///
///	This program is free software: you can redistribute it and/or modify
///	it under the terms of the GNU General Public License as published by
///	the Free Software Foundation, either version 3 of the License, or
///	(at your option) any later version.
///
///	This program is distributed in the hope that it will be useful,
///	but WITHOUT ANY WARRANTY; without even the implied warranty of
///	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///	GNU General Public License for more details.
///
///	You should have received a copy of the GNU General Public License
///	along with this program.  If not, see <https://www.gnu.org/licenses/>.
///

//
//	A Faster Pin IO system
//
//	A class to front then the high speed Pin IO
//	template class written by Mikael Patel.
//
//	See: https://github.com/mikaelpatel/Arduino-GPIO
//

#include "FasterPinIO.h"

//
//	We define the static constant tables that are placed into
//	the program memory.
//
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
static const BOARD::pin_t FasterPinIO::_pin_map[ FasterPinIO::_io_pins ] PROGMEM = {
	BOARD::D0,  BOARD::D1,  BOARD::D2,  BOARD::D3,  BOARD::D4,
	BOARD::D5,  BOARD::D6,  BOARD::D7,  BOARD::D8,  BOARD::D9,
	BOARD::D10, BOARD::D11, BOARD::D12, BOARD::D13, BOARD::D14,
	BOARD::D15, BOARD::D16, BOARD::D17, BOARD::D18, BOARD::D19
};

#elif defined(__AVR_ATmega32U4__)
static const BOARD::pin_t FasterPinIO::_pin_map[ FasterPinIO::_io_pins ] PROGMEM = {
	BOARD::D0,  BOARD::D1,  BOARD::D2,  BOARD::D3,  BOARD::D4,
	BOARD::D5,  BOARD::D6,  BOARD::D7,  BOARD::D8,  BOARD::D9,
	BOARD::D10, BOARD::D11, BOARD::D12, BOARD::D13, BOARD::D14,
	BOARD::D15, BOARD::D16, BOARD::D17, BOARD::D18, BOARD::D19,
	BOARD::D20, BOARD::D21, BOARD::D22, BOARD::D23
};

#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
static const BOARD::pin_t FasterPinIO::_pin_map[ FasterPinIO::_io_pins ] PROGMEM = {
	BOARD::D0,  BOARD::D1,  BOARD::D2,  BOARD::D3,  BOARD::D4,
	BOARD::D5,  BOARD::D6,  BOARD::D7,  BOARD::D8,  BOARD::D9,
	BOARD::D10, BOARD::D11, BOARD::D12, BOARD::D13, BOARD::D14,
	BOARD::D15, BOARD::D16, BOARD::D17, BOARD::D18, BOARD::D19,
	BOARD::D20, BOARD::D21, BOARD::D22, BOARD::D23, BOARD::D24,
	BOARD::D25, BOARD::D26, BOARD::D27, BOARD::D28, BOARD::D29,
	BOARD::D30, BOARD::D31, BOARD::D32, BOARD::D33, BOARD::D34,
	BOARD::D35, BOARD::D36, BOARD::D37, BOARD::D38, BOARD::D39,
	BOARD::D40, BOARD::D41, BOARD::D42, BOARD::D43, BOARD::D44,
	BOARD::D45, BOARD::D46, BOARD::D47, BOARD::D48, BOARD::D49,
	BOARD::D50, BOARD::D51, BOARD::D52, BOARD::D53, BOARD::D54,
	BOARD::D55, BOARD::D56, BOARD::D57, BOARD::D58, BOARD::D59,
	BOARD::D60, BOARD::D61, BOARD::D62, BOARD::D63, BOARD::D64,
	BOARD::D65, BOARD::D66, BOARD::D67, BOARD::D68, BOARD::D69
};

#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
static const BOARD::pin_t FasterPinIO::_pin_map[ FasterPinIO::_io_pins ] PROGMEM = {
	BOARD::D0,  BOARD::D1,  BOARD::D2,  BOARD::D3,  BOARD::D4,
	BOARD::D5,  BOARD::D6,  BOARD::D7,  BOARD::D8,  BOARD::D9,
	BOARD::D10, BOARD::D11
};

#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
static const BOARD::pin_t FasterPinIO::_pin_map[ FasterPinIO::_io_pins ] PROGMEM = {
	BOARD::D0,  BOARD::D1,  BOARD::D2,  BOARD::D3, BOARD::D4,
	BOARD::D5,  BOARD::D6
};

#else
#error "FasterPinIO.cpp: MCU not supported"
#endif

//
//	EOF
//
