#ifndef caratin_h
#define caratin_h

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "errno.h"
#include "string.h"
#include <queue>
#include <string>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>

//////////
#define SOCKET_VER 6
#define SOCKET_TCP_BUFFER 16384
#define SOCKET_TCP_PACKET (SOCKET_TCP_BUFFER - sizeof(TCP_Head))

#pragma pack(1)
struct TCP_Info
{
    unsigned char cbDataKind;
    unsigned char cbCheckCode;
    unsigned short wPacketSize;
};

struct TCP_Command
{
    unsigned short wMainCmdID;
    unsigned short wSubCmdID;
};

struct TCP_Head
{
    TCP_Info TCPInfo;
    TCP_Command CommandInfo;
};

struct tagBuffer
{
    char szBuffer[SOCKET_TCP_BUFFER];
};

//加密解密
const unsigned int g_dwPacketKey = 0xa55aa55a;


#pragma pack()
//////////

class Carat
{
public:
    Carat();
    ~Carat();
    
    
    void asynConnect(const char * sAddress, unsigned short wPort);
    pthread_mutex_t asynThread;
    static void* asynConnectThread(void * pParam);
    const char* ip;
    unsigned short port;
    
    void Connect(const char * sAddress, unsigned short wPort);
    
    void SetCaratSink(CaratSink * pCaratSink);
    void Send(unsigned short wMainCmdID, unsigned short wSubCmdID, const void * pData, int nSize);
    void SetID(int nID);
    int GetID();
    void reset();
    
    int m_nID;
    
    bool m_bIPv6;
    bool m_bIPv4;
    
    bool m_bActive;
    
    int socket1;
    struct sockaddr_in6 addr1;
    struct sockaddr_in addr2;
    struct sockaddr * pAddr1;
    int nSize;
    
    static void * thread1(void * pParam);
    
    void timer1(float dt);
    
    pthread_mutex_t mutex1;
    std::queue<tagBuffer> lQueue1;
    bool bRecvErr;
    
    long m_clockRecv;
    bool m_bRecvTimeout;
    
    char m_cbRecvBuf[SOCKET_TCP_BUFFER * 5];
    unsigned short m_wRecvSize;
    
    CaratSink * m_pCaratSink;
    
    pthread_t id;
    
    pthread_t id_connect;

    bool m_bEventLink;
    int m_nEventLinkErrorLine;
    bool m_bTimeoutHeartBeat;
    bool m_bConnect;
};


#endif /* caratin_h */
