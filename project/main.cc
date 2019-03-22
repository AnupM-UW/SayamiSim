#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "sayamisim.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace std;
using namespace elma;
using namespace std::chrono;

static Channel nullChannel("nullchannel");

// class SayamiSim {
//     public:

//     private:
//         Manager _mgr;
//         Servo _servo;
//         System _system;
//         Controller _controller;
//         // Display _display;
//         Channel _controllerChannel;
//         Channel _attitudeChannel;

// };

int runtests(const char *clientIp ) {
    SayamiSim sim;
    if (clientIp != NULL) {
        SayamiSim::clientIp = string(clientIp);
    }
    sim.run();
    return 0;
}

int main(int argc, char **argv)
{
    // ::testing::InitGoogleTest(&argc, argv);
    srand(time(NULL));
    if (argc >= 2) {
        cout<<"Using:"<<argv[1]<<endl;
        runtests(argv[1]);
    } else {
        cout<<"Using: default ip client"<<endl;
        runtests(NULL);
    }

    // remove the default listener
    // run
    // return RUN_ALL_TESTS();
}
