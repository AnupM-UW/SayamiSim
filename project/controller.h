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
