/*!
 \file    serial-O-matic.cpp
 \brief   Source file of the class serialib. This class is used for communication over a serial device.
 \author  Philippe Lucidarme (University of Angers)
 \version 1.2
 \date    28 avril 2011

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


This is a licence-free software, it can be used by anyone who try to build a better world.
 */

#include "serialib.h"



//_____________________________________
// ::: Constructors and destructors :::


/*!
    \brief      Constructor of the class serialib.
*/
serialib::serialib()
{
#if defined (_WIN32) || defined( _WIN64)
    // Set default value for RTS and DTR (Windows only)
    currentStateRTS=true;
    currentStateDTR=true;
#endif
}


/*!
    \brief      Destructor of the class serialib. It close the connection
*/
// Class desctructor
serialib::~serialib()
{
    closeDevice();
}



//_________________________________________
// ::: Configuration and initialization :::



/*!
     \brief Open the serial port
     \param Device : Port name (COM1, COM2, ... for Windows ) or (/dev/ttyS0, /dev/ttyACM0, /dev/ttyUSB0 ... for linux)
     \param Bauds : Baud rate of the serial port.

                \n Supported baud rate for Windows :
                        - 110
                        - 300
                        - 600
                        - 1200
                        - 2400
                        - 4800
                        - 9600
                        - 14400
                        - 19200
                        - 38400
                        - 56000
                        - 57600
                        - 115200
                        - 128000
                        - 256000

               \n Supported baud rate for Linux :\n
                        - 110
                        - 300
                        - 600
                        - 1200
                        - 2400
                        - 4800
                        - 9600
                        - 19200
                        - 38400
                        - 57600
                        - 115200

     \return 1 success
     \return -1 device not found
     \return -2 error while opening the device
     \return -3 error while getting port parameters
     \return -4 Speed (Bauds) not recognized
     \return -5 error while writing port parameters
     \return -6 error while writing timeout parameters
  */
char serialib::openDevice(const char *Device,const unsigned int Bauds)
{
#if defined (_WIN32) || defined( _WIN64)
    // Open serial port
    hSerial = CreateFileA(Device,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
    if(hSerial==INVALID_HANDLE_VALUE) {
        if(GetLastError()==ERROR_FILE_NOT_FOUND)
            return -1;                                                  // Device not found
        return -2;                                                      // Error while opening the device
    }

    // Set parameters
    DCB dcbSerialParams;                                                // Structure for the port parameters
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))                       // Get the port parameters
        return -3;                                                      // Error while getting port parameters
    switch (Bauds)                                                      // Set the speed (Bauds)
    {
    case 110  :     dcbSerialParams.BaudRate=CBR_110; break;
    case 300  :     dcbSerialParams.BaudRate=CBR_300; break;
    case 600  :     dcbSerialParams.BaudRate=CBR_600; break;
    case 1200 :     dcbSerialParams.BaudRate=CBR_1200; break;
    case 2400 :     dcbSerialParams.BaudRate=CBR_2400; break;
    case 4800 :     dcbSerialParams.BaudRate=CBR_4800; break;
    case 9600 :     dcbSerialParams.BaudRate=CBR_9600; break;
    case 14400 :    dcbSerialParams.BaudRate=CBR_14400; break;
    case 19200 :    dcbSerialParams.BaudRate=CBR_19200; break;
    case 38400 :    dcbSerialParams.BaudRate=CBR_38400; break;
    case 56000 :    dcbSerialParams.BaudRate=CBR_56000; break;
    case 57600 :    dcbSerialParams.BaudRate=CBR_57600; break;
    case 115200 :   dcbSerialParams.BaudRate=CBR_115200; break;
    case 128000 :   dcbSerialParams.BaudRate=CBR_128000; break;
    case 256000 :   dcbSerialParams.BaudRate=CBR_256000; break;
    default : return -4;
    }
    dcbSerialParams.ByteSize=8;                                         // 8 bit data
    dcbSerialParams.StopBits=ONESTOPBIT;                                // One stop bit
    dcbSerialParams.Parity=NOPARITY;                                    // No parity
    if(!SetCommState(hSerial, &dcbSerialParams))                        // Write the parameters
        return -5;                                                      // Error while writing

    // Set TimeOut
    timeouts.ReadIntervalTimeout=0;                                     // Set the Timeout parameters
    timeouts.ReadTotalTimeoutConstant=MAXDWORD;                         // No TimeOut
    timeouts.ReadTotalTimeoutMultiplier=0;
    timeouts.WriteTotalTimeoutConstant=MAXDWORD;
    timeouts.WriteTotalTimeoutMultiplier=0;
    if(!SetCommTimeouts(hSerial, &timeouts))                            // Write the parameters
        return -6;                                                      // Error while writting the parameters
    return 1;                                                           // Opening successfull
