#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include <iostream>
#include <string>
#include "../elma_port/elma.h"

using namespace elma;
using namespace std::chrono;
using namespace std;


class Controller : public Process {
    public:
        Controller();

        Controller(string name);

        void init();

        void start();

        void update();

        void stop();

    private:
        Channel* _controllerChannel;
        double get_controller_pos();

};

#endif
