/*
 * circularbuffer.c
 *
 *  Created on: Mar 3, 2018
 *  Updated on: Mar 21, 2019 and converted to C++ templated class instead of C
 *      Author: anupm
 *
 *      Circular buffer implementation with overwrite of least frequently used data
 *
 */

#include "circularbuffer.h"

using namespace std;

CircularBuffer::CircularBuffer(unsigned int capacity = TELEMETRYSIZE) {
	if (capacity <= 1) {
		throw std::range_error("Invalid size for circular buffer");
	}
	_capacity = _capacity;
    _startIdx = 0;
    _endIdx = 0;
    _flagEmpty = true;
	_circularbuffer = new T[_capacity];
}

CircularBuffer::~CircularBuffer() {
	delete [] _circularbuffer;
}

bool CircularBuffer::isFull(const CircularBuffer *buffer) {
	if (_endIdx == _startIdx && !_flagEmpty) {
		return true;
	} else {
		return false;
	}
}

int CircularBuffer::isEmpty() {
    // start and end will coincide when the buffer is full or when it is empty
    return ((_startIdx == _endIdx) && _flagEmpty);
}

unsigned int CircularBuffer::size() {
    if (isFull(buffer)) {
        return _capacity;  // both start and end will point at the same index,
                           // but it is not empty in this case
    } else {
        if (_startIdx > _endIdx) {
            return _endIdx + _capacity - _startIdx ;  // e.g. in a 5 element array, startIdx = 2, endIdx = 0, means that index 2, 3, 4 are filled, 
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
bool CircularBuffer::insert(T data) {
	if (isFull()) {
		// buffer is full, start overwriting
		_startIdx = (_startIdx + 1) % _capacity;
	}
	_circularbuffer[_endIdx] = data;
	_endIdx = (_endIdx + 1) % _capacity; // increment endIdx, endIdx will point one past the filled portion of the buffer
	_flagEmpty = false;
	return 1;
}

template <typename T>
T CircularBuffer::removeitem()
{
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

int CircularBuffer::printdatalogtofile(const char *filename, std::function<string(T&)> printHandler)
{
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
        return 0;
    }
    fprintf(fp, "\n");

    if (!isEmpty()) {
        do {
			string s = printHandler(_circularbuffer[start]);
			// start and end can coincide when buffer is full too, hence the do loop to force 
			fprintf(fp, s.c_str());
            start = (start + 1) % _capacity;
        } while (start != _endIdx);
    }

    fclose(fp);
    return 1;
}

void CircularBuffer::printall(std::function<string(T&)> printHandler) const {
	int start = _startIdx;
	cout<<"CircularBuffer Data: "<<endl;
	do
	{
		string s = printHandler(_circularbuffer[start]);
		cout<<s<<endl;
		start = (start + 1) % _capacity;
	} while (start != _endIdx);
}


/*

Test code for circularbuffer.c
//////////////////////////////////////

CircularBuffer* telemetryBuffer;

void InitializeTelemetry()
{
    telemetryBuffer = (CircularBuffer*) malloc(sizeof(CircularBuffer));
    telemetryBuffer->startIdx = 0;
    telemetryBuffer->endIdx = 0;
    telemetryBuffer->flagEmpty = 1;
}

int main()
{
	InitializeTelemetry();
	printall(telemetryBuffer);
	printf("IsEmpty: %d, IsFull: %d\n", isEmpty(telemetryBuffer), isFull(telemetryBuffer));
	time_t seconds_past_epoch = time(0);
	tm* locTime = localtime(&seconds_past_epoch);
	printf("Running this test on: %s", asctime(locTime));

	TelemetryData data  = { seconds_past_epoch, TON, 3, 3};
	TelemetryData data2 = { seconds_past_epoch+5, TOFF, 12, 16 };
	TelemetryData data3 = { seconds_past_epoch + 10, TOFF, 22, 25};
	insert(telemetryBuffer, &data);
	insert(telemetryBuffer, &data);
	insert(telemetryBuffer, &data);
	insert(telemetryBuffer, &data);
	insert(telemetryBuffer, &data2);
	insert(telemetryBuffer, &data3);
	insert(telemetryBuffer, &data2);
	insert(telemetryBuffer, &data3);
	printall(telemetryBuffer);
	printf("IsEmpty: %d, IsFull: %d\n", isEmpty(telemetryBuffer), isFull(telemetryBuffer));
	printf("IsEmpty: %d, IsFull: %d\n", isEmpty(telemetryBuffer), isFull(telemetryBuffer));
	printf("IsEmpty: %d, IsFull: %d\n", isEmpty(telemetryBuffer), isFull(telemetryBuffer));
	TelemetryData data4;
	remove(telemetryBuffer, &data4);
	printf("Got back: %f %d %f %ld\n", data4.desiredtemp, data4.modeswitch, data4.temp, data4.time);
	printf("IsEmpty: %d, IsFull: %d\n", isEmpty(telemetryBuffer), isFull(telemetryBuffer));
	remove(telemetryBuffer, &data4);
	remove(telemetryBuffer, &data4);
	remove(telemetryBuffer, NULL);
	printf("IsEmpty: %d, IsFull: %d\n", isEmpty(telemetryBuffer), isFull(telemetryBuffer));
	remove(telemetryBuffer, NULL);
	remove(telemetryBuffer, NULL);
	printf("IsEmpty: %d, IsFull: %d FirstIdx: %d LastIdx: %d\n", isEmpty(telemetryBuffer), isFull(telemetryBuffer), telemetryBuffer->startIdx, telemetryBuffer->endIdx);
	remove(telemetryBuffer, NULL);
	remove(telemetryBuffer, NULL);
	remove(telemetryBuffer, NULL);
	printf("IsEmpty: %d, IsFull: %d FirstIdx: %d LastIdx: %d\n", isEmpty(telemetryBuffer), isFull(telemetryBuffer), telemetryBuffer->startIdx, telemetryBuffer->endIdx);

	return 0;
}

////////// end test code

*/

