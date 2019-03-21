#include "sayamisim.h"
using namespace elma;
using namespace std::chrono;
using namespace std;


Servo::Servo() : Servo("unnamed servo") {}

Servo::Servo(string name) : Process(name), _controllerChannel(NULL), _servoActive(false) {}

void Servo::init() {
    cout<<"Started Servo init"<<endl;
    _controllerChannel = &(this->channel("controller"));
    if (_controllerChannel == nullptr) {
        throw std::runtime_error("Controller channel not found.");
    }
    _servoActive = true;
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(RANGE);    // 1000 steps per pulse

    // Now we need a 50Hz signal for the Servo
    // therefore, 50 = 19.2e6 /(x * 1000)
    //   or, x = 19200000/(50*1000) = 1920 / 5 = 384;
    pwmSetClock(384);        // 19.2 MHz PWM signal from RPi PWM
                             // gives 50Hz signal

    pinMode(PWM_SERVO, PWM_OUTPUT);

    cout<<"Finished Servo init"<<endl;

    // // code to disable servo not hooked up
    // pinMode(BTN_GPIO, INPUT);
    // // set up the ISR for the button press
    // wiringPiISR(BTN_GPIO, INT_EDGE_RISING, &OnButtonPress);

}

void Servo::start() {
    cout<<"Started Servo start"<<endl;
    _servoActive = true;
    pwmWrite(PWM_SERVO, CENTER);
    cout<<"Finished Servo start"<<endl;

}

int Servo::normalize_servo_pos(int controllerPos) {
    // make it into the right range
    int retval = 0;
    // 0 corresponds to full left, 512 corresponds to CENTER
    // 1024 corresponds to full right for the controllerPos
    // we have to normalize that to range [LEFT..RIGHT] i.e. [50..100]

    double div = (double) controllerPos / (double) 1024 * (RIGHT - LEFT); // how far into thr range
    div += (double) LEFT;
    retval = (int)round(div);
    cout<<"Servo Pos:"<<retval<<endl;
    return retval;
}

void Servo::update() {
    if (!_controllerChannel->nonempty()) {
        return;
    }
    json val = _controllerChannel->latest();
    int controllerX = val["controllerPosX"];
    int normalizedVal = normalize_servo_pos(controllerX);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(RANGE);    // 1000 steps per pulse

    // Now we need a 50Hz signal for the Servo
    // therefore, 50 = 19.2e6 /(x * 1000)
    //   or, x = 19200000/(50*1000) = 1920 / 5 = 384;
    pwmSetClock(384);        // 19.2 MHz PWM signal from RPi PWM
                             // gives 50Hz signal
    pwmWrite(PWM_SERVO, normalizedVal); //normalizedVal);

}

void Servo::stop() {
    _servoActive = false;
    pwmWrite(PWM_SERVO, CENTER);

}







