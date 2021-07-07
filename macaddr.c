#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int load_hwmac( char* mac_addr )
{
    char* mac_file = "/sys/class/net/wlan0/address";
    FILE* fd;

    fd = fopen( mac_file, "r" );
    if (fd == NULL)
    {
       perror("Error while opening MAC address file.\n");
       return 1;
    }

    fscanf( fd, "%17[^\n]", mac_addr );

    fclose( fd );

    return 0;
}

/*
const char* gethwmac()
{
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1)
    {
        perror("Error getting socket for MAC address\n ");
        return "_";
    };

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1)
    {
        printf( "ioctl failed in gethwmac\n" );
        return "_";
    }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    success = 1;
                    break;
                }
            }
        }
        else
        {
            printf( "Failed to find interface in gethwmac\n" );
            return "_";
        }
    }

    close( sock );

    char* mac_fmt = "%02x:%02x:%02x:%02x:%02x:%02x";
    char* mac_address;

    sprintf(
        mac_address,
        mac_fmt,
        (unsigned char) ifr.ifr_hwaddr.sa_data[0],
        (unsigned char) ifr.ifr_hwaddr.sa_data[1],
        (unsigned char) ifr.ifr_hwaddr.sa_data[2],
        (unsigned char) ifr.ifr_hwaddr.sa_data[3],
        (unsigned char) ifr.ifr_hwaddr.sa_data[4],
        (unsigned char) ifr.ifr_hwaddr.sa_data[5]
    );


    printf( "MAC Address: %s\n", mac_address );

    return mac_address;
}
*/
