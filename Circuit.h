#ifndef CoreSlave_Circuit_h
#define CoreSlave_Circuit_h



#include <Arduino.h>


class Circuit {

  public:
  virtual void work()  =0;
  virtual void idle()  =0;
  virtual void check() =0;
  virtual void clean() =0;

};



#endif
