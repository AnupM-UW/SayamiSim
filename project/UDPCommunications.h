#ifndef _UDPCOMMS_H_
#define _UDPCOMMS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for sleep()
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include "../elma_port/elma.h"

using namespace elma;
using namespace std::chrono;
using namespace std;

    class UDPCommunications : public Process {
        public:
            UDPCommunications();

            UDPCommunications(string name);

            UDPCommunications(string name, string destAddr, int port);

            void init();
            
            void start();
            
            void update();
            
            void stop();


        private:
            
            const int DEFAULT_PORT = 54320;
            const string DEFAULT_DEST = "192.168.1.20";

            struct sockaddr_in _dest_sockaddr;
            int _socketfd; // socket file descriptor
            int _port;
            string _destaddr;
            vector<json> _event_list;
            std::mutex _mtx;
    };

#endif