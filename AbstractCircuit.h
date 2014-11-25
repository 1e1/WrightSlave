#ifndef CoreSlave_AbstractCircuit_h
#define CoreSlave_AbstractCircuit_h



#include <Arduino.h>


class AbstractCircuit {

  public:
  virtual void work()  {};
  virtual void idle()  {};
  virtual void check() {};
  virtual void clean() {};

};



#endif
