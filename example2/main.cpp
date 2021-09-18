/**
 * @file /example1/main.cpp
 * @author Philippe Lucidarme
 * @date December 2019
 * @brief File containing example of serial port communication
 *
 * This example send the ASCII table through the serial device
 *
 * @see https://lucidar.me
 */


// Serial library
#include "../lib/serialib.h"
#include <unistd.h>
#include <stdio.h>


#if defined (_WIN32) || defined(_WIN64)
    //for serial ports above "COM9", we must use this extended syntax of "\\.\COMx".
    //also works for COM0 to COM9.
    //https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea?redirectedfrom=MSDN#communications-resources
    #define SERIAL_PORT "\\\\.\\COM1"
#endif
#if defined (__linux__) || defined(__APPLE__)
    #define SERIAL_PORT "/dev/ttyS0"
#endif

/*!
 * \brief main  Example of read and write serial port IO pins
 * \return      never !
 */
int main( /*int argc, char *argv[]*/)
{
    // Serial object
    serialib serial;

    // Connection to serial port
    char errorOpening = serial.openDevice(SERIAL_PORT, 115200);


    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening!=1) return errorOpening;
    printf ("Successful connection to %s\n",SERIAL_PORT);

    // Set DTR
    serial.DTR(true);
    // Clear RTS
    serial.RTS(false);

    // Loop forever
    while (1)
    {
        // Read and display the status of each pin
        // DTR should be 1
        // RTS should be 0
        printf ("4-DTR=%d\t", serial.isDTR());
        printf ("7-RTS=%d\t", serial.isRTS());

        printf ("1-DCD=%d\t", serial.isDCD());
        printf ("8-CTS=%d\t", serial.isCTS());
        printf ("6-DSR=%d\t", serial.isDSR());
        printf ("9-RING=%d\n", serial.isRI());
    }

    // Close the serial device
    serial.closeDevice();

    return 0 ;
}