#endif
#ifdef __linux__
    struct termios options;                                             // Structure with the device's options


    // Open device
    fd = open(Device, O_RDWR | O_NOCTTY | O_NDELAY);                    // Open port
    if (fd == -1) return -2;                                            // If the device is not open, return -1
    fcntl(fd, F_SETFL, FNDELAY);                                        // Open the device in nonblocking mode

    // Set parameters
    tcgetattr(fd, &options);                                            // Get the current options of the port
    bzero(&options, sizeof(options));                                   // Clear all the options
    speed_t         Speed;
    switch (Bauds)                                                      // Set the speed (Bauds)
    {
    case 110  :     Speed=B110; break;
    case 300  :     Speed=B300; break;
    case 600  :     Speed=B600; break;
    case 1200 :     Speed=B1200; break;
    case 2400 :     Speed=B2400; break;
    case 4800 :     Speed=B4800; break;
    case 9600 :     Speed=B9600; break;
    case 19200 :    Speed=B19200; break;
    case 38400 :    Speed=B38400; break;
    case 57600 :    Speed=B57600; break;
    case 115200 :   Speed=B115200; break;
    default : return -4;
    }
    cfsetispeed(&options, Speed);                                       // Set the baud rate at 115200 bauds
    cfsetospeed(&options, Speed);
    options.c_cflag |= ( CLOCAL | CREAD |  CS8);                        // Configure the device : 8 bits, no parity, no control
    options.c_iflag |= ( IGNPAR | IGNBRK );
    options.c_cc[VTIME]=0;                                              // Timer unused
    options.c_cc[VMIN]=0;                                               // At least on character before satisfy reading
    tcsetattr(fd, TCSANOW, &options);                                   // Activate the settings
    return (1);                                                         // Success
#endif

}


/*!
     \brief Close the connection with the current device
*/
void serialib::closeDevice()
{
#if defined (_WIN32) || defined( _WIN64)
    CloseHandle(hSerial);
#endif
#ifdef __linux__
    close (fd);
#endif
}




//___________________________________________
// ::: Read/Write operation on characters :::



/*!
     \brief Write a char on the current serial port
     \param Byte : char to send on the port (must be terminated by '\0')
     \return 1 success
     \return -1 error while writting data
  */
char serialib::writeChar(const char Byte)
{
#if defined (_WIN32) || defined( _WIN64)
    // Number of bytes written
    DWORD dwBytesWritten;
    // Write the char to the serial device
    // Return -1 if an error occured
    if(!WriteFile(hSerial,&Byte,1,&dwBytesWritten,NULL)) return -1;                                                      // Error while writing
    // Write operation successfull
    return 1;
#endif
#ifdef __linux__
    if (write(fd,&Byte,1)!=1)                                           // Write the char
        return -1;                                                      // Error while writting
    return 1;                                                           // Write operation successfull
#endif
}



//________________________________________
// ::: Read/Write operation on strings :::


/*!
     \brief     Write a string on the current serial port
     \param     receivedString : string to send on the port (must be terminated by '\0')
     \return     1 success
     \return    -1 error while writting data
  */
char serialib::writeString(const char *receivedString)
{
#if defined (_WIN32) || defined( _WIN64)
    // Number of bytes written
    DWORD dwBytesWritten;
    // Write the string
    if(!WriteFile(hSerial,receivedString,strlen(receivedString),&dwBytesWritten,NULL))
        // Error while writing, return -1
        return -1;
    // Write operation successfull
    return 1;
#endif
#ifdef __linux__
    int Lenght=strlen(String);                                          // Lenght of the string
    if (write(fd,String,Lenght)!=Lenght)                                // Write the string
        return -1;                                                      // error while writing
    return 1;                                                           // Write operation successfull
#endif
}

