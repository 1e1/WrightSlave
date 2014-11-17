#include "VirtualScreen.h"




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




VirtualScreen::VirtualScreen()
{
  this->_tab  = 0;
  this->_itr = NULL;
}


void VirtualScreen::previousTab()
{
  if (VirtualScreen::_tab == 0) {
    VirtualScreen::_tab = Core::length();
  }
  VirtualScreen::_tab--;
}


void VirtualScreen::nextTab()
{
  VirtualScreen::_tab++;
  if (VirtualScreen::_tab == Core::length()) {
    VirtualScreen::_tab = 0;
  }
}


void VirtualScreen::previousPage()
{
  Warehouse<Pinout*>::iterator __itr = Core::groups[this->_tab].pinouts->begin();
  while (NULL != __itr) {
    if (this->_itr == __itr->next) {
      this->_itr == __itr;
      return;
    }
    __itr = __itr->next;
  }
}


void VirtualScreen::nextPage()
{
  if (NULL == this->_itr->next) {
    this->_itr = Core::groups[this->_tab].pinouts->begin();
  } else {
    this->_itr = this->_itr->next;
  }
}



const prog_char* VirtualScreen::getTitle()
{
  switch(Core::groups[this->_tab].type) {
    
  }
  return PSTR("TODO");
}

Pinout* VirtualScreen::getPinout()
{
  return this->_itr->item;
}

const uint8_t VirtualScreen::getPageNumber()
{
  return 0;
}


const uint8_t VirtualScreen::getLastPageNumber()
{
  return Core::groups[this->_tab].pinouts->size();
}
