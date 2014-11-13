#ifndef CoreSlave_Circuit_h
#define CoreSlave_Circuit_h



#include <Arduino.h>


class Circuit {

  public:
  Circuit();
  virtual ~Circuit();

  virtual void cycleAction() {};
  virtual void sectionAction() {};

  virtual void check() {};
  virtual void clean() {};

};



#endif
