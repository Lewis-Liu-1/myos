#include "x86.h"

enum PS2WRITEREAD
{
    PS2READ,
    PS2WRITE
};
#define PS2DATA 0x60
#define PS2STATUS 0x64
#define PS2COMMAND 0x64

char firstPortDevice[3];
char secondPortDevice[3];

void controllerWait(unsigned char type) // unsigned char
{
    unsigned int _time_out = 100000; // unsigned int
    if (type == 0)
    {
        while (_time_out--) // Data
        {
            if ((inb(PS2STATUS) & 1) == 1)
            {
                return;
            }
        }
        return;
    }
    else
    {
        while (_time_out--) // Signal
        {
            if ((inb(PS2STATUS) & 2) == 0)
            {
                return;
            }
        }
        return;
    }
}

unsigned char getPS2ControllerStatus()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0x20);
    controllerWait(PS2READ);
    return inb(0x60);
}

void setPS2ControllerStatus(unsigned char status)
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0x60);
    outb(PS2DATA, status);
}

void disableSecondPS2Port()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xA7);
}

void enableSecondPS2Port()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xA8);
}

void disableFirstPS2Port()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xAD);
}

void enableFirstPS2Port()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xAE);
}

void commandPS2Controller(unsigned char command)
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, command);
}

void writeToFirstPS2Port(unsigned char byte)
{
    controllerWait(PS2WRITE);
    outb(PS2DATA, byte);
}

void writeToSecondPS2Port(unsigned char byte)
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xD4);
    controllerWait(PS2WRITE);
    outb(PS2DATA, byte);
}

unsigned char readPS2DataPort()
{
    controllerWait(PS2READ);
    return inb(PS2DATA);
}

unsigned char initPS2()
{
    // Start by disabling devices

    disableFirstPS2Port();
    disableSecondPS2Port();

    // Flush the input buffer

    controllerWait(PS2READ);
    inb(PS2DATA);

    // Set the state of the status register.
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0x20);
    controllerWait(PS2READ);
    unsigned char status = inb(PS2DATA);
    unsigned char dual0 = (status & 0b00100000) == 0b00100000; // if dual = 0, there is no second channel
    status |= 0b01000011;
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0x60);
    controllerWait(PS2WRITE);
    outb(PS2DATA, status);

    // Perform self test
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xAA);
    controllerWait(PS2READ);
    unsigned char response = inb(PS2DATA);
    if (response == 0xFC)
        return 0xFC;
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0x60);
    controllerWait(PS2WRITE);
    outb(PS2DATA, status);

    // test dual channel controller
    unsigned char dual = 0xFF;

    if (dual0 == 1)
    {
        controllerWait(PS2WRITE);
        outb(PS2COMMAND, 0xAE);
        controllerWait(PS2WRITE);
        outb(PS2COMMAND, 0x20);
        controllerWait(PS2READ);
        status = inb(PS2DATA);
        unsigned char dual1 = (status & 0b00100000) == 0b00100000;
        // if dual1 == 0, dual channel exists
        controllerWait(PS2WRITE);
        outb(PS2COMMAND, 0xAD);
        dual = !dual1;
    }
    else
    {
        dual = 0;
    }

    // Perform interface tests
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xAB);
    controllerWait(PS2READ);
    unsigned char responseIT1 = inb(PS2DATA);
    unsigned char responseIT2;
    if (dual == 1)
    {
        controllerWait(PS2WRITE);
        outb(PS2COMMAND, 0xA9);
        controllerWait(PS2READ);
        responseIT2 = inb(PS2DATA);
    }

    if (responseIT1 != 0x0)
        return responseIT1;
    if (responseIT2 != 0x0 && dual == 1)
        return responseIT2;

    // Enable devices

    enableFirstPS2Port();
    if (dual == 1)
        enableSecondPS2Port();

    writeToFirstPS2Port(0xFF);
    response = readPS2DataPort();
    if (response == 0xFC)
        return 0xFC;

    writeToSecondPS2Port(0xFF);
    response = readPS2DataPort();
    if (response == 0xFC)
        return 0xFC;

    return 0;
}