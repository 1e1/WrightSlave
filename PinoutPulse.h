#ifndef CoreSlave_PinoutPulse_h
#define CoreSlave_PinoutPulse_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Pinout.h"


class PinoutPulse : public Pinout {

  public:
  PinoutPulse(const byte pin, const prog_char* label);
  PinoutPulse(const byte pin, const prog_char* label, uint8_t value);
  ~PinoutPulse();

  void addValue(const uint8_t d);

  __attribute__((always_inline)) inline const uint8_t getValue() { return this->_value; };
  __attribute__((always_inline)) inline void setValue(const uint8_t v) { this->_value = v; analogWrite(this->getPin(), this->getValue()); };

  protected:
  uint8_t _value;

};



#endif CoreSlave_PinoutPulse_h
