#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <iostream>
#include <string>

#include "../elma_port/elma.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

class Display : public Process {
    public:
        Display();

        Display(string name);
        
        void init();
        
        void start();
        
        void update();
        
        void stop();

    private:
        Channel* _attitudeChannel;
        void do_display(const json& att);

};

#endif