#ifndef _DATARECORDER_H_
#define _DATARECORDER_H_

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <thread>
#include "../elma_port/elma.h"
#include "circularbuffer.h"
#include "system.h"

using namespace elma;
using namespace std::chrono;
using namespace std;
using json = nlohmann::json;

//! A Data Recorder class
//! Derived from the Process class and gets scheduled by the Manager class
//! Listens to the Controller and Attitude channel to gather data and write it
//! to a MRU buffer of a specific size.
//! Also will periodically write the buffer content to a file on the disk

    class DataRecorder : public Process {
        public:
            DataRecorder();

            DataRecorder(string name);

            void init();

            void start();

            void update();

            void stop();


        private:

            void save_data();

            high_resolution_clock::time_point _prevSaveTime;
            CircularBuffer<json> _buffer;

            const int SAVE_INTERVAL = 15000; // 15000 milliseconds, or 15 seconds
                                             // we would want this to be faster in future

            Channel* _controllerChannel;
            Channel* _attitudeChannel;

            int _fileNum;

    };

#endif