// _____________________________________
// ::: Read/Write operation on bytes :::



/*!
     \brief Write an array of data on the current serial port
     \param Buffer : array of bytes to send on the port
     \param NbBytes : number of byte to send
     \return 1 success
     \return -1 error while writting data
  */
char serialib::writeBytes(const void *Buffer, const unsigned int NbBytes)
{
#if defined (_WIN32) || defined( _WIN64)
    // Number of bytes written
    DWORD dwBytesWritten;
    // Write data
    if(!WriteFile(hSerial, Buffer, NbBytes, &dwBytesWritten, NULL))
        // Error while writing, return -1
        return -1;
    // Write operation successfull
    return 1;
#endif
#ifdef __linux__
    if (write (fd,Buffer,NbBytes)!=(ssize_t)NbBytes)                              // Write data
        return -1;                                                      // Error while writing
    return 1;                                                           // Write operation successfull
#endif
}



/*!
     \brief Wait for a byte from the serial device and return the data read
     \param pByte : data read on the serial device
     \param TimeOut_ms : delay of timeout before giving up the reading
            If set to zero, timeout is disable (Optional)
     \return 1 success
     \return 0 Timeout reached
     \return -1 error while setting the Timeout
     \return -2 error while reading the byte
  */
char serialib::readChar(char *pByte,unsigned int TimeOut_ms)
{
#if defined (_WIN32) || defined(_WIN64)
    // Number of bytes read
    DWORD dwBytesRead = 0;

    // Set the TimeOut
    timeouts.ReadTotalTimeoutConstant=TimeOut_ms;

    // Write the parameters, return -1 if an error occured
    if(!SetCommTimeouts(hSerial, &timeouts)) return -1;

    // Read the byte, return -2 if an error occured
    if(!ReadFile(hSerial,pByte, 1, &dwBytesRead, NULL)) return -2;

    // Return 0 if the timeout is reached
    if (dwBytesRead==0) return 0;

    // The byte is read
    return 1;
#endif
#ifdef __linux__
    TimeOut         Timer;                                              // Timer used for timeout
    Timer.InitTimer();                                                  // Initialise the timer
    while (Timer.ElapsedTime_ms()<TimeOut_ms || TimeOut_ms==0)          // While Timeout is not reached
    {
        switch (read(fd,pByte,1)) {                                     // Try to read a byte on the device
        case 1  : return 1;                                             // Read successfull
        case -1 : return -2;                                            // Error while reading
        }
    }
    return 0;
#endif
}



/*!
     \brief Read a string from the serial device (without TimeOut)
     \param receivedString : string read on the serial device
     \param FinalChar : final char of the string
     \param MaxNbBytes : maximum allowed number of bytes read
     \return >0 success, return the number of bytes read
     \return -1 error while setting the Timeout
     \return -2 error while reading the byte
     \return -3 MaxNbBytes is reached
  */
int serialib::readStringNoTimeOut(char *receivedString,char FinalChar,unsigned int MaxNbBytes)
{
    // Number of characters read
    unsigned int    NbBytes=0;
    // Returned value from Read
    char            charRead;

    // While the buffer is not full
    while (NbBytes<MaxNbBytes)
    {
        // Read a character with the restant time
        charRead=readChar(&receivedString[NbBytes]);

        // Check a character has been read
        if (charRead==1)
        {
            // Check if this is the final char
            if (receivedString[NbBytes]==FinalChar)
            {
                // This is the final char, add zero (end of string)
                receivedString  [++NbBytes]=0;
                // Return the number of bytes read
                return NbBytes;
            }

            // The character is not the final char, increase the number of bytes read
            NbBytes++;
        }

        // An error occured while reading, return the error number
        if (charRead<0) return charRead;
    }
    // Buffer is full : return -3
    return -3;
}


