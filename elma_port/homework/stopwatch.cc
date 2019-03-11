#include "stopwatch.h"
using namespace std;

Stopwatch::Stopwatch() : Stopwatch("Stopwatch") {
}

Stopwatch::Stopwatch(string name)  : Process(name) {
    _start = high_resolution_clock::now(); _running = false; _durationIntegrator = high_resolution_clock::duration::zero();
}

void Stopwatch::init() {
    watch("start", [this](Event& e) {
        // start the stopwatch
        //// cout<<"Stopwatch: start stopwatch\n";
        this->_start = high_resolution_clock::now();
        this->_running = true;
    });
    watch("stop", [this](Event& e) {
        // stop the stopwatch
        //// cout<<"Stopwatch: stop stopwatch\n";
        if (_running) {
            _running = false;
            high_resolution_clock::duration newDuration = high_resolution_clock::now() - _start;
            _durationIntegrator += newDuration;
        } else {
            // was already not running. Ignore it.
        }
    });
    watch("reset", [this](Event& e) {
        // reset the stopwatch
        //// cout<<"Stopwatch: reset stopwatch\n";
        _start = high_resolution_clock::now();
        _durationIntegrator = high_resolution_clock::duration::zero();

        // if somebody calls reset on stopped state, and then calls get_* without first starting the clock, they will get 
        // duration of zero.

        // OTOH, if somebody calls reset on started state, we will zero out the duration integrator and the start
    });

}

high_resolution_clock::duration Stopwatch::instantaneous_time() {
    // return duration
    // if running, it will get the instantaneous time, and keep the clock running
    // if not running, it will return the duration integration till that time
    if (_running) {
        high_resolution_clock::duration newDuration = high_resolution_clock::now() - _start;
        return _durationIntegrator + newDuration; // but don't increment _durationIntegrator
    } else {
        // was stopped
        return _durationIntegrator;
    }
}

double Stopwatch::get_minutes(){
    using namespace std;
    high_resolution_clock::duration dur = instantaneous_time();
    return minutes_type(dur).count();
}

double Stopwatch::get_seconds(){
    high_resolution_clock::duration dur = instantaneous_time();
    return seconds_type(dur).count();
}

double Stopwatch::get_milliseconds(){
    high_resolution_clock::duration dur = instantaneous_time();
    return duration<double, std::milli>(dur).count();
}
double Stopwatch::get_nanoseconds(){
    high_resolution_clock::duration dur = instantaneous_time();
    return dblnanoseconds_type(dur).count();    
}

void Stopwatch::update() {
    using namespace std;

}

void Stopwatch::start() {

}

void Stopwatch::stop() {

}

double Stopwatch::seconds() {
    return get_seconds();
}


///////////////////////
// StopWatchUser class
///////////////////////

StopWatchUser::StopWatchUser() : Process("StopWatchUser") {}

StopWatchUser::StopWatchUser(string name) : Process(name) {
    _stopwatchRunningTime = 0.0;
    _start = high_resolution_clock::now();
    _currentState = RESET;
}

void StopWatchUser::init() {
    _start = high_resolution_clock::now();
    _currentState = RESET;
}

void StopWatchUser::update() {
    using namespace std;
    high_resolution_clock::duration totalTime = high_resolution_clock::now() - _start;
    // if things have been running for more than 100ms, change the state
    // else ignore
    
    if (duration<double, std::milli>(totalTime).count() >= 100) {
        // 100 ms have elapsed, do a state change
        if (_currentState == RESET) {
            _currentState = STARTED;
            cout<<"StopWatchUser: Starting Stopwatch\n";
            emit(StopwatchStart());
        } else if (_currentState == STARTED) {
            _currentState = STOPPED;
            cout<<"StopWatchUser: Stopping Stopwatch\n";
            emit(StopwatchStop());
            high_resolution_clock::duration dur = high_resolution_clock::now() - _start;
            _stopwatchRunningTime =  duration<double, std::milli>(dur).count();
        } else if (_currentState == STOPPED) {
            _currentState = RESET;
            cout<<"StopWatchUser: Resetting Stopwatch\n";
            emit(StopwatchReset());
            _start = high_resolution_clock::now();
        }
    } else {
        // do nothing
    }
}

void StopWatchUser::stop() {
    // make sure to stop the stopwatch
    if (_currentState == STARTED) {
        _currentState = STOPPED;
        cout<<"Forced Stop\n";
        emit(StopwatchStop());
        high_resolution_clock::duration dur = high_resolution_clock::now() - _start;
        _stopwatchRunningTime =  duration<double, std::milli>(dur).count();
    } else if (_currentState == RESET){
        _stopwatchRunningTime =  0;
    }        
}

double StopWatchUser::expected_milliseconds() {
    return _stopwatchRunningTime; 
}
