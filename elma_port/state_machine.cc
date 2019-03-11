#include <iostream>
#include "elma.h"

using json = nlohmann::json;
using namespace std;

namespace elma {

    int State::_id_counter = 0;

    StateMachine& StateMachine::set_initial(State& s) {
        _initial = &s;
        return *this;
    }

    StateMachine& StateMachine::add_transition(string event_name, State& from, State& to) {
        _transitions.push_back(Transition(event_name, from, to));
        to._state_machine_ptr = this;
        from._state_machine_ptr = this;
        _stateNames.insert(std::string(from.name()));
        _stateNames.insert(std::string(to.name()));
        return *this;
    }

    void StateMachine::init() {
        for (auto transition : _transitions ) {
            watch(transition.event_name(), [this, transition](Event& e) {
                if ( _current->id() == transition.from().id() ) {
                    //// cout <<"***   +ve State Transition change ("<<transition.from().name()<<")"<<endl;
                    _current->exit(e);
                    _current = &transition.to();
                    _current->entry(e);
                    if ( !_propagate ) {
                      e.stop_propagation();
                    }
                } else {
                    //// cout <<"***   -ve State Transition change ("<<transition.from().name()<<")"<<endl;
                }
            });
        }
    }

    void StateMachine::start() {
        if ( _initial == NULL ) { 
            throw(Exception("State machine started without an initial state (call set_initial(...) first)"));
        }
        _current = _initial;
        _current->entry(Event("start"));
    }

    void StateMachine::update() {
        _current->during();
    }

    void StateMachine::stop() {}

    template <typename T>
    std::ostream& operator<<(std::ostream& os, vector<T> vec) {
        for (T elem : vec) {
            os<<elem<<" ";
        }
        return os;
    }

    vector<string> StateMachine::get_state_names() {
        vector<string> states = vector<string>();
        for (auto str : _stateNames) {
            states.push_back(str);
        }
        return states;
    }

    vector<string> StateMachine::get_state_names_old() {
        vector<string> states = vector<string>();
        // for (Transition tx : _transitions) {
        //     State& state = tx.from();
        //     // if (states.find(state.name) == )
        //     // see if state already exists
        //     // vector<string>::const_iterator it = states.begin();
        //     // for (; it != states.end(); ++it) {
        //     //     if (*it == state.name()) {
        //     //         break; // already in the list
        //     //     }
        //     // }
        //     // if (it == states.end()) {
        //     //     // not in the list

        //     // }
        //     std::vector<string>::iterator it = find(states.begin(), states.end(), state.name());
        //     if (it == states.end()) {
        //         // not in the list
        //         states.push_back(state.name());
        //     }
        //     state = tx.to();
        //     it = find(states.begin(), states.end(), state.name());
        //     if (it == states.end()) {
        //         // not in the list
        //         states.push_back(state.name());
        //     }
        // }
        cout<<"States: " <<states<<endl;
        return states;

    }

    struct transitionInfo {
        string from;
        string to;
        string when;
    };

    json StateMachine::to_json() {
        json j = {
            {"name", this->name()}
        };
        j["states"] = json(get_state_names());
        int size = _transitions.size();

        transitionInfo txs[size];
        for (int i = 0; i < size; ++i) {
            txs[i].from = _transitions[i].from().name();
            txs[i].to = _transitions[i].to().name();
            txs[i].when = _transitions[i].event_name();
        }

        json transitionsJson[size];
        string txArray = "[ ";
        for (int i = 0; i < size; ++i) {
            json js;
            js["from"] = txs[i].from;
            js["to"] = txs[i].to;
            js["when"] = txs[i].when;
            string s = js.dump();
            //// cout<<i<<": json: "<<s<<endl;
            transitionsJson[i] = js;
            txArray += s;
            if (i < (size - 1)) {
                txArray += ",";
            }
        }
        txArray += " ]";
        //// cout<<"Array of transitions: "<<txArray<<endl;
        json txComplete = json::parse(txArray);
        j["transitions"] = txComplete;

        // this older code would only work with 
        // json arrays on the stack e.g. transitionsJson[3] but
        // not with transitionsJson[_transtions.size()] or json* transitionsJson
        // so I had to switch implementation to the one above

        // stype txs[3];
        // for (int i = 0; i < 3; ++i) {
        //     txs[i].from = _transitions[i].from().name();
        //     txs[i].to = _transitions[i].to().name();
        //     txs[i].when = _transitions[i].event_name();
        // }

        // json transitionsJson[3];
        // string
        // for (int i = 0; i < 3; ++i) {
        //     cout<<i<<endl;
        //     json js;
        //     js["from"] = txs[i].from;
        //     // cout<<i<<": json: "<<js.dump()<<endl;
        //     js["to"] = txs[i].to;
        //     // cout<<i<<": json: "<<js.dump()<<endl;
        //     js["when"] = txs[i].when;
        //     string s = 
        //     cout<<i<<": json: "<<js.dump()<<endl;
        //     transitionsJson[i] = js;
        // }
        // j["transitions"] = transitionsJson;
        // delete [] transitionsJson;

        //// std::cout<<j.dump()<<std::endl;
        return j;
    }
};