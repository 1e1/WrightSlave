#ifndef CoreSlave_Circuit_h
#define CoreSlave_Circuit_h



#include <Arduino.h>


class Circuit {

  public:
  virtual void work()  {};
  virtual void idle()  {};
  virtual void check() {};
  virtual void clean() {};

};



#endif