/*!
     \brief Read a string from the serial device (with timeout)
     \param receivedString : string read on the serial device
     \param FinalChar : final char of the string
     \param MaxNbBytes : maximum allowed number of bytes read
     \param TimeOut_ms : delay of timeout before giving up the reading (optional)
     \return  >0 success, return the number of bytes read
     \return  0 timeout is reached
     \return -1 error while setting the Timeout
     \return -2 error while reading the byte
     \return -3 MaxNbBytes is reached
  */
int serialib::readString(char *receivedString,char finalChar,unsigned int maxNbBytes,unsigned int timeOut_ms)
{
    // Check if timeout is requested
    if (timeOut_ms==0) return readStringNoTimeOut(receivedString,finalChar,maxNbBytes);

    // Number of bytes read
    unsigned int    nbBytes=0;
    // Character read on serial device
    char            charRead;
    // Timer used for timeout
    timeOut         timer;
    long int        timeOutParam;

    // Initialize the timer (for timeout)
    timer.initTimer();

    // While the buffer is not full
    while (nbBytes<maxNbBytes)
    {
        // Compute the TimeOut for the next call of ReadChar
        timeOutParam = timeOut_ms-timer.elapsedTime_ms();

        // If there is time remaining
        if (timeOutParam>0)
        {
            // Wait for a byte on the serial link with the remaining time as timeout
            charRead=readChar(&receivedString[nbBytes],timeOutParam);

            // If a byte has been received
            if (charRead==1)
            {
                // Check if the character received is the final one
                if (receivedString[nbBytes]==finalChar)
                {
                    // Final character: add the end character 0
                    receivedString  [++nbBytes]=0;
                    // Return the number of bytes read
                    return nbBytes;
                }
                // This is not the final character, just increase the number of bytes read
                nbBytes++;
            }
            // Check if an error occured during reading char
            // If an error occurend, return the error number
            if (charRead<0) return charRead;
        }
        // Check if timeout is reached
        if (timer.elapsedTime_ms()>timeOut_ms)
        {
            // Add the end caracter
            receivedString[nbBytes]=0;
            // Return 0 (timeout reached)
            return 0;
        }
    }

    // Buffer is full : return -3
    return -3;
}


/*!
     \brief Read an array of bytes from the serial device (with timeout)
     \param Buffer : array of bytes read from the serial device
     \param MaxNbBytes : maximum allowed number of bytes read
     \param TimeOut_ms : delay of timeout before giving up the reading
     \return >=0 return the number of bytes read before timeout or
                requested data is completed
     \return -1 error while setting the Timeout
     \return -2 error while reading the byte
  */
int serialib::readBytes (void *Buffer,unsigned int MaxNbBytes,unsigned int TimeOut_ms, unsigned int SleepDuration_us)
{
#if defined (_WIN32) || defined(_WIN64)
    // Avoid warning while compiling
    UNUSED(SleepDuration_us);

    // Number of bytes read
    DWORD dwBytesRead = 0;

    // Set the TimeOut
    timeouts.ReadTotalTimeoutConstant=(DWORD)TimeOut_ms;

    // Write the parameters and return -1 if an error occrured
    if(!SetCommTimeouts(hSerial, &timeouts)) return -1;


    // Read the bytes from the serial device, return -2 if an error occured
    if(!ReadFile(hSerial,Buffer,(DWORD)MaxNbBytes,&dwBytesRead, NULL))  return -2;

    // Return the byte read
    return dwBytesRead;
#endif
#ifdef __linux__
    TimeOut          Timer;                                             // Timer used for timeout
    Timer.InitTimer();                                                  // Initialise the timer
    unsigned int     NbByteRead=0;
    while (Timer.ElapsedTime_ms()<TimeOut_ms || TimeOut_ms==0)          // While Timeout is not reached
    {
        unsigned char* Ptr=(unsigned char*)Buffer+NbByteRead;           // Compute the position of the current byte
        int Ret=read(fd,(void*)Ptr,MaxNbBytes-NbByteRead);              // Try to read a byte on the device
        if (Ret==-1) return -2;                                         // Error while reading

        // One or several byte(s) has been read on the device
        if (Ret>0)
        {
            NbByteRead+=Ret;                                            // Increase the number of read bytes
            if (NbByteRead>=MaxNbBytes)                                 // Success : bytes has been read
                return NbByteRead;
        }
        // Suspend the loop to avoid charging the CPU
        usleep (SleepDuration_us);
    }
    return NbByteRead;                                                  // Timeout reached, return the number of bytes read
#endif
}




