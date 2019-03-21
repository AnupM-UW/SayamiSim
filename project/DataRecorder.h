#ifndef _DATARECORDER_H_
#define _DATARECORDER_H_

#include <iostream>
#include <string>

#include <thread>
#include "../elma_port/elma.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

    class DataRecorder : public Process {
        public:
            DataRecorder();

            DataRecorder(string name);

            void init();

            void start();

            void update();

            void stop();


        private:

            high_resolution_clock::time_point _prevSaveTime;

    };

#endif
