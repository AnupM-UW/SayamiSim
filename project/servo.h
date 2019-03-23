/*
 * servo.h
 *
 *      Created class March 21, 2019
 *      Author: anupm
 *
 *      Servo controller class for Raspberry Pi derived from Elma Process class
 *
 */

#ifndef _SERVO_H_
#define _SERVO_H_

#include <iostream>
#include <string>
#include <wiringPi.h>
#include <unistd.h>

#include "../elma_port/elma.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

//! A Servo class that uses PWM output to set servo postion based on joystick
//! or controlelr input. It is derived from the Process class and gets scheduled by
//! the Manager class. Listens to the Controller channel to gather data about the
//! joystick and moves the servo position to mimic the position of the joystick controller

class Servo : public Process {
    public:
        Servo();

        Servo(string name);

        //! Initialization will setup the PWM mode, calculations for 
        //! duty cycle for PWM
        //! It will usually be called once, after all processes and
        //! communication objects have been added to the manager, but before
        //! the Manager starts running.
        void init();

        //! Start method. Will set deflection to center at start
        //! Called just before the manager starts running.
        //! It may be called multiple times, if the manager is started and stopped.
        //! Session type initialization should happen here.
        void start();

        //! Update method. Will continually set the servo postion. Called 
        //! repeatedly by the manager at a frequency determined by the 
        //! period used when the process is scheduled with the Manager
        //! 
        //! Updates the servo position to match the joystick
        //! Note that the servo is artifically slowed down because the PWM pulse
        //! has a lag of about 100ms if the throw of the servo movement is large
        void update();

        //! Stop method. Will set deflection of servo to center
        //! It may be called multiple times, if the manager is started and stopped.
        //! stops the servo and moves it back to neutral position
        void stop();

        //! Normalize method. Will return size of PWM duty cycle (out of 1000)
        //! If controller pos is max 1024, it should return max value of 75 and
        //! 0 should return 25.
        //! \returns the value between 25 and 75 to normalize the value within left and right range
        int normalize_servo_pos(int controllerPos);


    private:

        Channel* _controllerChannel;

        const int RANGE  = 1000; // 1000 range for each PWM pulse
        const int CENTER = 75;   // 75/RANGE, i.e. 7.5% in our case. This is 90 deg position
                                 // this is 1.5ms duty cycle. At 50Hz, each cycle is 20ms,
                                 // so the duty cycle is
                                 // 1.5ms/20ms or 7.5%

        const int LEFT   = 50;   // per spec, about 0.5ms, or 2.5% pulse is left at 0 degrees (0.5ms)
                                 // In our case, we will use about 45 deg as left, which should be about
                                 // 1ms duty ycle linearly scaled (0.5ms =  0 deg, 1.5 ms =  90 deg).
                                 // cycle linearly scaled (0.5ms =  0 deg, 1.5 ms =  90 deg). Thus this
                                 // is 1ms/20ms or 5% duty pulse. Thus 50/1000

        const int RIGHT  = 100;  // this is about 135 degrees position, Scaling similarly,
                                 //  the duty cycle is 2ms
                                 // (note 0.5 ms = 0 deg, 1.5ms = 90 deg, 2.5 ms = 180 deg)
                                 // hence 2ms/20ms or 10% duty cycle. i.e. 100/1000


        const int PWM_SERVO = 18;   // PWM0, Pin12
        const int BTN_GPIO  = 27;   // Pin 13, GPIO27

        bool _servoActive;

};

#endif
