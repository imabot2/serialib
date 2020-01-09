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
    #define SERIAL_PORT "COM1"
#endif
#ifdef __linux__
    #define SERIAL_PORT "/dev/ttyACM0"
#endif


/*!
 * \brief main  Simple example that send ASCII characters to the serial device
 * \return      0 : success
 *              <0 : an error occured
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


    // Display ASCII characters (from 32 to 128)
    for (int c=32;c<128;c++)
    {
        serial.writeChar(c);
        usleep(10000);
    }

    // Close the serial device
    serial.closeDevice();

    return 0 ;
}
