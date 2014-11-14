#include "Core.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/



/*
PinoutDigital Core::digitals[] =
{ NEWDIGITAL_NC(22, relay11)
, NEWDIGITAL_NO(24, relay12)
, NEWDIGITAL_NO(26, relay13)
, NEWDIGITAL_NO(28, relay14)
, NEWDIGITAL_NC(30, relay15)
, NEWDIGITAL_NO(32, relay16)
, NEWDIGITAL_NO(34, relay17)
, NEWDIGITAL_NO(36, relay18)

//, NEWDIGITAL_NC(63, sendmail)
//, NEWDIGITAL_NO(62, automatic)
};


PinoutPulse Core::pulses[] =
{ //NEWPULSE(13, led)
};


Schedule Core::schedules[] =
{ NEWSCHEDULE_NC
  ( 10
  , heating1
  , SMASK_FULLYEAR(false)
  | SMASK_DAY(WEDNESDAY) | SMASK_WEEKEND
  | SMASK_HOUR(18) | SMASK_HOUR(19) | SMASK_HOUR(7)
  , SMASK_PIN(22)  | SMASK_PIN(23)  | SMASK_PIN(30)  | SMASK_PIN(31)
  )
, NEWSCHEDULE_NC
  ( 11
  , heating2
  , SMASK_FULLYEAR(false)
  | SMASK_EVERYDAY
  | SMASK_HOUR(20) | SMASK_HOUR(22) | SMASK_HOUR(24) | SMASK_HOUR(3) | SMASK_HOUR(5) | SMASK_HOUR(6)
  , SMASK_PIN(24)  | SMASK_PIN(25)  | SMASK_PIN(32)  | SMASK_PIN(33) | SMASK_PIN(34)
  )
, NEWSCHEDULE_NC
  ( 20
  , bathroom1
  , SMASK_FULLYEAR(false)
  | SMASK_EVERYDAY
  | SMASK_HOUR(17) | SMASK_HOUR(18) | SMASK_HOUR(20) | SMASK_HOUR(7) | SMASK_HOUR(8)
  , SMASK_PIN(26)  | SMASK_PIN(27)
  )
, NEWSCHEDULE_NC
  ( 21
  , bathroom2
  , SMASK_FULLYEAR(true)
  | SMASK_EVERYDAY
  | SMASK_HOUR(17) | SMASK_HOUR(18) | SMASK_HOUR(6) | SMASK_HOUR(7)
  , SMASK_PIN(28)  | SMASK_PIN(29)
  )
};
*/

Stream* Core::_currentStream;
char    Core::_buffer[max(READBUFFERSIZE, WRITEBUFFERSIZE)];
uint8_t Core::_bufferSize;


Warehouse<PinoutPulse*>*    Core::pulses    = new Warehouse<PinoutPulse*>();
Warehouse<PinoutDigital*>*  Core::digitals  = new Warehouse<PinoutDigital*>();
Warehouse<PinoutSchedule*>* Core::schedules = new Warehouse<PinoutSchedule*>();





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




void Core::processTimer(const boolean fullYear, const uint8_t dayOfWeek, const unsigned int hour)
{
  boolean state;
  Pinout* pinout;
  Warehouse_FOREACHPP(PinoutSchedule, Core::schedules, current)

    LOG("schedule#"); LOGLN((uint8_t)current->getPin();)
    LOG("  |  value: "); LOGLN((uint8_t)current->getValue());

    state = current->is(fullYear, dayOfWeek, hour);
    if (state ^ current->isActive()) {

      LOG("  |  state: "); LOGLN((boolean)state);
      LOG("  |  pins:");

      for (uint8_t iPin=SMASK_PIN_MIN; iPin<=SMASK_PIN_MAX; iPin++) {
        if (current->hasPin(iPin)) {

          LOG(" "); LOG((uint8_t)iPin);

          if (NULL != (pinout=Core::getPinoutDigitalAtPin(iPin))) {
            pinout->setValue(state);
          } else {
            digitalWrite(iPin, state);
          }
        }
      }
      LOGLN();
    }
    LOG("  |--"); LOGLN();
  Warehouse_ENDFOREACHPP
}


void Core::processLine()
{
  char type;
  Pinout* pinout;
  uint8_t pin, value, watchdog = Core::digitals->size() + Core::pulses->size() + Core::schedules->size();
  // [0-9]+ OTHER [0-9]+ (OTHER [0-9]+ OTHER [0-9]+)
  while (Core::_currentStream->available() && watchdog--) {
    type  = Core::_currentStream->read();
    pin   = Core::readUint8();
    value = Core::readUint8();
    LOG("SET #"); LOG(type); LOG(pin); LOG(" <- "); LOGLN(value);
    switch (type) {

      case 'S':
      pinout = Core::getPinoutScheduleAtPin(pin);
      break;

      case 'D':
      // first, check if this digital is not controlled by a timer
      pinout = Core::getPinoutDigitalAtPin(pin);
      break;

      case 'P':
      pinout = Core::getPinoutPulseAtPin(pin);
      break;

    }

    if (NULL != pinout) {
      pinout->setValue(value);
    }
  }
}


void Core::readUntil(char terminator)
{
  char c;
  Core::unbuffer();
  while (Core::_bufferSize < READBUFFERSIZE && (c=Core::_currentStream->read())!=terminator && c!=-1) {
    Core::_buffer[Core::_bufferSize++] = c;
  }
}


