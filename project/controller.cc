#include "sayamisim.h"
using namespace elma;
using namespace std::chrono;
using namespace std;

#ifndef TEST_CTRL_POS
#define TEST_CTRL_POS
#endif

Controller::Controller() : Controller("unnamed Controller") {}

Controller::Controller(string name) : Process(name), _controllerChannel(NULL) {}

void Controller::init() {
    _controllerChannel = &channel("controller");
    if (_controllerChannel == nullptr) {
        throw std::runtime_error("Controller channel not found.");
    }

}

void Controller::start() {}

void Controller::update() {
    // get controller pos.
    // push controller pos to channel("Controller")
    json j;
    double cpos = get_controller_pos();
    j["controllerpos"] = cpos;
    cout<<"Controller Pos"<<cpos<<endl;
    _controllerChannel->send(j); // updated the controller position
}

void Controller::stop() {
    // stop controller
}

double Controller::get_controller_pos() {
    double val = 0.0;
    #ifdef TEST_CTRL_POS
    val = (double)(rand() % 1024 + 1);
    #else
    val = 512;
    #endif
    return val;
}