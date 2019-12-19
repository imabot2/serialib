/*!
\file    serial-O-matic.h
\brief   Header file of the class serialib. This class is used for communication over a serial device.
\author  Philippe Lucidarme (University of Angers)
\version 1.2
\date    28 avril 2011
This Serial library is used to communicate through serial port.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This is a licence-free software, it can be used by anyone who try to build a better world.
*/


#ifndef SERIALIB_H
#define SERIALIB_H



// Used for TimeOut operations
#include <sys/time.h>
// Accessing to the serial port under Windows
#include <windows.h>

// For debugging
#include <stdio.h>

// To avoid unused parameters
#define UNUSED(x) (void)(x)


/*!  \class     serialib
     \brief     This class is used for communication over a serial device.
*/
class serialib
{
public:

    //_____________________________________
    // ::: Constructors and destructors :::



    // Constructor of the class
    serialib    ();

    // Destructor
    ~serialib   ();



    //_________________________________________
    // ::: Configuration and initialization :::


    // Open a device
    char    openDevice (const char *Device,const unsigned int Bauds);

    // Close the current device
    void    closeDevice();




    //___________________________________________
    // ::: Read/Write operation on characters :::


    // Write a char
    char    writeChar   (char);

    // Read a char (with timeout)
    char    readChar    (char *pByte,const unsigned int TimeOut_ms=0);




    //________________________________________
    // ::: Read/Write operation on strings :::


    // Write a string
    char    writeString (const char *String);

    // Read a string (with timeout)
    int     readString  (   char *String,
                            char FinalChar,
                            unsigned int MaxNbBytes,
                            const unsigned int TimeOut_ms=0);



    // _____________________________________
    // ::: Read/Write operation on bytes :::


    // Write an array of bytes
    char    writeBytes  (const void *Buffer, const unsigned int NbBytes);

    // Read an array of byte (with timeout)
    int     readBytes   (void *Buffer,unsigned int MaxNbBytes,const unsigned int TimeOut_ms=0, unsigned int SleepDuration_us=100);




    // _________________________
    // ::: Special operation :::


    // Empty the received buffer
    char    flushReceiver();

    // Return the number of bytes in the received buffer
    int     available();




    // _________________________
    // ::: Access to IO bits :::


    // Set CTR status (Data Terminal Ready, pin 4)
    bool    DTR(bool status);
    bool    setDTR();
    bool    clearDTR();

    // Set RTS status (Request To Send, pin 7)
    bool    RTS(bool status);
    bool    setRTS();
    bool    clearRTS();

    // Get RI status (Ring Indicator, pin 9)
    bool    isRI();

    // Get DCD status (Data Carrier Detect, pin 1)
    bool    isDCD();

    // Get CTS status (Clear To Send, pin 8)
    bool    isCTS();

    // Get DSR status (Data Set Ready, pin 9)
    bool    isDSR();

    //
    bool    isRTS();

    bool    isDTR();


private:
    // Read a string (no timeout)
    int     readStringNoTimeOut  (char *String,char FinalChar,unsigned int MaxNbBytes);




    // Handle on serial device
    HANDLE          hSerial;

    // For setting serial port timeouts
    COMMTIMEOUTS    timeouts;


};



/*!  \class     timeOut
     \brief     This class can manage a timer which is used as a timeout.
   */
// Class timeOut
class timeOut
{
public:

    // Constructor
    timeOut();

    // Init the timer
    void                initTimer();

    // Return the elapsed time since initialization
    unsigned long int   elapsedTime_ms();

private:
    // Used to store the previous time (for computing timeout)
    struct timeval      previousTime;
};

#endif // serialib_H
