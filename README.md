# FasterPinIO
A faster mechanism for dynamic access to Arduino PINs


A class to front end the high speed Pin IO template class written by Mikael Patel.

See: https://github.com/mikaelpatel/Arduino-GPIO

Elementary usage:

Define a pin with:

    FasterPinIO name;
    
In setup (or other executing part of the code) use:

    name.setPin( X );
    name.output();
    name.low();
    
Where "X" is the Arduino logical number of the pin.  Then the pin can be written with either:

    name.high();
or

    name.low();
    
This (when compared with digitalWrite using an oscillascope) looks to take 1/2 the time of the
standard API.  The variable can be re-assigned to another pin using setPin again.

The pin can be read using:

    name.read()

There are other elements to the class, but it isn't that big.

AVR only for thhe moment.
