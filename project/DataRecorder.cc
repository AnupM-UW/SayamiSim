
#include "DataRecorder.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

using json = nlohmann::json;

DataRecorder::DataRecorder() : DataRecorder("datarecorder") {
}

DataRecorder::DataRecorder(string name) : Process(name) {

}

void DataRecorder::init() {
    watch("send_data", [this](Event& e) {
        json j = e.value();

        // _event_list.push_back(e.value());
    });

}

void DataRecorder::start() {

}

void DataRecorder::update() {

}

void DataRecorder::stop() {
}

