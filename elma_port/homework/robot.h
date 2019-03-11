#ifndef _ROBOT_H_
#define _ROBOT_H_

#include <iostream>
#include <chrono>
#include "elma.h"
#include <string>

using namespace elma;
using namespace std;

/*!
 * This is an internally used by Robot. We won't be testing this class.
 */
class RobotState : public State {
public:
    /*
     * Question 2: define a 'RobotState' that inherits from 'State'
     * Hint 1: you'll need to implement the 'virtual' methods and inherit both of
     * the 'State' constructors.
     */

    RobotState() : RobotState("unnamed state") {}
    RobotState(string name) : State(name) {}

};

class IdleState : public RobotState {
public:
    IdleState() : RobotState("Idle") {}
    void entry(const Event& e) {
        if (e.name() == "start") {
            cout<<"Transitioning from idle state"<<endl;
            // we are being explicit here, but the unit tests actually emit an event start
            // a la     .emit(Event("start"));
            
            //// emit(Event("start"));

        }
    }
    void during() {
        //// cout<<"Initial State\n";
    } 
    void exit(const Event& e) {}

private:

};

class WanderState : public RobotState {
public:
    WanderState() : RobotState("Wander") {}
    void entry(const Event& e) {}
    void during() {
        cout<<"Wandering\n";
    } 
    void exit(const Event& e) {}

private:
};

class FindRechargeStationState : public RobotState {
public:
    FindRechargeStationState() : RobotState("Find Recharge Station") {}
    void entry(const Event& e) {}
    void during() {
        cout<<"Finding Recharge Station\n";
    } 
    void exit(const Event& e) {}

private:
    
};

class RechargeState : public RobotState {
public:
    RechargeState() : RobotState("Recharge") {}
    void entry(const Event& e) {}
    void during() {
        cout<<"Recharging\n";
    } 
    void exit(const Event& e) {}

private:
    
};

class EvadeState : public RobotState {
public:
    EvadeState() : RobotState("Evade") {}
    void entry(const Event& e) {}
    void during() {
        cout<<"Evading\n";
    } 
    void exit(const Event& e) {}

private:
    
};

class MakeNoiseState : public RobotState {
public:
    MakeNoiseState() : RobotState("Make Noise") {}
    void entry(const Event& e) {}
    void during() {
        cout<<"Sounding Alarm\n";
    } 
    void exit(const Event& e) {}

private:
    
};

class Robot : public StateMachine {
public:
    Robot() : Robot("unnamed robot") {}

    Robot(string robotName) : StateMachine(robotName),
                            _rechargeState(), 
                            _wanderState(), 
                            _findRechargeStationState(), 
                            _startState(),
                            _evadeState(),
                            _makeNoiseState() {
        set_initial(_startState)
        .add_transition("start", _startState, _wanderState)
        .add_transition("intruder detected", _wanderState, _makeNoiseState)
        .add_transition("battery low", _wanderState, _findRechargeStationState)
        .add_transition("reset", _makeNoiseState, _wanderState)
        .add_transition("proximity warning", _makeNoiseState, _evadeState)
        .add_transition("reset", _evadeState, _makeNoiseState)
        .add_transition("battery low", _evadeState, _findRechargeStationState)
        .add_transition("found recharge station", _findRechargeStationState, _rechargeState)
        .add_transition("battery full", _rechargeState, _wanderState);
        
    }



    /*
     * Implement contructors. Constructors should instantiate states
     * and transitions AND should set initial_state so that Event('start')
     * sets the Robot to state 'Wander'
     *
     * Look at code for StateMachine.h and in examples and elma/example if you need help.
     *
     * States should have the EXACT names:
     * "Recharge"
     * "Wander"
     * "Find Recharge Station"
     * "Evade"
     * "Make Noise"
     *
     * Robot should respond to events with the EXACT names:
     * "found recharge station"
     * "battery full"
     * "battery low"
     * "start"
     * "reset"
     * "intruder detected"
     * "proximity warning"
     */

private:
    MakeNoiseState _makeNoiseState;
    EvadeState _evadeState;
    FindRechargeStationState _findRechargeStationState;
    WanderState _wanderState;
    RechargeState _rechargeState;
    IdleState _startState;
};

#endif