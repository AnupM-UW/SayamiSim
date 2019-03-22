/*
 * DataRecorder.h
 *
 *      Created class March 21, 2019
 *      Author: anupm
 *
 *      derived from Elma Process class
 *
 */

#ifndef _DATARECORDER_H_
#define _DATARECORDER_H_

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <thread>
#include "../elma_port/elma.h"
#include "circularbuffer.h"
#include "system.h"

using namespace elma;
using namespace std::chrono;
using namespace std;
using json = nlohmann::json;

//! A Data Recorder class
//! Derived from the Process class and gets scheduled by the Manager class
//! Listens to the Controller and Attitude channel to gather data and write it
//! to a MRU buffer of a specific size.
//! Also will periodically write the buffer content to a file on the disk

    class DataRecorder : public Process {
        public:
            //! Constructor of data recorder
            DataRecorder();

            //! Constructor ctor with name parameter
            //! \param name The name of the Process
            DataRecorder(string name);

            //! Initialization method. 
            //! It will usually be called once, after all processes and
            //! communication objects have been added to the manager, but before
            //! the Manager starts running.
            void init();

            //! Start method. 
            //! Called just before the manager starts running.
            //! It may be called multiple times, if the manager is started and stopped.
            //! Session type initialization should happen here.
            void start();

            //! Update method. Updates the data in its internal buffer. At a set 
            //! schedule the Most Recent data points will be flushed to disk as
            //! a log file.
            //! Called repeatedly by the manager at a frequency
            //! determined by the period used when the process is scheduled with the
            //! Manager (see Manager::schedule).
            void update();

            //! Stop method. All session cleanup happens here
            //! It may be called multiple times, if the manager is started and stopped.        
            void stop();


        private:

            void save_data();

            high_resolution_clock::time_point _prevSaveTime;
            CircularBuffer<json> _buffer;

            const int SAVE_INTERVAL = 15000; // 15000 milliseconds, or 15 seconds
                                             // we would want this to be faster in future

            Channel* _controllerChannel;
            Channel* _attitudeChannel;

            int _fileNum;

    };

#endif