void Core::stateToBuffer()
{
  // schedules
  Core::copyToBuffer_P(PSTR("\n=== SCHEDULES ===\n"));
  // add IP info
  Warehouse_FOREACHPP(PinoutSchedule, Core::schedules, element)
    Core::statusLineToBuffer(element);
  Warehouse_ENDFOREACHPP
  // pulses
  Core::copyToBuffer_P(PSTR("\n=== PULSES ===\n"));
  Warehouse_FOREACHPP(PinoutPulse, Core::pulses, element)
    Core::statusLineToBuffer(element);
  Warehouse_ENDFOREACHPP
  // digitals
  Core::copyToBuffer_P(PSTR("\n=== DIGITALS ===\n"));
  Warehouse_FOREACHPP(PinoutDigital, Core::digitals, element)
    Core::statusLineToBuffer(element);
  Warehouse_ENDFOREACHPP
  // messages
  /*
  Core::copyToBuffer_P(PSTR("\n=== MESSAGES ===\n"));
  // add IP info
  for (uint8_t i=0; i < Core::messages_len; i++) {
    USlave::statusLine(Core::messages[i], Core::messages[i].getValue());
  }
  */
  Core::copyToBuffer('\n');
}


/**
  * value from 0 to 999
  */
void Core::copyToBuffer(uint8_t x)
{
  if (x>=10) {
    if (x>=100) {
      Core::copyToBuffer(char('0'+(x/100)));
      x%=100;
    }
    Core::copyToBuffer(char('0'+(x/10)));
    x%=10;
  }
  Core::copyToBuffer(char('0'+x));
}


void Core::copyToBuffer(const char c)
{
  Core::_buffer[Core::_bufferSize++] = c;
  Core::autoSendBuffer();
}


void Core::copyToBuffer_P(const prog_char* const data)
{
  uint8_t i = 0;
  while (pgm_read_byte_near(&data[i]) && i<MAXLINESIZE) {
    Core::_buffer[Core::_bufferSize++] = pgm_read_byte_near(&data[i]);
    Core::autoSendBuffer();
    i++;
  }
}


void Core::copyToBuffer(const char chars[], uint8_t size)
{
  do {
    Core::_buffer[Core::_bufferSize++] = *chars++;
    Core::autoSendBuffer();
  } while(--size);
}


void Core::copyToBuffer_P(const prog_uchar data[], size_t size)
{
  for (size_t i=0; i<size; i++) {
    Core::_buffer[Core::_bufferSize++] = pgm_read_byte_near(&data[i]);
    Core::autoSendBuffer();
  }
}


const uint8_t Core::bufferEqualsLength_P(const prog_char* str)
{
  uint8_t i = 0;
  while (i<Core::_bufferSize && ((char)pgm_read_byte_near(&str[i]))==Core::_buffer[i++]);
  return i;
}


void Core::sendBuffer()
{
  if (Core::_bufferSize) {
    Core::_currentStream->write((uint8_t*) Core::_buffer, Core::_bufferSize);
  }
  Core::unbuffer();
}


void Core::sendBufferLn()
{
  Core::copyToBuffer_P( PSTR(CRLF) );
  Core::sendBuffer();
}


void Core::registerPulse(const byte pin, const prog_char* label)
{
  PinoutPulse* p = new PinoutPulse(pin, label);
  Core::pulses->push(p);
}


void Core::registerDigital(const byte pin, const prog_char* label, const boolean isNC)
{
  PinoutDigital* p = new PinoutDigital(pin, label, isNC);
  Core::digitals->push(p);
}


void Core::registerSchedule(const byte id, const prog_char* label, const boolean isNC, const unsigned int schedule, const unsigned long digitals_22_49)
{
  PinoutSchedule* p = new PinoutSchedule(id, label, isNC, schedule, digitals_22_49);
  Core::schedules->push(p);
}




/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void Core::autoSendBuffer()
{
  if (Core::_bufferSize == WRITEBUFFERSIZE) {
    Core::sendBuffer();
  }
}


uint8_t Core::readUint8()
{
  char c;
  uint8_t watchdog = MAXLINESIZE, out = 0;
  _read:
  if (Core::_currentStream->available()) {
    while ((c=Core::_currentStream->read()) && '0'<=c && c<='9' && --watchdog) {
      out = (out *10) + byte(c -'0');
    }
    if (c==-1) {
      delay(READCHAR_TIMEOUT);
      goto _read;
    }
  }
  return out;
  // return (uint8_t) _currentStream->parseInt();
}


void Core::statusLineToBuffer(Pinout* pinout)
{
  Core::copyToBuffer(pinout->getPin());
  Core::copyToBuffer('\t');
  Core::copyToBuffer(pinout->getValue());
  Core::copyToBuffer('\t');
  Core::copyToBuffer_P(pinout->getLabel());
  Core::copyToBuffer('\n');
}


Pinout* Core::getPinoutAtPin(uint8_t pin, Warehouse<Pinout*>* pinouts)
{
  Warehouse_FOREACHPP(Pinout, pinouts, element)
    if (pin == element->getPin()) {
      return element;
    }
  Warehouse_ENDFOREACHPP
  return NULL;
}
