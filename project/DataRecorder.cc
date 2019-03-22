
#include "DataRecorder.h"

using namespace elma;
using namespace std::chrono;
using namespace std;
using json = nlohmann::json;

DataRecorder::DataRecorder() : DataRecorder("datarecorder") {
}

DataRecorder::DataRecorder(string name) : Process(name), _buffer(35), _fileNum(0) {

}

void DataRecorder::init() {
    _controllerChannel = &(this->channel("controller"));
    _attitudeChannel = &(this->channel("attitude"));
    // watch("send_data", [this](Event& e) {
    //     json j = e.value();

    //     j["timestamp"] =
    //     // _event_list.push_back(e.value());
    // });

}

void DataRecorder::start() {

}

void DataRecorder::update() {
    if (!_attitudeChannel->nonempty()) { return; }
    json j;
    json jatt = _attitudeChannel->latest();

    j["timestamp"] = jatt["timestamp"];
    j["lat"] = jatt["lat"];
    j["long"] = jatt["long"];
    j["alt"] = jatt["alt"];
    j["hdg"] = jatt["hdg"];
    j["aoa"] = jatt["aoa"];

    if (_controllerChannel->nonempty()) {
        json jctr = _controllerChannel->latest();
        j["controllerPosX"] = jctr["controllerPosX"];
        j["controllerPosY"] = jctr["controllerPosY"];
    } else {
        j["controllerPosX"] = "unknown";
        j["controllerPosY"] = "unknown";
    }

    _buffer.insert(j);

    high_resolution_clock::time_point now = high_resolution_clock::now();
    high_resolution_clock::duration dur = now - _prevSaveTime;

    if (milliseconds_type(dur).count() >= SAVE_INTERVAL) {
        _prevSaveTime = now;
        save_data();
    }
}

void DataRecorder::stop() {
}

void DataRecorder::save_data() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    string v = string(std::asctime(&tm));
    string needle = "\n";
    std::size_t found = v.find(needle);
    while (found != string::npos) {
        v.replace(found, needle.length(), " ");
        found = v.find(needle);
    }
    _fileNum = (_fileNum + 1) % 1000;
    string filename = v + to_string(_fileNum);
    needle = ":";
    found = filename.find(needle);
    while (found != string::npos) {
        filename.replace(found, needle.length(), "_");
        cout<<"* Saving Filename:" << filename<<endl;
        found = filename.find(needle);
    }
}

