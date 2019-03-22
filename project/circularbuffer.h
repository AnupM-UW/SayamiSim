/*
 * circularbuffer.h
 *
 *  Created on: Mar 3, 2018
 *      Converted to C++ templated class March 21, 2019
 *      Author: anupm
 *
 *      Circular buffer implementation with overwrite of least frequently used data
 *
 */

//  circularbuffer.h


#ifndef _circularbuffer_h_
#define _circularbuffer_h_

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <assert.h>
#include <exception>
#include <vector>
#include <functional>

#define TELEMETRYSIZE 35

using namespace std;

typedef string (*string_converter_t) (const void*);

//! A Circular Buffer class that is templated to take diverse type arguments
//! The capacity is defined at construction time. Oldest items are culled from
//! the buffer if the buffer gets filled.
template <typename T>
class CircularBuffer
{
  public:
    //! Constructor. Takes a default size parameter as capacity.
    //! This is a fixed size capacity (i.e. does not grow). Items
    //! added beyond the capacity/size will overwrite the oldest items
    CircularBuffer(unsigned int capacity = TELEMETRYSIZE);

    //! Default destructor
    ~CircularBuffer();

    //! Copy Constructor
    CircularBuffer(const CircularBuffer& other);

    //! Assignment operator for assignment of one circular buffer to another
    CircularBuffer& operator=(const CircularBuffer& rhs);

    //! Getter
    //! \return Whether the buffer is full, i.e. new additions will over-write the oldest values.
    bool isFull() const;

    //! Getter
    //! \return Whether the buffer is empty (no items in it).
    bool isEmpty() const;

    //! Getter
    //! \return The number of items in the circular buffer.
    unsigned int size() const;

    //! insert method. This method will insert items at
    //! the end of the circular buffer. If the buffer is already
    //! full, the oldest items will be deleted from the buffer
    //! \param data The object to be inserted at the end of the buffer
    //! \return whether the insertion was successful
    bool insert(T data);

    //! removeitem method. 
    //! Will remove the last inserted item from the 
    //! circular buffer and will return a copy of the removed item
    //! throws an exception if the buffer is empty and you try to remove an item
    //! \return a copy of the removed item
    T removeitem();

    //! printall method. 
    //! Will print to console all the items that are in the buffer, first to last
    //! \param printHandler Address of a function that can print out the data as as string
    void printall(string_converter_t printHandler) const;

    //! printdatalogtofile method. 
    //! Will print to a file all the items that are in the buffer, first to last.
    //! \param filename Name of the file to be written
    //! \param printHandler Address of a function that can print out the data as as string
    bool printdatalogtofile(const char *filename,
                   string_converter_t printHandler) const;

    // these are commented out, since std::function with string(T&) lambda instead tried to invoke string ctor with templated reference
    // Instead I had to use C-style function pointers (string_converter_t) and pass in const void* params
    //
    // void printall(std::function<string(T&)> printHandler) const;
    //
    // bool printdatalogtofile(const char *filename,
    //               std::function<strign(T&)> printHandler) const;

  private:

    unsigned int _capacity;
    int _startIdx;
    int _endIdx;

    // endIndex points 1 entry past the top of the buffer, i.e. it is already the next insertion point
    // in cases where start and end are equal, the buffer is either full, or empty,
    // hence the need for the empty flag flagEmpty
    bool _flagEmpty;
    T *_circularbuffer;
};

template <typename T>
CircularBuffer<T>::CircularBuffer(unsigned int capacity) {
    if (capacity <= 1) {
        throw std::range_error("Invalid size for circular buffer");
    }
    _capacity = capacity;
    _startIdx = 0;
    _endIdx = 0;
    _flagEmpty = true;
    _circularbuffer = new T[capacity];
}

template <typename T>
CircularBuffer<T>::~CircularBuffer() {
    delete [] _circularbuffer;
}

template <typename T>
CircularBuffer<T>::CircularBuffer(const CircularBuffer<T>& other) {
    (*this) = other;
}

template <typename T>
CircularBuffer<T>& CircularBuffer<T>::operator=(const CircularBuffer<T>& rhs) {
    cout<<"*** Copy ctor or operator= on CircularBuffer called"<<endl;
    if ( this != &rhs) {
        this->_capacity = rhs._capacity;
        this->_startIdx = rhs._startIdx;
        this->_endIdx = rhs._endIdx;
        this->_flagEmpty = rhs._flagEmpty;
        this->_circularbuffer = new T[rhs._capacity];
        for (int i = 0; i < _capacity; ++i) {
            _circularbuffer[i] = rhs._circularbuffer[i];
        }
    }
    return *this;
}

