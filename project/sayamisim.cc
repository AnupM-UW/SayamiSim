#include "sayamisim.h"

Display* currentDisplay;

string SayamiSim::clientIp = "none";

SayamiSim::SayamiSim() :
                _mgr(),
                _controller("controller"),
                _servo("servo"),
                _display("display"),
                _system("system"),
                _udpComms("udpcomms"),
                _dataRecorder("datarecorder"),
                _controllerChannel("controller"),
                _attitudeChannel("attitude") {

    ::currentDisplay = &_display;

    // initialize WiringPi using GPIO pin numbers
    wiringPiSetupGpio();

    cout<<"Wiring Pi Setup"<<endl;

}

void SayamiSim::run() {
    run(295_s);
}

void SayamiSim::run(high_resolution_clock::duration duration) {
    cout<<"IP:"<<SayamiSim::clientIp<<endl;
    if (SayamiSim::clientIp != "none") {
        _udpComms = UDPCommunications(string("udpcomms"), SayamiSim::clientIp, DEFAULT_PORT);
    }

    _mgr
    .schedule(_controller, 100_ms)
    .schedule(_system, 50_ms)
    .schedule(_display, 50_ms)
    .schedule(_servo, 500_ms)
    .schedule(_udpComms, 100_ms)
    .schedule(_dataRecorder, 200_ms)
    .add_channel(_controllerChannel)
    .add_channel(_attitudeChannel)
    .init();
    _mgr.run(duration);
}
