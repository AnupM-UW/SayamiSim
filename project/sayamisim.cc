#include "sayamisim.h"

Display* currentDisplay;

SayamiSim::SayamiSim() : 
                _mgr(), 
                _controller("controller"), 
                _servo("servo"),  
                _display("display"),
                _system("system"), 
                _controllerChannel("controller"),
                _attitudeChannel("attitude") {
    ::currentDisplay = &_display;
    _mgr
    .schedule(_controller, 100_ms)
    .schedule(_system, 50_ms)
    .schedule(_display, 50_ms)
    .schedule(_servo, 100_ms)
    .add_channel(_controllerChannel)
    .add_channel(_attitudeChannel)
    .init();
}

void SayamiSim::run() {
    _mgr.run(25_s);
}

void SayamiSim::run(high_resolution_clock::duration duration) {
    _mgr.run(duration);
}