template <typename T>
bool CircularBuffer<T>::isFull() const {
    if (_endIdx == _startIdx && !_flagEmpty) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
bool CircularBuffer<T>::isEmpty() const {
    // start and end will coincide when the buffer is full or when it is empty
    return ((_startIdx == _endIdx) && _flagEmpty);
}

template <typename T>
unsigned int CircularBuffer<T>::size() const {
    if (isFull()) {
        return _capacity;  // both start and end will point at the same index,
                           // but it is not empty in this case
    } else {
        if (_startIdx > _endIdx) {
            return _endIdx + _capacity - _startIdx ;  // e.g. in a 5 element array, startIdx = 2, endIdx = $
                                                      // endIdx points past element 4, which is 0
                                                      // hence the size in this case is 0 + 5 - 2,  i.e. 3
        } else {   // startIdx is <= endIdx
            return _endIdx - _startIdx;
                    // could be 0 for empty,
                    // Also remember, endIdx points past the top of the buffer,
                    // e.g. endIdx = 4 and startIdx = 2 means there are 2 elements at index 2 and 3
        }
    }
}

template <typename T>
bool CircularBuffer<T>::insert(T data) {
    if (isFull()) {
        // buffer is full, start overwriting
        _startIdx = (_startIdx + 1) % _capacity;
    }
    _circularbuffer[_endIdx] = data;
    _endIdx = (_endIdx + 1) % _capacity; // increment endIdx, endIdx will point one past the filled portion$
    _flagEmpty = false;
    return true;
}

template <typename T>
T CircularBuffer<T>::removeitem() {
    if (isEmpty()) {
        //// cout<<"Removing from empty buffer.\n";
        throw std::range_error("removing item from empty circular buffer");
    }

    // remember, endIndex points past the top of the valid buffer, so decrement it
    _endIdx = _endIdx - 1;
    if (_endIdx < 0) {
        _endIdx += _capacity;
    }
    if (_startIdx == _endIdx) {
        // start and end became equal when removing, buffer is now empty
        _flagEmpty = true;
    }

    T retval = _circularbuffer[_endIdx];

    // clear out the data.
    _circularbuffer[_endIdx] = T(); // set it to default value

    cout<<"Removal: start:"<< _startIdx <<" end: "<< _endIdx<<endl;
    return retval;
}

template <typename T>
void CircularBuffer<T>::printall(string_converter_t printHandler) const {
// void CircularBuffer<T>::printall(std::function<string(T&)> printHandler) const {
    int start = _startIdx;
    cout<<"CircularBuffer Data: "<<endl;
    do
    {
        // string s = "";
        // string s = printHandler(_circularbuffer[start]);
        string s = printHandler(&_circularbuffer[start]);
        // int i = 4;
        // printHandler(i);//_circularbuffer[start]);
        cout<<s<<endl;
        start = (start + 1) % _capacity;
    } while (start != _endIdx);
}

template <typename T>
bool CircularBuffer<T>::printdatalogtofile(const char *filename,
                                           string_converter_t printHandler) const {
//                              std::function<void(int&)> printHandler) const {
    assert(filename != NULL);

    if(filename == NULL) {
        cout<<"filename null in printdatalogtofile of circular buffer"<<endl;
                throw std::range_error("filename null in printdatalogtofile of circular buffer");
    }

    int start = _startIdx;
    FILE* fp;

    fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "[%s,%d]Could not open datalogger file to write: %s", __FILE__, __LINE__, filename);
        return false;
    }
    fprintf(fp, "\n");

    if (!isEmpty()) {
        do {
            //string s = printHandler(_circularbuffer[start]);
            string s = printHandler(&_circularbuffer[start]);
            // int i = 5;
            // printHandler(i);//_circularbuffer[start]);
            // start and end can coincide when buffer is full too, hence the do loop to force
            fprintf(fp, s.c_str());
            fprintf(fp, "\n");
            start = (start + 1) % _capacity;
        } while (start != _endIdx);
    }

    fclose(fp);
    return true;
}


#endif
