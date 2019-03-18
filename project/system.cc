#include "system.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

System::System() : System("unnamed system") {}

System::System(string name) : Process(name),
    _lat(0), _long(0), _altitude(10000), _heading(0.0), _controllerChannel(NULL), _attitudeChannel(NULL) {
    
}

void System::init() {
    _controllerChannel = &(this->channel("controller"));
    _attitudeChannel = &(this->channel("attitude"));
}

void System::start() {}

void System::update() {
    // read input channel (controller)
    // update heading
    if (_controllerChannel->nonempty()) {
        // update attitude
        json val = _controllerChannel->latest();
        double v = val["controllerpos"];
        update_heading(v);
    }
    json attitude;
    attitude["lat"] = _lat;
    attitude["long"] = _long;
    attitude["alt"] = _altitude;
    attitude["hdg"] = _heading;

    _attitudeChannel->send(attitude);
}

void System::update_heading(double controllerInput) {
    // note that here the controller input will cause rotation about yaw axis (a bit counterintuitive if you expected roll instead of yaw)
    double relativeInput = controllerInput - 512; // 512 is neutral position
    // in one cycle, i.e. in the period of System (50 ms), a full deflection of the controller
    // will cause 5 deg rotation
    _heading += relativeInput/(double)512 * 5;
}

void System::stop() {}

