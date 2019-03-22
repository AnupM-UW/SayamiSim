#ifndef _DATARECORDER_H_
#define _DATARECORDER_H_

#include <iostream>
#include <string>
#include <ctime>
#include <thread>
#include "../elma_port/elma.h"
#include "circularbuffer.h"
#include "system.h"

using namespace elma;
using namespace std::chrono;
using namespace std;
using json = nlohmann::json;

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

            const int SAVE_INTERVAL = 5000; // 15000 milliseconds, or 15 seconds
                                             // we would want this to be faster in future

            Channel* _controllerChannel;
            Channel* _attitudeChannel;

            int _fileNum;

    };

#endif
