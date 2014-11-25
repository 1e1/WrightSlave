#include "VirtualScreen.h"




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




VirtualScreen::VirtualScreen()
{
  this->_state(State.);
  this->resetTab();
}


void VirtualScreen::resetTab()
{
  this->_tab = 0;
  this->resetPage();
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


void VirtualScreen::resetPage()
{
  this->_page = 0;
  this->_itr = Core::groups[this->_tab].pinouts->begin();
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
  if (NULL == this->_itr || NULL == this->_itr->next) {
    this->resetPage();
  } else {
    this->_itr = this->_itr->next;
  }
}


const prog_char* VirtualScreen::getTitle()
{
  return Core::groups[this->_tab].title;
}


Pinout* VirtualScreen::getPinout()
{
  return this->_itr->item;
}


const uint8_t VirtualScreen::getPageNumber()
{
  return this->_page;
}


const uint8_t VirtualScreen::getLastPageNumber()
{
  return Core::groups[this->_tab].pinouts->size();
}
