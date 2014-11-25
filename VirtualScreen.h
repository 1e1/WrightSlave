#ifndef CoreSlave_VirtualScreen_h
#define CoreSlave_VirtualScreen_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "macros.h"
#include "Core.h"


#define SMASK_TAB(bookmark)   bookmark >> 6
#define SMASK_PAGE(bookmark)  bookmark & B111111
#define SMASK_TAB_INC         B1000000
#define SMASK_PAGE_INC        B1


class VirtualScreen : public AbstractCircuit, public Observable, public Observer {

  public:
  typedef enum { DOWN, IDLE, UP } State;

  VirtualScreen() : Observable();

  void update(Observable* observable);

  void resetTab();
  void previousTab();
  void nextTab();

  void resetPage();
  void previousPage();
  void nextPage();

  const prog_char* getTitle();
  Pinout* getPinout();

  __attribute__((always_inline)) inline const uint8_t getTabNumber();
  __attribute__((always_inline)) inline const uint8_t getLastTabNumber();

  const uint8_t getPageNumber();
  const uint8_t getLastPageNumber();

  protected:
  uint8_t _tab;
  uint8_t _page;
  struct {
    byte state : 2;
    byte timer : 6;
  } _state;
  Warehouse<Pinout*>::iterator _itr;

};




/***********************************************************
 *                         INLINE                          *
 **********************************************************/


__attribute__((always_inline)) inline
const uint8_t VirtualScreen::getTabNumber()
{
  return 1 + this->_tab;
}


__attribute__((always_inline)) inline
const uint8_t VirtualScreen::getLastTabNumber()
{
  return Core::length();
}



#endif
