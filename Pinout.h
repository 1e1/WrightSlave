#ifndef CoreSlave_Pinout_h
#define CoreSlave_Pinout_h



#include <Arduino.h>
#include <avr/pgmspace.h>


class Pinout {

  public:
  Pinout();
  Pinout(const byte pin, const prog_char* label);
  virtual ~Pinout();

  virtual const uint8_t getValue() =0;
  virtual void setValue(const uint8_t v) =0;

  __attribute__((always_inline)) inline const byte getPin()           { return this->_id.pin; };
  __attribute__((always_inline)) inline const prog_char* getLabel()   { return this->_label;  };

  protected:
  void init(const byte pin, const prog_char* label);

  struct {
    byte pin          : 6;
    boolean isNC      : 1;
    boolean isActive  : 1;
  } _id;
  const prog_char* _label PROGMEM;

};



#endif CoreSlave_Pinout_h
