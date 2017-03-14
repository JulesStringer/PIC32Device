# PIC32Device
Common base classes for PIC32 based projects using a simple serial command protocol.

# ControlledDevice Class

This class is a base from which device specific classes should be derived.
It provides public methods to:
- Initialise the device
- SetLevel - sets the exposed property of the device, such as volume or input channel - it is assumed that devices only have one property.
- SendReading - sends a reading from the device to an IODevice, such as a UART.

# IODevice Class

This class is a base from which peripheral specific classes should be derived.
Currently there is one derived class UART1
This class publicly exposes simple character IO functions:
- Write
- HasChar
- GetChar
- PutChar

# UART1 Class

This class implements IODevice for UART1 on the PIC32.
In addition to implementing the base class functions it also implements:
- Initialise, which allows the baud rate to be set. 

A global circular buffer is declared in UART1.cpp, which is added to by an interrupt service routine _IntUartHandler 


# Server class

Server is a base class from which a project specific server class should be derived.
It maintains an array of devices and handles 
