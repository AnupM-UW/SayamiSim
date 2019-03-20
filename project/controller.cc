#include "controller.h"
using namespace elma;
using namespace std::chrono;
using namespace std;
using namespace exploringRPi;

//#ifndef TEST_CTRL_POS
//#define TEST_CTRL_POS
//#endif

Controller::Controller() : Controller("unnamed Controller") {}

Controller::Controller(string name) : Process(name), _controllerChannel(NULL), _busDevice(NULL) {}

void Controller::init() {
    _controllerChannel = &channel("controller");
    if (_controllerChannel == nullptr) {
        throw std::runtime_error("Controller channel not found.");
    }


}

void Controller::start() {
    _busDevice = new SPIDevice(0,0);
    _busDevice->setSpeed(488000);  // The recommended SPI bus speed is approx 500kHz for the MCP3008 ADC
    _busDevice->setMode(SPIDevice::MODE0); // Mode 0. The SCLK line is low by default and read values on rising edge of CLK
    cout<<"Set up ADC"<<endl;
    unsigned char send[3], receive[3];

    send[0] = 0b00000001; // The start bit
    send[1] = 0b10010000; // MSB is single/diff bit followed by 001 for CH1
    send[2] = 0;          // not needed and ignored

    _busDevice->transfer(send, receive, 3);
    // Use the 8-bits of the second value and the two LSBs of the first value
    int valX = (int)combineValues(receive[1] & 0b00000011, receive[2]);
    cout<<"ValX: "<<valX<<endl;
}

void Controller::update() {
    // get controller pos.
    // push controller pos to channel("Controller")
    json j;
    std::tuple<double,double> cpos = get_controller_pos();
    j["controllerPosX"] = std::get<0>(cpos);
    j["controllerPosY"] = std::get<1>(cpos);
    cout<<"Controller Pos: "<<std::get<0>(cpos)<<","<<std::get<1>(cpos)<<endl;
    _controllerChannel->send(j); // updated the controller position
}

void Controller::stop() {
    // stop controller
    delete _busDevice;
}

short Controller::combineValues(unsigned char upper, unsigned char lower) {
   return ((short)upper<<8)|(short)lower;
}

int ct = 0;

std::tuple<int,int> Controller::get_controller_pos() {
    int valX = 0, valY = 0;
#ifdef TEST_CTRL_POS
    valX = (rand() % 1024 + 1);
    valY = (rand() % 1024 + 1);
#else
    unsigned char send[3], receive[3];
    // in case of MCP3008 ADC, the first byte is 0x01 which signals start of comms
    // in case of MCP3208, this is slightly different

    // for MCP3008, the values are bit7 : 1, bit8 : 1/0 for single/diff mode, bits 9-11 : the channel number or diff (e.g. 000 is channel 0)
    // e.g.
    // 0 0 0 0   0 0 0 1   1 0 0 0   0 0 0 0
    //                 ^   ^ ^ ^ ^
    // 0 0 0 0   0 0 0 0   0 0 1 1   1 1 1 1
    // 0 1 2 3   4 5 6 7   8 9 0 1   2 3 4 5

    // not sure why I have to reinit the bus device again each cycle. But this is the workaround
    // otherwise the ioctl call fails with "Invalid Argument"
    delete _busDevice;
    _busDevice = new SPIDevice(0,0);
    _busDevice->setSpeed(488000);  // The recommended SPI bus speed is approx 500kHz for the MCP3008 ADC
    _busDevice->setMode(SPIDevice::MODE0); // Mode 0. The SCLK line is low by default and read values on rising edge of CLK

    send[0] = 0b00000001; // The start bit
    send[1] = 0b10010000; // MSB is single/diff bit followed by 001 for CH1
    send[2] = 0;          // not needed and ignored

    if (ct < 10) {
       valX = 512;
    } else {

        _busDevice->transfer(send, receive, 3);
        // Use the 8-bits of the second value and the two LSBs of the first value
        valX = (int)combineValues(receive[1] & 0b00000011, receive[2]);
    }
    ++ct;

    //send[0] = 0b00000001; // The start bit
    //send[1] = 0b10100000; // MSB is single/diff bit followed by 010 for CH2
    //send[2] = 0;          // not needed and ignored
    //_busDevice->transfer(send, receive, 3);

    valY = 512; //(int)combineValues(receive[1] & 0b00000011, receive[2]);


#endif
    return std::make_tuple(valX, valY);
}
