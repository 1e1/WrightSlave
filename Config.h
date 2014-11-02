#ifndef CoreSlave_Config_h
#define CoreSlave_Config_h


#define SP ' '
#define CR '\r'
#define LF '\n'
#define CRLF "\r\n"

#define MONDAY    byte(0) // MONTAG
#define TUESDAY   byte(1) // DIENSTAG
#define WEDNESDAY byte(2) // MITTWOCH
#define THURSDAY  byte(3) // DONNERSTAG
#define FRIDAY    byte(4) // FREITAG
#define SATURDAY  byte(5) // SAMSTAG
#define SUNDAY    byte(6) // SONNTAG

//#define READBUFFERSIZE          max(byte(16), byte(1+ ARRAYLEN(HTTP_AUTH64)))
#define READBUFFERSIZE          byte(16)
#define WRITEBUFFERSIZE         byte(64)
#define MAXLINESIZE             byte(255)

#define READCHAR_TIMEOUT        byte(4)


#endif
