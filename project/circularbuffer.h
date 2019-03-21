/*
 * circularbuffer.h
 *
 *  Created on: Mar 3, 2018
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

#define TELEMETRYSIZE 35

template <typename T>
class CircularBuffer
{
  public:
    CircularBuffer(unsigned int capacity = TELEMETRYSIZE);

    ~CircularBuffer();

    CircularBuffer(const CircularBuffer& other);

    CircularBuffer& operator=(const CircularBuffer& rhs);

    bool isFull() const;

    bool isEmpty() const;

    unsigned int size() const;

    bool insert(T data);

    int removeitem(T* returnData);

    void printall() const;

    bool printdatalogtofile(const char *filename) const;

  private:
    
    unsigned int _capacity;
    int _startIdx;
    int _endIdx;

    // endIndex points 1 entry past the top of the buffer, i.e. it is already the next insertion point
    // in cases where start and end are equal, the buffer is either full, or empty,
    // hence the need for the empty flag flagEmpty
    bool _flagEmpty;
    T* _circularbuffer; //[TELEMETRYSIZE];
};


#endif
