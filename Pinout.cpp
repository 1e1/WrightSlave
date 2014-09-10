#include "Pinout.h"




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




Pinout::Pinout()
{
}


Pinout::Pinout(const byte pin, const prog_char* label)
{
  this->init(pin, label);
}


Pinout::~Pinout()
{
}


void Pinout::init(const byte pin, const prog_char* label)
{
  this->_id.pin = pin;
  this->_label  = label;
  pinMode(this->getPin(), OUTPUT);
}