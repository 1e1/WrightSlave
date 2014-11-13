#ifndef CoreSlave_Circuit_h
#define CoreSlave_Circuit_h



#include <Arduino.h>


class Circuit {

  public:
  virtual void cycleAction() {};
  virtual void sectionAction() {};

  virtual void begin() {};
  virtual void check() {};
  virtual void clean() {};

};



#endif
