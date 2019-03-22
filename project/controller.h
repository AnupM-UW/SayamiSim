#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include <iostream>
#include <string>
#include "../elma_port/elma.h"
#include <sstream>
#include <unistd.h>
#include "SPIDevice.h"

using namespace elma;
using namespace std::chrono;
using namespace std;
using namespace exploringRPi;

//! A Controller class for joystick input
//! Derived from the Process class and gets scheduled by the Manager class
//! Will write to the Controller Channel the values read by the joystick (X and Y direction)
//! Joystick hat switch is not used by the program and hence not available

class Controller : public Process {
    public:
        Controller();

        Controller(string name);

        void init();

        void start();

        void update();

        void stop();

    private:

        Channel *_controllerChannel;
        std::tuple<int,int> get_controller_pos();
        short combineValues(unsigned char upper, unsigned char lower);
        SPIDevice *_busDevice;

};

#endif
