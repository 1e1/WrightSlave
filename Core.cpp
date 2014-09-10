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

//const uint8_t Core::digitals_len   = ARRAYLEN(digitals);
//const uint8_t Core::pulses_len     = ARRAYLEN(pulses);
//const uint8_t Core::messages_len   = 0;//ARRAYLEN(messages);
//const uint8_t Core::schedules_len  = ARRAYLEN(schedules);
//const uint8_t Core::total_len      = ARRAYLEN(digitals) + ARRAYLEN(pulses)/* + ARRAYLEN(messages) */+ ARRAYLEN(schedules);

Stream* Core::_currentStream;
char    Core::_buffer[max(READBUFFERSIZE, WRITEBUFFERSIZE)];
uint8_t Core::_bufferSize;


Warehouse<PinoutDigital>* Core::digitals;
Warehouse<PinoutPulse>* Core::pulses;
Warehouse<PinoutSchedule>* Core::schedules;





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




void Core::processTimer(const boolean fullYear, const uint8_t dayOfWeek, const unsigned int hour)
{
  boolean state;
  Pinout* pinout;
  Warehouse_FOREACH(PinoutSchedule, Core::schedules, current)

    LOG("schedule#"); LOGLN((uint8_t)current->getPin();)
    LOG("  |  value: "); LOGLN((uint8_t)current->getValue());

    if (current->isActive()) {
      state = current->is(fullYear, dayOfWeek, hour);

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
  Warehouse_ENDFOREACH
}


void Core::processLine()
{
  char type;
  Pinout* pinout;
  uint8_t pin, value, watchdog = Core::digitals->size() + Core::pulses->size() + Core::schedules->size();
  // [0-9]+ OTHER [0-9]+ (OTHER [0-9]+ OTHER [0-9]+)
  while (Core::_currentStream->available() && --watchdog) {
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

/*
uint8_t Core::getPinoutIndexOfPin(uint8_t pin, Pinout pinouts[], const uint8_t size)
{
  for (uint8_t index=0; index<size; index++) {
    if (pinouts[index].getPin() == pin) {
      return index;
    }
  }
  return PIN_NOT_FOUND;
}


uint8_t Core::getPulsePinoutIndexOfPin(uint8_t pin)
{
  for (uint8_t index=0; index<Core::pulses_len; index++) {
    if (Core::pulses[index].getPin() == pin) {
      return index;
    }
  }
  return PIN_NOT_FOUND;
}


uint8_t Core::getDigitalPinoutIndexOfPin(uint8_t pin)
{
  for (uint8_t index=0; index<Core::digitals_len; index++) {
    if (Core::digitals[index].getPin() == pin) {
      return index;
    }
  }
  return PIN_NOT_FOUND;
}


uint8_t Core::getSchedulePinoutIndexOfPin(uint8_t pin)
{
  for (uint8_t index=0; index<Core::schedules_len; index++) {
    if (Core::schedules[index].getPin() == pin) {
      return index;
    }
  }
  return PIN_NOT_FOUND;
}
*/

Pinout* Core::getPinoutAtPin(uint8_t pin, Warehouse<Pinout>* pinouts)
{
//  Warehouse_FOREACH(Pinout, pinouts, element)
//  /* * /
//  {
//    Warehouse<Pinout>::iterator __itr = pinouts->begin();
//    Pinout* element;
//    while (NULL != __itr) {
//      element = &((Pinout) (__itr->item));
//  /* */
//    if (pin == element->getPin()) {
//      return element;
//    }
//  Warehouse_ENDFOREACH
//  /* * /
//      __itr = __itr->next;
//    }
//  }
//  /* */
  return NULL;
}


PinoutPulse* Core::getPinoutPulseAtPin(uint8_t pin)
{
  //return (PinoutPulse*) (Core::getPinoutAtPin(pin, (Warehouse<Pinout>*) (Core::pulses)));
  Warehouse_FOREACH(PinoutPulse, Core::pulses, element)
    if (pin == element->getPin()) {
      return element;
    }
  Warehouse_ENDFOREACH
  return NULL;
}


PinoutDigital* Core::getPinoutDigitalAtPin(uint8_t pin)
{
  //return (PinoutDigital*) (Core::getPinoutAtPin(pin, (Warehouse<Pinout>*) (Core::digitals)));
  Warehouse_FOREACH(PinoutDigital, Core::digitals, element)
    if (pin == element->getPin()) {
      return element;
    }
  Warehouse_ENDFOREACH
  return NULL;
}


PinoutSchedule* Core::getPinoutScheduleAtPin(uint8_t pin)
{
  //return (PinoutSchedule*) (Core::getPinoutAtPin(pin, (Warehouse<Pinout>*) (Core::schedules)));
  Warehouse_FOREACH(PinoutSchedule, Core::schedules, element)
    if (pin == element->getPin()) {
      return element;
    }
  Warehouse_ENDFOREACH
  return NULL;
}