#ifndef CoreSlave_PinoutDigital_h
#define CoreSlave_PinoutDigital_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Pinout.h"


class PinoutDigital : public Pinout {

  public:
  PinoutDigital();
  PinoutDigital(byte pin, const prog_char* label, const boolean isNC);
  PinoutDigital(byte pin, const prog_char* label, const boolean isNC, const boolean value);
  ~PinoutDigital();

  // inline
  const uint8_t getValue();
  void setValue(const uint8_t v);

  protected:
  // hardwareValue to humanValue OR humanValue to hardwareValue
  __attribute__((always_inline)) inline const boolean convertValue(const boolean v)     { return v ^ this->_id.isNC; };

};




/***********************************************************
 *                         INLINE                          *
 **********************************************************/


__attribute__((always_inline)) inline
const uint8_t PinoutDigital::getValue()
{
  return this->convertValue( this->_id.isActive );
}


__attribute__((always_inline)) inline
void PinoutDigital::setValue(const uint8_t v)
{
  this->_id.isActive = this->convertValue(0!=v);
  digitalWrite(this->getPin(), this->_id.isActive);
}




#endif CoreSlave_PinoutDigital_h
