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

#ifndef _FASTERPINIO_H_
#define _FASTERPINIO_H_

//
//	Need access to program memory.
//
#include <Arduino.h>
#include <avr/pgmspace.h>

//
//	Include the Arduino-GPIO code that defines the specific
//	board characteristics.
//
#include "Arduino-GPIO/Board.h"

//
//	We need to modify the Board.h approach to ensuring an
//	atomic action is taken when addressing or modifying a
//	pin.
//
//	The following macro test make the necessary check and
//	returns true if the code is inherently atomic.
//
#define FPIO_IS_ATOMIC(adrs)	(((int)(adrs))<GPIO_REG(GPIO_ATOMIC_MAX))

//
//	Now define our own version of the atomic macro to use
//	(more or less) in place of the Board.h macro.
//
#define FPIO_ATOMIC(adrs,expr)						\
	do {								\
		if( FPIO_IS_ATOMIC( adrs )) {				\
			expr;						\
		}							\
		else {							\
			uint8_t sreg = SREG;				\
			__asm__ __volatile__("cli" ::: "memory");	\
			expr;						\
			SREG = sreg;					\
			__asm__ __volatile__("" ::: "memory");		\
		}							\
	} while( false )


//
//	Note that where static pin definition are specified
//	then direct use of the Arduino-GPIO template class
//	by Mikael Patel will always be the quickest system.
//
//	The following class only seeks to add the thinnest
//	possible layer on top of this to facilitate a more
//	dynamic access mechanism.
//

class FasterPinIO {
private:
	//
	//	The core essence of the class is a static
	//	table which does the translation from an abstract
	//	pin index number to specific memory location and
	//	bit number which can be used against the GPIO
	//	code.
	//
	//	Define the size of the table for each MCU type.
	//
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
	static const byte _io_pins = 20;
#elif defined(__AVR_ATmega32U4__)
	static const byte _io_pins = 24;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	static const byte _io_pins = 70;
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
	static const byte _io_pins = 12;
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	static const byte _io_pins = 6;
#else
#error "FasterPinIO.h: avr mcu not supported"
#endif

	//
	//	Declare the array of IO Pin definitions (size determined
	//	above).
	//
	static const BOARD::pin_t _pin_map[ _io_pins ] PROGMEM;

	//
	//	Now define specifics we need to remember to access
	//	the pin itself.
	//
	struct gpio_register {
		volatile byte pin;
		volatile byte ddr;
		volatile byte port;
	}	*_port_adrs;
	//
	//	The bit set and bit unset masks.
	//
	byte	_bit_set,
		_bit_unset;
	//
	//	Remember the "logical" pin number
	//
	byte	_pin;


public:
	//
	//	Specify the numerical number of the pin at
	//	constructor time (or accept pin 0 as the
	//	selected pin).
	//
	FasterPinIO( byte pin = 0 ) {
		BOARD::pin_t gpio = pgm_read_word_near( &( _pin_map[ pin ]));

		_pin = pin;
		_port_adrs = (struct gpio_register *)GPIO_REG( gpio );
		_bit_set = GPIO_MASK( gpio );
		_bit_unset = ~_bit_set;
	}
	
	FasterPinIO( BOARD::pin_t pin ) {
		for( _pin = 0; _pin < _io_pins; _pin++ ) {
			if( pin == pgm_read_word_near( &( _pin_map[ _pin ]))) break;
		}
		_port_adrs = (struct gpio_register *)GPIO_REG( pin );
		_bit_set = GPIO_MASK( pin );
		_bit_unset = ~_bit_set;
	}
	
	~FasterPinIO() {}
	
	//
	//	Re-assign the pin which this object addresses.
	//
	inline void setPin( byte pin ) {
		BOARD::pin_t gpio = pgm_read_word_near( &( _pin_map[ pin ]));

		_pin = pin;
		_port_adrs = (struct gpio_register *)GPIO_REG( gpio );
		_bit_set = GPIO_MASK( gpio );
		_bit_unset = ~_bit_set;
	}
	
	inline void setPin( BOARD::pin_t pin ) {
		for( _pin = 0; _pin < _io_pins; _pin++ ) {
			if( pin == pgm_read_word_near( &( _pin_map[ _pin ]))) break;
		}
		_port_adrs = (struct gpio_register *)GPIO_REG( pin );
		_bit_set = GPIO_MASK( pin );
		_bit_unset = ~_bit_set;
	}
		
	//
	//	Now the basic primitive actions which pins support.
	//
	inline void input( void ) {
		FPIO_ATOMIC( _port_adrs, _port_adrs->ddr &= _bit_unset );
	}
	
	inline void output( void ) {
		FPIO_ATOMIC( _port_adrs, _port_adrs->ddr |= _bit_set );
	}
	
	inline bool read( void ) {
		return(( _port_adrs->pin & _bit_set ) != 0 );
	}
	
	inline void low( void ) {
		FPIO_ATOMIC( _port_adrs, _port_adrs->port &= _bit_unset );
	}
	
	inline void high( void ) {
		FPIO_ATOMIC( _port_adrs, _port_adrs->port |= _bit_set );
	}
	
	inline void write( bool val ) {
		if( val ) {
			high();
		}
		else {
			low();
		}
	}
	
	inline void toggle( void ) {
		FPIO_ATOMIC( _port_adrs, _port_adrs->port ^= _bit_set );
	}
	
	inline byte pin( void ) {
		return( _pin );
	}
};


#endif

//
//	EOF
//
