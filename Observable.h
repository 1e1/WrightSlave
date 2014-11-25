#ifndef CoreSlave_Observable_h
#define CoreSlave_Observable_h



#include "Observer.h"



class Observable {

  public:
  Observable() { this->_observers = new Warehouse<Observer*>() };

  void notifyObservers();
  void addObserver(Observer* observer) { this->_observers->push(observer); };

  protected:
  Warehouse<Observer*>* _observers;

};




/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/


void Observable::notifyObservers()
{
  Warehouse_FOREACHPP(Observer, this->_observers, element)
    element->update((Observable *) this);
  Warehouse_ENDFOREACHPP
}



#endif
