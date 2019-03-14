#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;


const int RANGEE = 1000; // 1000 range for each PWM pulse
const int CENTER = 75;   // 75/RANGE, i.e. 7.5% in our case. This is 90 deg position
                         // this is 1.5ms duty cycle. At 50Hz, each cycle is 20ms, so the duty cycle is
                         // 1.5ms/20ms or 7.5%

const int LEFT   = 50;   // per spec, about 0.5ms, or 2.5% pulse is left at 0 degrees (0.5ms)
                         // In our case, we will use about 45 deg as left, which should be about 1ms duty
                         // cycle linearly scaled (0.5ms =  0 deg, 1.5 ms =  90 deg). Thus this is 1ms/20ms 
                         // or 5% duty pulse. Thus 50/1000

const int RIGHT  = 100;  // this is about 135 degrees position, Scaling similarly, the duty cycle is 2ms
                         // (note 0.5 ms = 0 deg, 1.5ms = 90 deg, 2.5 ms = 180 deg)
                         // hence 2ms/20ms or 10% duty cycle. i.e. 100/1000


#define PWM_SERVO 18
			// PWM0, Pin12
#define BTN_GPIO  27    // Pin 13, GPIO27

bool servoActive;

void ServoShutdown() {
    servoActive = false;
    pwmWrite(PWM_SERVO, CENTER);
}

void OnButtonPress() {
    ServoShutdown();
}

void ServoInit() {
    servoActive = true;
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(1000);    // 1000 steps per pulse

    // Now we need a 50Hz signal for the Servo
    // therefore, 50 = 19.2e6 /(x * 1000)
    //   or, x = 19200000/(50*1000) = 1920 / 5 = 384;
    pwmSetClock(384);        // 19.2 MHz PWM signal from RPi PWM
                             // gives 50Hz signal

}

int GetRandomServoPosition() {
    int retval = LEFT;
    int validRange = RIGHT - LEFT + 1;
    retval = rand() % validRange;
    retval += LEFT;
    return retval;
}

int main() {
    srand(time(0));
    wiringPiSetupGpio();
    pinMode(PWM_SERVO, PWM_OUTPUT);
    pinMode(BTN_GPIO, INPUT);
    // set up the ISR for the button press
    wiringPiISR(BTN_GPIO, INT_EDGE_RISING, &OnButtonPress);
    ServoInit();

    for (int i = 0; i < 10 && servoActive; ++i) {
        int value = GetRandomServoPosition();
        cout<<"Setting value to :"<<value<<endl;
        pwmWrite(PWM_SERVO, value);
        usleep(1000000); // 1 second
    }

    ServoShutdown();
    cout<<"Shutting Down"<<endl;
}
