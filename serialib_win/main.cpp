// Author : Philippe Lucidarme
// Date : devember 2019
// Copyright : Philippe Lucidarme
// Can't be used without agreement of Philippe Lucidarme
// Contact : philippe@lucidar.me
// https://lucidar.me


// Serial library
#include "serialib_win.h"
#include <stdio.h>
#include <unistd.h>


#define SERIAL_PORT "COM1"


int main( /*int argc, char *argv[]*/)
{
    // Serial object
    serialib serial;


    // Connection to serial port
    char errorOpening = serial.openDevice(SERIAL_PORT, 115200);


    // If connection fails, return the error code
    // Otherwise, display a success message
    if (errorOpening!=1) return errorOpening;
    printf ("Successful connection to %s\n",SERIAL_PORT);

    serial.RTS(true);
    serial.DTR(false);
    while (1)
    {

        printf ("1->%d \t 6->%d \t 8->%d \t 9->%d\n", serial.isDCD(), serial.isDSR(), serial.isCTS(), serial.isRI());
    }


    // Wait one second before the device is ready
    sleep(1);    

    // Display the number bytes received but not read
    printf ("%d bytes in the buffer\n",serial.available());


    // Flush receiver (delete all pending characters)
    char errorFlushing = serial.flushReceiver();
    if (errorFlushing!=1) return errorFlushing;
    printf ("RX buffer flushed\n");


    // Display the number bytes received but not read
    printf ("%d bytes in the buffer\n",serial.available());


    // Send the string "Hello !" to the device
    char errorWritting = serial.writeString("Hello !\n");
    if (errorWritting!=1) return errorWritting;
    printf ("String successfully sent to the device\n");


    // Read string on the serial device
    char receivedString[100];
    int n = serial.readString(receivedString, '\n', 100, 3000);
    printf ("%d characters read\n",n);
    printf ("String read : %s\n",receivedString);


    // Close properly the serial device
    serial.closeDevice();


    return 0 ;
}
