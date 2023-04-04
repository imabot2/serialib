/**
 * @file /example1/main.cpp
 * @author Philippe Lucidarme
 * @date December 2019
 * @brief File containing example of serial port communication
 *
 * This example send an array of bytees through the serial device
 * The program expect receiving a the same number of bytes from the device
 *
 * The following code has been tested with this Arduino Uno sketch
 * that return the uppercase character received on the serial device
 *
 * // Setup, initialize
 * void setup() {
 *    Serial.begin(115200);
 * }
 *
 * // Loop forever
 * void loop() {
 *    // If serial data is pending, read, write twice the received byte
 *    if (Serial.available())
 *        Serial.write( 2*Serial.read() );
 * }
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
    #define SERIAL_PORT "/dev/ttyACM0"
#endif

/*!
 * \brief main  Example of read and write string
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


    // Create an array of bytes
    unsigned char prime[8] = { 1, 3, 5, 7, 11, 13, 17, 19 };

    // Write the array on the serial device
    serial.writeBytes(prime, 8);
    printf ("Data sent\n");

    // Read the bytes
    unsigned char received[8];
    serial.readBytes(received, 8, 2000, 1000);

    // Display each byte received
    for (int i=0; i<8 ; i++)
        printf("Receive [%d] = %d\n", i, received[i]);

    // Close the serial device
    serial.closeDevice();

    return 0 ;
}
