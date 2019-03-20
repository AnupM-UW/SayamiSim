#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for sleep()
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// here we will use UDP multicast groups for comms
// UDP can also be used in Peer to Peer communication 
// (see https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html)
// but here we use it for UDP multicast to a group
// (see https://linux.m2osw.com/c-implementation-udp-clientserver)

int main(int argc, char *argv[])
{
    // if (argc != 3) {
    //    printf("Command line args should be multicast group and port\n");
    //    printf("(e.g. for SSDP, `sender 239.255.255.250 1900`)\n");
    //    return 1;
    // }

    const char* group = "224.255.1.15"; // has to be 224.0.0.0/4 IP address 
    //argv[1]""; // e.g. 239.255.255.250 for SSDP
    int port = 54320; //atoi(argv[2]); // 0 if error, which is an invalid port
    const int delay_secs = 1;
    const char *message = "Hello, World!";

    // create a UDP socket
    
    // not sure why you don't set up the 3rd param of socket as IPPROTO_UDP instead of 0    
    int fd = socket(AF_INET, SOCK_DGRAM, 0); // note that AF_INET specifies IPv4 address
    
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // set up destination address

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // note that AF_INET specifies IPv4 address
    addr.sin_addr.s_addr = inet_addr(group); // group address
    addr.sin_port = htons(port);

    while (1) {
        char ch = 0;
        int nbytes = sendto(
            fd,
            message,
            strlen(message),
            0,
            (struct sockaddr*) &addr,
            sizeof(addr)
        );
        if (nbytes < 0) {
            perror("sendto");
            return 1;
        }

          sleep(delay_secs); // Unix sleep is seconds
     }

    return 0;
}