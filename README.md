# PIC32Device
Common base classes for PIC32 based projects using a simple serial command protocol.

## Logical pins
For each board the serial command protocol defines logical pins.

## Levels
Each logical pin has a level (0-255) which can be updated by the protocol.
This level could be a volume, input channel, light level appropriate to the application.

## Readings
Each logical pin may have some readings that can be reported from it.
These are things like voltage, current, temperature.

# ControlledDevice Class

This class is a base from which device specific classes should be derived.
It provides public methods to:
- Initialise the device
- SetLevel - sets level for the device.
- SendReading - sends a reading from the device to an IODevice, such as a UART.

# IODevice Class

This class is a base from which peripheral specific classes should be derived.
Currently there is one derived class UART1
This class publicly exposes simple character IO functions:
- Write
- HasChar
- GetChar
- PutChar

## UART1 Class

This class implements IODevice for UART1 on the PIC32.
In addition to implementing the base class functions it also implements:
- Initialise, which allows the baud rate to be set. 

A global circular buffer is declared in UART1.cpp, a character is added to the tail of the buffer by an interrupt service routine _IntUartHandler, which is called when an char is received. 
HasChar tests for a character at the head of the buffer.
GetChar gets the character at the head of the buffer.
PutChar writes a single character to the UART.
Write calls PutChar multiple times to write to the UART.

# CommandParser class

This class parses commands received from an IODevice and calls the SetLevel or SendReading methods on the server to execute commands.
Service implements a state machine which acts on the next available character from IODevice.
SendReady sends a standard ready prompt to IODevice, which includes the board ID, which it gets by calling GetBoardID.

# Server class

Server is a base class from which a project specific server class should be derived.
It maintains an array of controlled devices, the array index for a pin is one less than its logical pin number.

AddDevice adds a controlled device to the array.
SetLevel sets the level of the device associated with a logical pin.
SendReading reports a reading for the device associated with a logical pin.
Run performs initialisation and executes the service loop.
InitialiseDevices should be overridden and should initialise and then add any member devices.

server.h defines extern "C" RunServer, but server.cpp does not implement it. This enables a generic main.c to be used. 
RunServer should be implemented in the .cpp file of a derived class.

# Defining a project for a new board


