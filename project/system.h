/*
 * system.h
 *
 *      Created class March 21, 2019
 *      Author: anupm
 *
 *      Flight dynamics class derived from Elma Process class
 *
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <iostream>
#include <string>

#include <thread>
#include "../elma_port/elma.h"

typedef std::chrono::duration<double,std::ratio<1,1>> seconds_type;
typedef std::chrono::duration<double,std::ratio<60,1>> minutes_type;
typedef std::chrono::duration<double,std::milli> milliseconds_type;
typedef std::chrono::duration<double,std::ratio<1,1000000>> dblmicroseconds_type;
typedef std::chrono::duration<double,std::ratio<1,1000000000>> dblnanoseconds_type;

using namespace elma;
using namespace std::chrono;
using namespace std;

    //! A Flight Simulator Process that models flight dynamics and manages the state of the system
    class System : public Process {
        public:
            //! Constructor System ctor
            System();

            //! Constructor ctor with name parameter
            //! \param name The name of the Process
            System(string name);

            //! Initialization method. Initialize the Simulator state/dynamics
            //! It will usually be called once, after all processes and
            //! communication objects have been added to the manager, but before
            //! the Manager starts running.
            void init();

            //! Start method. 
            //! Called just before the manager starts running.
            //! It may be called multiple times, if the manager is started and stopped.
            //! Session type initialization should happen here.
            void start();

            //! Update method. Update the simulator state/dynamics
            //! Called repeatedly by the manager at a frequency
            //! determined by the period used when the process is scheduled with the
            //! Manager (see Manager::schedule).
            void update();

            //! Stop method. System session cleanup
            //! It may be called multiple times if the manager starts and stops the process.
            void stop();


        private:

            const int UPDATE_INTERVAL = 200; // milliseconds interval for update to UDP client
                                             // this is not the update interval for the system itself - misnamed

            void update_heading(int);
            void update_aoa(int);

            double _lat, _long;
            double _altitude;
            double _aoa;                // angle of attack
            double _heading;            // heading in degrees
            Channel* _controllerChannel;
            Channel* _attitudeChannel;

            high_resolution_clock::time_point _prevReportTime;

    };

#endif
