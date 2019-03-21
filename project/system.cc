#include "system.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

System::System() : System("unnamed system") {}

System::System(string name) : Process(name),
    _lat(0), 
    _long(0), 
    _altitude(10000), 
    _heading(0.0), 
    _controllerChannel(NULL), 
    _attitudeChannel(NULL), 
    _aoa(0.0) {

    _prevReportTime = high_resolution_clock::now();
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
        int v = val["controllerPosX"];
        int a = val["controllerPosY"];
        update_heading(v);
        update_aoa(a);
    }
    json attitude;
    attitude["lat"] = _lat;
    attitude["long"] = _long;
    attitude["alt"] = _altitude;
    attitude["hdg"] = _heading;
    attitude["aoa"] = _aoa;

    // we will not yet send speed, radio altitude, etc.

    _attitudeChannel->send(attitude);

    // now send updates
    high_resolution_clock::time_point now = high_resolution_clock::now();
    high_resolution_clock::duration dur = now - _prevReportTime;

    if (milliseconds_type(dur).count() > UPDATE_INTERVAL) {
        _prevReportTime = now;
        emit(Event("send_data", attitude));
    }

}

void System::update_heading(int controllerInput) {
    // note that here the controller input will cause rotation about yaw axis (a bit counterintuitive if you expected roll instead of yaw as in most joysticks and flight sims)
    // this is temporary since we don't have the yaw/rudder axis on the two axis analog joystick for RPi.
    int relativeInput = controllerInput - 512; // 512 is neutral position
    // in one cycle, i.e. in the period of System (50 ms), a full deflection of the controller
    // will cause 10/20 deg rotation, i.e. in 1 sec, it will cause 10 deg rotation
    _heading += (double)relativeInput/(double)512 * 0.5;

    if (_heading >= 360) {
        _heading -= 360;
    } else if (_heading < 0) {
        _heading += 360;
    }
}

void System::update_aoa(int controllerInput) {
    // the controller y-input will cause rotation about pitch axis
    int relativeInput = controllerInput - 512; // 512 is neutral position
    // in one cycle, i.e. in the period of System (50 ms), a full deflection of the controller in y-axis will cause
    // pitch rotation of 10/20 deg, i.e. in 1 sec, the will cause pitch change of 10 deg
    _aoa += (double)relativeInput/(double)512 * 0.5;

    if (_aoa >= 180) {
        _aoa -= 360;
    } else if (_aoa < -180) {
        _aoa += 360;
    }
}

void System::stop() {}