// _________________________
// ::: Special operation :::



/*!
    \brief Empty receiver buffer
    \return If the function succeeds, the return value is nonzero.
            If the function fails, the return value is zero.
*/
char serialib::flushReceiver()
{
#if defined (_WIN32) || defined(_WIN64)
    // Purge receiver
    return PurgeComm (hSerial, PURGE_RXCLEAR);
#endif
#ifdef __linux__
    tcflush(fd,TCIFLUSH);
#endif
}



/*!
    \brief  Return the number of bytes in the received buffer (UNIX only)
    \return The number of bytes received by the serial provider but not yet read.
*/
int serialib::available()
{    
#if defined (_WIN32) || defined(_WIN64)
    // Device errors
    DWORD commErrors;
    // Device status
    COMSTAT commStatus;
    // Read status
    ClearCommError(hSerial, &commErrors, &commStatus);
    // Return the number of pending bytes
    return commStatus.cbInQue;
#endif
#ifdef __linux__
    int Nbytes=0;
    ioctl(fd, FIONREAD, &Nbytes);
#endif
}



// __________________
// ::: I/O Access :::

/*!
    \brief      Set or unset the bit DTR
                Convenience method :This method calls setDTR and clearDTR
    \param      Status=true  set DTR
                Status=false unset DTR
    \return     If the function fails, the return value is false
                If the function succeeds, the return value is true.
*/
bool serialib::DTR(bool status)
{
    if (status)
        // Set DTR
        return this->setDTR();
    else
        // Unset DTR
        return this->clearDTR();
}


/*!
    \brief      Set the bit DTR
    \return     If the function fails, the return value is false
                If the function succeeds, the return value is true.
*/
bool serialib::setDTR()
{
#if defined (_WIN32) || defined(_WIN64)
    // Set DTR
    currentStateDTR=true;
    return EscapeCommFunction(hSerial,SETDTR);
#endif
#ifdef __linux__
    // Set DTR
    int status_DTR=0;
    ioctl(fd, TIOCMGET, &status_DTR);
    status_DTR |= TIOCM_DTR;
    ioctl(fd, TIOCMSET, &status_DTR);
#endif
}

/*!
    \brief      Clear the bit DTR
    \return     If the function fails, the return value is false
                If the function succeeds, the return value is true.
*/
bool serialib::clearDTR()
{
#if defined (_WIN32) || defined(_WIN64)
    // Clear DTR
    currentStateDTR=true;
    return EscapeCommFunction(hSerial,CLRDTR);
#endif
#ifdef __linux__
    // Clear DTR
    int status_DTR=0;
    ioctl(fd, TIOCMGET, &status_DTR);
    status_DTR &= ~TIOCM_DTR;
    ioctl(fd, TIOCMSET, &status_DTR);
#endif
}


/*!
    \brief      Set or unset the bit DTR
                Convenience method :This method calls setDTR and clearDTR
    \param      Status=true  set DTR
                Status=false unset DTR
    \return     If the function fails, the return value is false
                If the function succeeds, the return value is true.
*/
bool serialib::RTS(bool status)
{
    if (status)
        // Set RTS
        return this->setRTS();
    else
        // Unset RTS
        return this->clearRTS();
}


/*!
    \brief      Set the bit RTS
    \return     If the function fails, the return value is false
                If the function succeeds, the return value is true.
*/
bool serialib::setRTS()
{
#if defined (_WIN32) || defined(_WIN64)
    // Clear DTR
    currentStateRTS=false;
    return EscapeCommFunction(hSerial,SETRTS);
#endif
#ifdef __linux__
    // Clear DTR
    int status_RTS=0;
    ioctl(fd, TIOCMGET, &status_RTS);
    status_RTS |= TIOCM_RTS;
    ioctl(fd, TIOCMSET, &status_RTS);
#endif
}



