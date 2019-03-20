#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for sleep()
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{

    int port = 54320; //atoi(argv[2]); // 0 if error, which is an invalid port
    const int delay_secs = 1;
    const char *message = "Harminckettesek Tere";
    const char *dest = "192.168.1.20";

    // create a UDP socket

    // not sure why you don't set up the 3rd param of socket as IPPROTO_UDP instead of 0
    int fd = socket(AF_INET, SOCK_DGRAM, 0); // note that AF_INET specifies IPv4 address

    if (fd < 0) {
        perror("Cannot create socket");
        return 1;
    }

    // set up destination address

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // note that AF_INET specifies IPv4 address
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // let OS choose any of our addresses in case
                                              // there are multiple NICs on the machine
    addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Bind failed for addr");
        return 1;
    }

    struct sockaddr_in dest_addr;    // destination address
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    if (argc == 2) {
        dest_addr.sin_addr.s_addr = inet_addr(argv[2]);
    } else {
        dest_addr.sin_addr.s_addr = inet_addr(dest);
    }
    while (1) {
        char ch = 0;
        int nbytes = sendto(
            fd,
            message,
            strlen(message),
            0,
            (struct sockaddr*) &dest_addr,
            sizeof(dest_addr)
        );
        if (nbytes < 0) {
            perror("sendto failed");
            return 1;
        }

          sleep(delay_secs); // Unix sleep is seconds
     }

    return 0;
}
