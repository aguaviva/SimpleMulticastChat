#ifdef WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#endif

#include <thread>

char mcastGroup[] = "224.1.2.3";
int mcastPort = 5435;

void PrintAddrIn(sockaddr_in addr_in)
{
    char str[255];
    inet_ntop(AF_INET, &addr_in.sin_addr, (char *)str, sizeof(str));
    printf("%s", str);
    //printf("%d.%d.%d.%d", addr_in.sin_addr.S_un..S_un.S_un_b.s_b1, addr_in.sin_addr.S_un.S_un_b.s_b2, addr_in.sin_addr.S_un.S_un_b.s_b3, addr_in.sin_addr.S_un.S_un_b.s_b4);
}

int receiver(int sck)
{
    while (1)
    {
        char buf[1024];
        memset(buf, 0, sizeof(buf));

        struct sockaddr_in addrin;
        socklen_t addrinlen = sizeof(addrin);
        memset(&addrin, 0, sizeof(addrin));
        int res = recvfrom(sck, buf, sizeof(buf), 0, (sockaddr *)&addrin, &addrinlen);
        if (res<0)
        {
            printf("Message read error\n");
            exit(1);
        }
        else
        {
            PrintAddrIn(addrin); printf(": %s\n", buf);
        }
    }
    return 0;
}

int sender(int sck)
{
    while (1)
    {
        sockaddr_in grpSck;
        memset((char *)&grpSck, 0, sizeof(grpSck));
        grpSck.sin_family = AF_INET;
        grpSck.sin_port = htons(mcastPort);
        grpSck.sin_addr.s_addr = inet_addr(mcastGroup);

        for (int i = 0; i < 100; i++)
        {
            char buf[1024];
            fgets(buf, sizeof(buf), stdin);

            if (sendto(sck, buf, strlen(buf), 0, (struct sockaddr*)&grpSck, sizeof(grpSck)) < 0)
            {
                printf("Error in sending message");
                exit(1);
            }
        }
    }
    return 0;
}

int main()
{
#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    int sck = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Set reuse
    //
    int reuse = 1;
    if (setsockopt(sck, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
    {
        perror("Socket reuse address error\n");
        exit(1);
    }
    else
    {
        printf("Socket reuse address successfull\n");
    }

    // Join mcast group
    //
    struct ip_mreq grp;
    grp.imr_multiaddr.s_addr = inet_addr(mcastGroup);
    grp.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sck, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&grp, sizeof(grp)) < 0)
    {
        printf("Error in joining group\n");
        exit(1);
    }
    else
    {
        printf("Group joined successfully\n");
    }

    // Bind socket
    //
    struct sockaddr_in lclSck;
    memset((char *)&lclSck, 0, sizeof(lclSck));
    lclSck.sin_family = AF_INET;
    lclSck.sin_port = htons(mcastPort);
    lclSck.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sck, (struct sockaddr*)&lclSck, sizeof(lclSck)))
    {
        perror("Error in binding socket\n");
        exit(1);
    }
    else
    {
        printf("Socket binding successfull\n");
    }

    u_char ttl = 5;
    setsockopt(sck, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl));

    std::thread t1([&] { receiver(sck); return 0; });

    sender(sck);

#ifdef WIN32
    WSACleanup();
#endif
}
