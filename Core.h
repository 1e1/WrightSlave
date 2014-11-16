#ifndef CoreSlave_Core_h
#define CoreSlave_Core_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "macros.h"
#include "Pinout.h"
#include "PinoutPulse.h"
#include "PinoutDigital.h"
#include "PinoutSchedule.h"
#include "Warehouse.h"


/** ===================== **/
/**      connections      **/
/** ===================== **/

/*
// DRAFT
static const byte temperaturePins[] = { 13 };
const intfMessage STATIC_MESSAGES[] = {
  { obsTemperature, 'x', temperaturePins, "Indoor" }
};
*/


#define PIN_NOT_FOUND uint8_t(-1)
#define TYPE_MESSAGE  'M'
#define TYPE_PULSE    'P'
#define TYPE_DIGITAL  'D'
#define TYPE_SCHEDULE 'S'




class Core {

  public:
  static void processTimer(const boolean fullYear, const uint8_t dayOfWeek, const unsigned int hour);
  static void processLine();
  static void readUntil(char terminator);
  static void stateToBuffer();
  static void copyToBuffer  (uint8_t x);
  static void copyToBuffer  (char c);
  static void copyToBuffer_P(const prog_char* const data);
  static void copyToBuffer  (const char chars[], uint8_t size);
  static void copyToBuffer_P(const prog_uchar data[], size_t size);
  static const uint8_t bufferEqualsLength_P(const prog_char* str);
  static void sendBuffer();
  static void sendBufferLn();

  static void registerPulse   (const byte pin, const prog_char* label);
  static void registerDigital (const byte pin, const prog_char* label, const boolean isNC);
  static void registerSchedule(const byte id, const prog_char* label, const boolean isNC, const unsigned int schedule, const unsigned long digitals_22_49);

  static Warehouse<Pinout*>* getPinoutsOf(const char type);
  static Pinout* getPinoutAtPin(const uint8_t pin, Warehouse<Pinout*>* pinouts);
  static const uint8_t size();
  //static inline void printBuffer() {for(uint8_t i=0; i<_bufferSize; i++) Serial.print(_buffer[i]); };

  // inline
  static void setStream(Stream* inputStream);
  static void unbuffer();
  static const boolean  bufferIsEqualTo_P(const prog_char* str);
  static const uint8_t bufferIsPrefixOf_P(const prog_char* str);

  __attribute__((always_inline)) inline static Warehouse<Pinout*>* currentPinouts() { return _pinouts; };
  __attribute__((always_inline)) inline static Pinout* currentPinout() { return _pinout; };

  protected:
  static void autoSendBuffer();
  static uint8_t readUint8();
  static void statusLineToBuffer(Pinout* pinout);

  struct group {
    const char type;
    Warehouse<Pinout*>* pinouts;
  };

  static Stream* _currentStream;
  static char    _buffer[max(READBUFFERSIZE, WRITEBUFFERSIZE)];
  static uint8_t _bufferSize;

  static const group         _groups[];
  static Warehouse<Pinout*>* _pinouts;
  static Pinout*             _pinout;

};




/***********************************************************
 *                         INLINE                          *
 **********************************************************/


__attribute__((always_inline)) inline
void Core::setStream(Stream* inputStream)
{
  Core::_currentStream = inputStream;
}


__attribute__((always_inline)) inline
void Core::unbuffer()
{
  Core::_bufferSize = 0;
}


__attribute__((always_inline)) inline
const boolean Core::bufferIsEqualTo_P(const prog_char* str)
{
  return Core::_bufferSize == strlen_P(str) && strlen_P(str) == Core::bufferEqualsLength_P(str);
};


__attribute__((always_inline)) inline
const uint8_t Core::bufferIsPrefixOf_P(const prog_char* str)
{
  return Core::bufferEqualsLength_P(str) == strlen_P(str);
};



#endif
