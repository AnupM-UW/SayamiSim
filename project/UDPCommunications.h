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
#include <vector>
#include <mutex>
#include "../elma_port/elma.h"

#define DEFAULT_PORT 54320


using namespace elma;
using namespace std::chrono;
using namespace std;

    class UDPCommunications : public Process {
        public:
            //! Constructor of UDP server
            UDPCommunications();

            //! Constructor ctor with name parameter
            //! \param name The name of the Process
            UDPCommunications(string name);

            //! Constructor UDP server
            //! \param name The name of the Process
            //! \param destAddr destination address of the P2P UDP client
            //! \param port the destination port (default is 54320)
            UDPCommunications(string name, string destAddr, int port);

            //! Initialization method. Initialize the P2P connection for UDP
            //! It will usually be called once, after all processes and
            //! communication objects have been added to the manager, but before
            //! the Manager starts running.
            void init();

            //! Start method. 
            //! Called just before the manager starts running.
            //! It may be called multiple times, if the manager is started and stopped.
            //! Session type initialization should happen here.
            void start();

            //! Update method. On a schedule send UDP messages to the client with
            //! up to date state info. Client will use that info for its purposes
            //! data is sent as json
            //! determined by the period used when the process is scheduled with the
            //! Manager (see Manager::schedule).
            void update();

            //! Stop method. All session cleanup happens here
            //! It may be called multiple times, if the manager is started and stopped.        
            void stop();


        private:

            struct sockaddr_in _dest_sockaddr;
            int _socketfd; // socket file descriptor
            int _port;
            string _destaddr;
            vector<json> _event_list;

            // std::mutex _mtx;
    };

#endif
