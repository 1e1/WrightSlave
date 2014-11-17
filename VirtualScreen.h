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


class VirtualScreen {

  public:
  VirtualScreen();

  void previousTab();
  void nextTab();
  void previousPage();
  void nextPage();

  const prog_char* getTitle();
  Pinout* getPinout() {};

  __attribute__((always_inline)) inline const uint8_t getTabNumber();
  __attribute__((always_inline)) inline const uint8_t getLastTabNumber();

  const uint8_t getPageNumber();
  const uint8_t getLastPageNumber();

  protected:
  uint8_t _bookmark;
  uint8_t _tab;
  Warehouse<T*>::iterator _itr;

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
  return ARRAYLEN(Core::groups);
}



#endif
