#include "sayamisim.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

Display::Display() : Display("unnamed Display") {}

Display::Display(string name) : Process(name), _attitudeChannel(NULL) {
}

void Display::init() {
    _attitudeChannel = &channel("attitude");
}

void Display::start() {}

void Display::update() {
    // there is an 
    if (_attitudeChannel->nonempty()) {
        json latest = _attitudeChannel->latest();
        do_display(latest);
    }
}

void Display::do_display(const json& att) {
    double lat = att["lat"];
    double lng = att["long"];
    double alt = att["alt"];
    double hdg = att["hdg"];
    cout<<"Display update .. "
        <<"Latitude: "<<lat<<" "
        <<"Longitude: "<<lng<<" "
        <<"Altitude: "<<alt<<" "
        <<"Heading: "<<hdg<<endl;
}

void Display::stop() {}