/*!
    \brief      Clear the bit RTS
    \return     If the function fails, the return value is false
                If the function succeeds, the return value is true.
*/
bool serialib::clearRTS()
{
#if defined (_WIN32) || defined(_WIN64)
    // Clear DTR
    currentStateRTS=false;
    return EscapeCommFunction(hSerial,CLRRTS);
#endif
#ifdef __linux__
    // Clear DTR
    int status_RTS=0;
    ioctl(fd, TIOCMGET, &status_RTS);
    status_RTS &= ~TIOCM_RTS;
    ioctl(fd, TIOCMSET, &status_RTS);
#endif

}




/*!
    \brief      Get the CTS's status
    \return     Return true if CTS is set otherwise false
  */
bool serialib::isCTS()
{
#if defined (_WIN32) || defined(_WIN64)
    DWORD modemStat;
    GetCommModemStatus(hSerial, &modemStat);
    return modemStat & MS_CTS_ON;
#endif
#ifdef __linux__
    int status=0;
    //Get the current status of the CTS bit
    ioctl(fd, TIOCMGET, &status);
    return status & TIOCM_CTS;
#endif
}



/*!
    \brief      Get the DTR's status
    \return     Return true if DTR is set otherwise false
  */
bool serialib::isDSR()
{
    DWORD modemStat;
    GetCommModemStatus(hSerial, &modemStat);
    return modemStat & MS_DSR_ON;
}






/*!
    \brief      Get the DCD's status (pin1)
                CDC stands for Data Carrier Detect
    \return     Return true if DCD is set otherwise false
  */
bool serialib::isDCD()
{
    DWORD modemStat;
    GetCommModemStatus(hSerial, &modemStat);
    return modemStat & MS_RLSD_ON;
}


bool serialib::isRI()
{
#if defined (_WIN32) || defined(_WIN64)
    DWORD modemStat;
    GetCommModemStatus(hSerial, &modemStat);
    return modemStat & MS_RING_ON;
#endif
#ifdef __linux__

#endif
}


/*!
    \brief      Get the CTS's status
    \return     Return true if CTS is set otherwise false
  */
bool serialib::isDTR()
{
#if defined (_WIN32) || defined( _WIN64)
    return currentStateDTR;
#endif
#ifdef __linux__
    int status=0;
    //Get the current status of the CTS bit
    ioctl(fd, TIOCMGET, &status);
    return status & TIOCM_DTR  ;
#endif
}



/*!
    \brief      Get the RTS's status
    \return     Return true if RTS is set otherwise false
  */
bool serialib::isRTS()
{
#if defined (_WIN32) || defined(_WIN64)
    return currentStateRTS;
#endif
#ifdef __linux__
    int status=0;
    //Get the current status of the CTS bit
    ioctl(fd, TIOCMGET, &status);
    return status & TIOCM_RTS;
#endif
}






// ******************************************
//  Class timeOut
// ******************************************


/*!
    \brief      Constructor of the class timeOut.
*/
// Constructor
timeOut::timeOut()
{}


/*!
    \brief      Initialise the timer. It writes the current time of the day in the structure PreviousTime.
*/
//Initialize the timer
void timeOut::initTimer()
{
    gettimeofday(&previousTime, NULL);
}

/*!
    \brief      Returns the time elapsed since initialization.  It write the current time of the day in the structure CurrentTime.
                Then it returns the difference between CurrentTime and PreviousTime.
    \return     The number of microseconds elapsed since the functions InitTimer was called.
  */
//Return the elapsed time since initialization
unsigned long int timeOut::elapsedTime_ms()
{
    // Current time
    struct timeval CurrentTime;
    // Number of seconds and microseconds since last call
    int sec,usec;

    // Get current time
    gettimeofday(&CurrentTime, NULL);

    // Compute the number of seconds and microseconds elapsed since last call
    sec=CurrentTime.tv_sec-previousTime.tv_sec;
    usec=CurrentTime.tv_usec-previousTime.tv_usec;

    // If the previous usec is higher than the current one
    if (usec<0) {
        // Recompute the microseonds and substract one second
        usec=1000000-previousTime.tv_usec+CurrentTime.tv_usec;
        sec--;
    }

    // Return the elapsed time in milliseconds
    return sec*1000+usec/1000;
}




