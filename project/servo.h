#ifndef _SERVO_H_
#define _SERVO_H_

#include <iostream>
#include <string>

#include "../elma_port/elma.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

class Servo : public Process {
    public:
        Servo();

        Servo(string name);
        
        void init();
        
        void start();
        
        void update();
        
        void stop();

    private:
        Channel* _attitudeChannel;
};

#endif