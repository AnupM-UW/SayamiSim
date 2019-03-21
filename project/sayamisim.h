#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <algorithm>
#include "../elma_port/elma.h"
#include <wiringPi.h>

#include "servo.h"
#include "system.h"
#include "controller.h"
#include "display.h"
#include "UDPCommunications.h"
#include "DataRecorder.h"

using namespace elma;
using namespace std::chrono;
using namespace std;


class SayamiSim {
    public:
        SayamiSim();
        void run();
        void run(high_resolution_clock::duration duration);

    private:
        Manager _mgr;
        Servo _servo;
        System _system;
        Controller _controller;
        UDPCommunications _udpComms;
        Display _display;
        DataRecorder _dataRecorder;
        Channel _controllerChannel;
        Channel _attitudeChannel;

};

#endif
