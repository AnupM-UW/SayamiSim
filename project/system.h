#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <iostream>
#include <string>

#include <thread>
#include "../elma_port/elma.h"

typedef std::chrono::duration<double,std::ratio<1,1>> seconds_type;
typedef std::chrono::duration<double,std::ratio<60,1>> minutes_type;
typedef std::chrono::duration<double,std::milli> milliseconds_type;
typedef std::chrono::duration<double,std::ratio<1,1000000>> dblmicroseconds_type;
typedef std::chrono::duration<double,std::ratio<1,1000000000>> dblnanoseconds_type;

using namespace elma;
using namespace std::chrono;
using namespace std;

    class System : public Process {
        public:
            System();

            System(string name);

            void init();

            void start();

            void update();

            void stop();


        private:

            const int UPDATE_INTERVAL = 200; // milliseconds

            void update_heading(int);
            void update_aoa(int);

            double _lat, _long;
            double _altitude;
            double _aoa;                // angle of attack
            double _heading;            // heading in degrees
            Channel* _controllerChannel;
            Channel* _attitudeChannel;

            high_resolution_clock::time_point _prevReportTime;

    };

#endif
