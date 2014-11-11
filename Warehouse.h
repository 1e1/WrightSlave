#ifndef CoreSlave_Warehouse_h
#define CoreSlave_Warehouse_h



#include <Arduino.h>
#include <avr/pgmspace.h>


#define Warehouse_FOREACH(T, list, element) \
    { \
        Warehouse<T>::iterator __itr = list.begin(); \
        T element; \
        while (NULL != __itr) { \
            element = __itr->item;
#define Warehouse_ENDFOREACH \
            __itr = __itr->next; \
        } \
    }

#define Warehouse_FOREACHP(T, list, element) \
    { \
        Warehouse<T>::iterator __itr = list->begin(); \
        T element; \
        while (NULL != __itr) { \
            element = (__itr->item);
#define Warehouse_ENDFOREACHP \
            __itr = __itr->next; \
        } \
    }

#define Warehouse_FOREACHPP(T, list, element) \
    { \
        Warehouse<T*>::iterator __itr = list->begin(); \
        T* element; \
        while (NULL != __itr) { \
            element = __itr->item;
#define Warehouse_ENDFOREACHPP \
            __itr = __itr->next; \
        } \
    }



template<typename T> class Warehouse {
    
public:
    typedef struct node {
        T item;
        node* next;
    } node;
    typedef node* iterator;
    
    Warehouse();
    ~Warehouse();
    
    void push(const T item);
    
    __attribute__((always_inline)) inline iterator begin() { return this->_head; };
    __attribute__((always_inline)) inline const uint8_t size() { return this->_size; };
    
protected:
    iterator _head;
    uint8_t _size;
    
};




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




template<typename T>
Warehouse<T>::Warehouse()
{
    this->_head = NULL;
    this->_size = 0;
}


template<typename T>
Warehouse<T>::~Warehouse()
{
    for (iterator itr = this->_head; itr != NULL; this->_head = itr) {
        itr = this->_head->next;
        delete this->_head;
    }
    this->_size = 0;
}


template<typename T>
void Warehouse<T>::push(const T item)
{
    iterator itr = (iterator) new node;
    if (NULL != itr) {
        itr->item = item;
        itr->next = this->_head;
        this->_head = itr;
        this->_size++;
    }
}



#endif
