/*
 * controller.h
 *
 *      Created class March 21, 2019
 *      Author: anupm
 *
 *      Joystick controller class for Raspberry Pi derived from Elma Process class
 *
 */

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include <iostream>
#include <string>
#include "../elma_port/elma.h"
#include <sstream>
#include <unistd.h>
#include "SPIDevice.h"

using namespace elma;
using namespace std::chrono;
using namespace std;
using namespace exploringRPi;

//! A Controller class for joystick input
//! Derived from the Process class and gets scheduled by the Manager class
//! Will write to the Controller Channel the values read by the joystick (X and Y direction)
//! Joystick hat switch is not used by the program and hence not available

class Controller : public Process {
    public:
        //! Constructor for Joystick controller
        Controller();

        //! Constructor
        //! \param name The name of the Process
        Controller(string name);

        //! Initialization method. 
        //! It will usually be called once, after all processes and
        //! communication objects have been added to the manager, but before
        //! the Manager starts running.
        void init();

        //! Start method. Session specific state is created
        //! Called just before the manager starts running.
        //! It may be called multiple times, if the manager is started and stopped.
        //! Session type initialization should happen here.
        void start();

        //! Update method. Updates made on repeated calls by the manager
        //! at some set frequency determined by the manage. The joystick data
        //! is read during update and put into the "controller" channel for
        //! other processes to read
        //! determined by the period used when the process is scheduled with the
        //! Manager (see Manager::schedule).
        void update();

        //! Stop method. Cleans up session specific bus device info
        //! It may be called multiple times, if the manager is started and stopped.        
        void stop();

    private:

        Channel *_controllerChannel;
        std::tuple<int,int> get_controller_pos();
        short combineValues(unsigned char upper, unsigned char lower);
        SPIDevice *_busDevice;

};

#endif
