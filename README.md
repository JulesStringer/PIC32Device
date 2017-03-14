# PIC32Device
Common base classes for PIC32 based projects using a simple serial command protocol.

This is currently used by the AnalogAudio and BiStableRelays projects.

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

A protected method SendReading is provided that formats a reading and writes it to IODevice.

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

# Tick.c
This file derives from the microchip TCPIP stack.
It has been modified to provide GetTickCount, which gives the number of milliseconds since the processor started.

# Utils.c
This just provides the function HexChar which encodes a number(0 to 15) as a hexadecimal digit.

# Hardware.c
This provides implementations of some functions required to interface to the hardware:
_DefaultInterrupt - default interrupt handler which breaks in debug, but resets in release.
INTEnableInterrupts
INTEnableSystemMultiVectoredInt
InitialiseBoard - sets up the oscillator and configures TX1 as digital output, RX1 as digital input, then enables interrupts.
InitialiseSPI2 - initialises SPI2
WriteSPI2 - writes an unsigned long to SPI2

TODO - further abstraction as SPI2 is remappable and specific pin assignments should not be referenced in here.

# Defining a project for a new board

## Define pin assignments in pindefs.h

pindefs.h should contain #defines for application functions in terms of related pins.
This facilitates changing these assignments for different board versions.
If needed #if should be used to conditionally compile dependent on board version.

// Multiplexer channel control
#define CH1_MUXB_LAT     (LATBbits.LATB2)
#define CH1_MUXB_TRIS    (TRISBbits.TRISB2)
#define CH1_MUXB_ANSEL   (ANSELBbits.ANSB2)
#define CH1_MUXB_PORT    (PORTBbits.RB2)

## Define boarddef.h

This must define SWITCH_ID , which is the unique ID of the current board. This will need to be edited when a new board is programmed.

It may also define other constants that control compilation depending on SWITCH_ID

## Derive a class for each controlled device
For each device that needs to be controlled by SetLevel or read by SendReading, a class should be derived from ControlledDevice.

The implementation of SetLevel and SendReading should handle the specifics of the device.

### Example MC14052B multiplexer class
For example AnalogAudio has two MC14052B Analog Multiplexers for channel selection. 
A class MC14052B is derived from ControlledDevice.

The Initialise function should call the base implementation ControlledDevice::Initialise(nChannel), which sets m_nDevice.
It should then configure the pins used to control the multiplexer.

The SetLevel function sets the control pins on the multiplexer by outputting to the associated 2 pins on the microcontroller. The pins used will depend on m_nDevice.

The SendReading function in this case returns the level by testing the control pins of the multiplexer.

## Derive the server class
A server class should be derived from Server.

Define members for each controlled device.

e.g.
    MC14052 m_Mux1;
    MC14052 m_Mux2;
    
### Write the InitialiseDevices function
This should initialise each of the member devices, and then call AddDevice.

### RunServer
This is declared in server.h and needs to be implemented in your server class.
e.g.

extern "C" void RunServer()
{
    AnalogAudioServer svr;
    svr.Run();
}

### GetBoardID

this should just return SWITCH_ID
