#include "sayamisim.h"
using namespace elma;
using namespace std::chrono;
using namespace std;


Servo::Servo() : Servo("unnamed servo") {}

Servo::Servo(string name) : Process(name), _attitudeChannel(NULL) {}

void Servo::init() {
    _attitudeChannel = &(this->channel("attitude"));
}

void Servo::start() {}

void Servo::update() {}

void Servo::stop() {}







