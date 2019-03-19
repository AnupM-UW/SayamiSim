#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <iostream>
#include <string>

#include <thread>
#include "../elma_port/elma.h"

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

            void update_heading(int);
            void update_aoa(int);

            double _lat, _long;
            double _altitude;
            double _aoa;                // angle of attack
            double _heading;            // heading in degrees
            Channel* _controllerChannel;
            Channel* _attitudeChannel;

    };

#endif