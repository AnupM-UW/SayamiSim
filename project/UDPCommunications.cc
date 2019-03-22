#include "UDPCommunications.h"

using namespace elma;
using namespace std::chrono;
using namespace std;
const string DEFAULT_DEST = "192.168.1.20";

UDPCommunications::UDPCommunications() : UDPCommunications("unnamed udp client server") {}

UDPCommunications::UDPCommunications(string name) : UDPCommunications(name, DEFAULT_DEST, DEFAULT_PORT) {
}

UDPCommunications::UDPCommunications(string name, string destAddr, int port) :
                                                Process(name),
                                                _destaddr(destAddr),
                                                _socketfd(0),
                                                _port(port),
                                                // _mtx(),
                                                _event_list() {

}

void UDPCommunications::init() {
    cout<<"UDP init with dest address: "<<_destaddr<<endl;
    // create a UDP socket
    // not sure why you don't set up the 3rd param of socket as IPPROTO_UDP instead of 0
    _socketfd = socket(AF_INET, SOCK_DGRAM, 0); // note that AF_INET specifies IPv4 address

    if (_socketfd < 0) {
        perror("Cannot create socket");
        throw std::runtime_error("Could not create socket.");
    }

    // set up destination address
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // note that AF_INET specifies IPv4 address
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // let OS choose any of our addresses in case
                                              // there are multiple NICs on the machine
    addr.sin_port = htons(_port);

    if (bind(_socketfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Bind failed for local endpoint addr");
        throw std::runtime_error("Bind failed for local endpoint addr");
    }

    memset(&_dest_sockaddr, 0, sizeof(_dest_sockaddr));
    _dest_sockaddr.sin_family = AF_INET;
    _dest_sockaddr.sin_port = htons(_port);
    _dest_sockaddr.sin_addr.s_addr = inet_addr(_destaddr.c_str());

    // watch event
    watch("send_data", [this](Event& e) {
        // _mtx.lock();
        _event_list.push_back(e.value());
        // _mtx.unlock();
    });

}

void UDPCommunications::start() {
    _event_list.clear();
}

void UDPCommunications::update() {
    // taking a lock should not be needed for single threaded apps
    // _mtx.lock();

    // note that we could also send only the last message instead of all messages
    // depending upon the situation,
    // but to be sure we will send all messages
    for (json msg : _event_list) {
        string message = msg.dump();
        cout<<"UDP send: "<<message<<endl;
        int nbytes = sendto(
                            _socketfd,
                            message.c_str(),
                            strlen(message.c_str()),
                            0,
                            (struct sockaddr*) &_dest_sockaddr,
                            sizeof(_dest_sockaddr)
                        );
        if (nbytes < 0) {
            perror((string("Sendto failed: ") + message).c_str());
            // if send fails, we will not throw exception and instead just keep going
        }
    }
    _event_list.clear();

    // _mtx.unlock();
}

void UDPCommunications::stop() {
    _event_list.clear();
    // might have to do an unbind of the socket here and dispose of the socket ...
}
