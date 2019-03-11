#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

#include <iostream>
#include <chrono>
#include "elma.h"
#include <thread>

using namespace elma;
using namespace std::chrono;
using namespace std;

typedef std::chrono::duration<double,std::ratio<1,1>> seconds_type;
typedef std::chrono::duration<double,std::ratio<60,1>> minutes_type;
typedef std::chrono::duration<int,std::milli> milliseconds_type;
typedef std::chrono::duration<double,std::ratio<1,1000000>> dblmicroseconds_type;
typedef std::chrono::duration<double,std::ratio<1,1000000000>> dblnanoseconds_type;

#define SLEEP_MS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
#define SLEEP_S(x)  std::this_thread::sleep_for(std::chrono::seconds(x))

class Stopwatch : public Process {

    public:
        Stopwatch();

        Stopwatch(string name);
        
        void init();
        
        void start();
        
        void update();

        void stop();
        
        double seconds();

        // get durations
        double get_minutes();      // number of minutes counted
        double get_seconds();      // number of seconds counted
        double get_milliseconds(); // number of milliseconds counted
        double get_nanoseconds();  // number of nanoseconds counted

    private:
        high_resolution_clock::duration instantaneous_time();

        high_resolution_clock::time_point _start; // high_resolution_clock has resolution of nanosecond, which is required for this stopwatch
        high_resolution_clock::duration   _durationIntegrator;        
        bool _running;
};

class StopWatchUser : public Process {
    
    public:

        StopWatchUser();

        StopWatchUser(string name);
        
        void init();
        
        void start() {}
        
        void update();

        void stop();
        
        double expected_milliseconds();

    private:
        typedef enum {STARTED, STOPPED, RESET} stopwatch_state;
        stopwatch_state _currentState;
        high_resolution_clock::time_point _start; // high_resolution_clock has nanosecond accuracy
        double _stopwatchRunningTime;
};

class StopwatchStart : public Event {
    public:
        StopwatchStart() : Event("start") {}
    private:
};

class StopwatchStop : public Event {
    public:
        StopwatchStop() : Event("stop") {}
    private:

};

class StopwatchReset : public Event {
    public:
        StopwatchReset() : Event("reset") {}
    private:

};

#endif