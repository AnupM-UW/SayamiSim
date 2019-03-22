/*
 * sayamisim.h
 *
 *      Created class March 21, 2019
 *      Author: anupm
 *
 *      Main class for Raspberry Pi flight simulator SayamiSim
 *
 */

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

//! The main class for the flight simulator.
//! It is a manager for all the subsystems in the program. Creates the manager
//! and schedules processes, events and channels for communication

class SayamiSim {
    public:
        //! Constructor
        SayamiSim();

        //! Run method. Runs for a default specified time
        void run();

        //! Run method. Runs for the specified time
        void run(high_resolution_clock::duration duration);

        //! static value that sets the client IP to communicate via UDP
        //! Note that this value must be set before the run() method is called
        //! to take effect
        static string clientIp; // for UDP client if there is one

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
