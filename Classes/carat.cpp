#include "carat.h"
#include "caratin.h"

std::vector<Carat *> vCarat;

Carat::Carat()
{
    reset();
}

void Carat::reset()
{
    m_pCaratSink = NULL;
    m_nID = 0;
    m_bEventLink = false;
    m_bTimeoutHeartBeat = false;
    m_nEventLinkErrorLine = 0;
    m_bRecvTimeout = false;
    m_bConnect = false;
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    m_clockRecv = tv.tv_sec;
    
    m_bActive = false;
    
    m_bIPv6=false;
    m_bIPv4=false;
    
    pAddr1 = NULL;
    nSize = 0;
    
    bRecvErr=false;
    
    m_wRecvSize = 0 ;
}

Carat::~Carat()
{
    
}

void Carat::asynConnect(const char * sAddress, unsigned short wPort)
{
    printf("id_connect is 11\n");
    this->ip =sAddress;
    this->port =wPort;
    pthread_attr_t tAttr;
    pthread_attr_init(&tAttr);
    pthread_create(&id_connect, &tAttr, Carat::asynConnectThread,this);
    
}

void * Carat::asynConnectThread(void * pParam){
    Carat * pCarat = (Carat *)pParam;
    pCarat->Connect(pCarat->ip, pCarat->port);
    return NULL;
}


void Carat::Connect(const char * sAddress, unsigned short wPort)
{
    if (m_pCaratSink == NULL)
    {
        //        fprintf(stderr, "m_pCaratSink is NULL\n");
        m_nEventLinkErrorLine = __LINE__;
        return;
    }
    
    //解析地址
    struct addrinfo * paiResult, * paiCurrent;
    char szIP4[128] = {}, szIP6[128] = {};
    int nRet = getaddrinfo(sAddress, NULL, NULL, &paiResult);
    if (nRet != 0)
    {
        //        fprintf(stderr, "%s\n", gai_strerror(nRet));
        m_nEventLinkErrorLine = __LINE__;
        return;
    }
    for (paiCurrent = paiResult; paiCurrent != NULL; paiCurrent = paiCurrent->ai_next)
    {
        //        printf("%hhu %d %d %d\n", paiCurrent->ai_addr->sa_family, paiCurrent->ai_flags, paiCurrent->ai_socktype, paiCurrent->ai_protocol);
        if (paiCurrent->ai_socktype == SOCK_DGRAM) continue;
        switch (paiCurrent->ai_addr->sa_family)
        {
            case AF_INET:
            {
                inet_ntop(AF_INET, &(((struct sockaddr_in *)paiCurrent->ai_addr)->sin_addr), szIP4, sizeof(szIP4));
                m_bIPv4 = true;
                                printf("ipv4 is %s\n", szIP4);
                break;
            }
            case AF_INET6:
            {
                inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)paiCurrent->ai_addr)->sin6_addr), szIP6, sizeof(szIP6));
                m_bIPv6 = true;
                //                printf("ipv6 is %s\n", szIP6);
                break;
            }
        }
    }
    if (!m_bIPv4 && !m_bIPv6)
    {
        m_nEventLinkErrorLine = __LINE__;
        return;
    }
    
    //创建套接字
    socket1 = 0;
    if (m_bIPv6)
    {
        socket1 = socket(AF_INET6, SOCK_STREAM, 0);
        //        printf("ipv6 socket\n");
    }
    else if (m_bIPv4)
    {
        socket1 = socket(AF_INET, SOCK_STREAM, 0);
                printf("ipv4 socket\n");
    }
    //    printf("socket1 is %d\n", socket1);
    if (socket1 == -1)
    {
        //        printf("errno = %d %s\n", errno, strerror(errno));
        m_nEventLinkErrorLine = __LINE__;
        return;
    }
    
    //创建地址
    if (m_bIPv6)
    {
        bzero(&addr1, sizeof(addr1));
        addr1.sin6_family = AF_INET6;
        if (inet_pton(AF_INET6, szIP6, &addr1.sin6_addr) < 0)
        {
            //            printf("inet_pton\n");
            m_nEventLinkErrorLine = __LINE__;
            return;
        }
        addr1.sin6_port = htons(wPort);
        pAddr1 = (struct sockaddr *)&addr1;
        nSize = sizeof(addr1);
        //        printf("ipv6 addr\n");
    }
    else if (m_bIPv4)
    {
        addr2.sin_family = AF_INET;
        if (inet_pton(AF_INET, szIP4, &addr2.sin_addr) < 0)
        {
            //            printf("inet_pton\n");
            m_nEventLinkErrorLine = __LINE__;
            return;
        }
        addr2.sin_port = htons(wPort);
        pAddr1 = (struct sockaddr *)&addr2;
        nSize = sizeof(addr2);
                printf("ipv4 addr\n");
    }
    
    //建立连接
    //    printf("socket1 = %d\n", socket1);
    nRet = connect(socket1, pAddr1, nSize);
        printf("connect nRet is %d\n", nRet);
    if (nRet == 0)
    {
        //        printf("connect success\n");
        
        m_bEventLink = true;
        
        m_bConnect = true;
        
        bRecvErr = false;
        
        int nSet = 1;
        setsockopt(socket1, SOL_SOCKET, SO_NOSIGPIPE, (void *)&nSet, sizeof(int));
        
        pthread_mutex_init(&mutex1, NULL);
        
        nRet = pthread_create(&id, NULL, Carat::thread1, this);
        printf("id is %p\n", id);
        
        return;
    }
    else
    {
        //        printf("connect fail\n");
        //        printf("errno = %d %s\n", errno, strerror(errno));
        m_nEventLinkErrorLine = __LINE__;
        return;
    }
}

void Carat::SetCaratSink(CaratSink * pCaratSink)
{
    m_pCaratSink = pCaratSink;
}

void Carat::Send(unsigned short wMainCmdID, unsigned short wSubCmdID, const void * pData, int nSize)
{
    if(bRecvErr)
    {
        return ;
    }
    if (!m_bConnect)//修复send报错
    {
        return;
    }
    tagBuffer buffer1 = {};
    
    TCP_Head * s = (TCP_Head *)buffer1.szBuffer;
    s->TCPInfo.wPacketSize = sizeof(TCP_Head) + nSize;
    s->TCPInfo.cbDataKind = SOCKET_VER;
    s->CommandInfo.wMainCmdID = wMainCmdID;
    s->CommandInfo.wSubCmdID = wSubCmdID;
    memcpy(s + 1, pData, nSize);
    
    int nAlreadySend = 0;
    int nWantSend = s->TCPInfo.wPacketSize;
    while (nAlreadySend < nWantSend)
    {
        const char * pSend = (const char *)buffer1.szBuffer + nAlreadySend;
        int nSend = send(socket1, pSend, nWantSend - nAlreadySend, 0);
        if (nSend == -1 || nSend == 0)
        {
            //            printf("send error\n");
            //            printf("errno = %d %s\n", errno, strerror(errno));
            break;
        }
        nAlreadySend += nSend;
    }
    //    printf("nAlreadySend is %d\n", nAlreadySend);
    if (nAlreadySend != nWantSend)
    {
        //        printf("error\n");
    }
}

void Carat::timer1(float dt)
{
    //    printf("dt is %.2f\n", dt);
    if (dt > 1.0f)
    {
        //fprintf(stderr, "too slow!\n");
    }
    
    pthread_mutex_lock(&mutex1);
    //printf("queue size is %u\n", (unsigned int)lQueue1.size());
    while (!lQueue1.empty())
    {
        tagBuffer buffer1 = lQueue1.front();
        TCP_Head * pTemp = (TCP_Head *)buffer1.szBuffer;
        
        m_pCaratSink->OnEventTCPSocketRead(GetID(), buffer1.szBuffer, pTemp->TCPInfo.wPacketSize);
        
        lQueue1.pop();
    }
    pthread_mutex_unlock(&mutex1);
    
    if (m_bEventLink)
    {
        m_bEventLink = false;
        m_pCaratSink->OnEventTCPSocketLink(GetID(), 0);
    }
    
    if (m_nEventLinkErrorLine != 0)
    {
        m_pCaratSink->OnEventTCPSocketLink(GetID(), m_nEventLinkErrorLine);
        m_nEventLinkErrorLine = 0;
    }
    
    //    printf("m_clockRecv is %ld\n", m_clockRecv);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long lElapse = tv.tv_sec - m_clockRecv;
    //    printf("lElapse is %ld %d\n", lElapse, m_bTimeoutHeartBeat);
    if (lElapse > 5 && !m_bTimeoutHeartBeat)
    {
        m_bTimeoutHeartBeat = true;
        Send(0, 4, NULL, 0);
        //        printf("send heart\n");
    }
    
    if (lElapse > 10 && !m_bRecvTimeout && m_bActive)
    {
        m_bRecvTimeout = true;
        m_bActive = false;
        pthread_cancel(id);
        printf("lElapse is %ld, m_bRecvTimeout is %d, %p\n", lElapse, m_bRecvTimeout, id);
        m_pCaratSink->OnEventTCPSocketShut(GetID(), -1);
        //        printf("shut\n");
    }
}

void * Carat::thread1(void * pParam)
{
    printf("hello world\n");
    pthread_detach(pthread_self());
    Carat * pCarat = (Carat *)pParam;
    while (true)
    {
        usleep(100000);
        if (pCarat->bRecvErr)
        {
            //            printf("bRecvErr %lu\n", (unsigned long)pthread_self());
            usleep(1000000);//防止出错的时候占用处理器
            continue;
        }
        int nRecv = recv(pCarat->socket1, pCarat->m_cbRecvBuf + pCarat->m_wRecvSize, sizeof(pCarat->m_cbRecvBuf) - pCarat->m_wRecvSize, 0);
        if (nRecv == -1 || nRecv == 0)
        {
            //            printf("recv error\n");
            //            printf("nRecv is %d\n", nRecv);
            //
            pCarat->bRecvErr = true;
            pCarat->m_bActive = false;
            printf("2 errno = %d %s\n", errno, strerror(errno));
            pCarat->m_pCaratSink->OnEventTCPSocketShut(pCarat->GetID(), errno);
            //            continue;
            return NULL;
        }
        
        //        printf("nRecv is %d\n", nRecv);
        
        struct timeval tv;
        gettimeofday(&tv, NULL);
        //        printf("tv is %ld %d\n", tv.tv_sec, tv.tv_usec);
        pCarat->m_clockRecv = tv.tv_sec;
        pCarat->m_bTimeoutHeartBeat = false;
        
        pCarat->m_wRecvSize += nRecv;
        TCP_Head * pTCP_Head = (TCP_Head *)pCarat->m_cbRecvBuf;
        
        try
        {
            tagBuffer buffer1 = {};
            while (pCarat->m_wRecvSize >= sizeof(TCP_Head))
            {
                unsigned short wPacketSize = pTCP_Head->TCPInfo.wPacketSize;
                if (pCarat->m_wRecvSize < wPacketSize)
                {
                    break;
                }
                
                //                printf("当前收到的数据长度: %d \n  本条协议头中标识的数据长度: %d \n", pCarat->m_wRecvSize,wPacketSize );
                
                
                memcpy(buffer1.szBuffer, pCarat->m_cbRecvBuf, wPacketSize);
                pCarat->m_wRecvSize -= wPacketSize;
                memmove(pCarat->m_cbRecvBuf, pCarat->m_cbRecvBuf + wPacketSize, pCarat->m_wRecvSize);
                
                pthread_mutex_lock(&pCarat->mutex1);
                pCarat->lQueue1.push(buffer1);
                pthread_mutex_unlock(&pCarat->mutex1);
            }
        }
        catch (...)
        {
            
        }
    }
    return NULL;
}

void Carat::SetID(int nID)
{
    m_nID = nID;
}

int Carat::GetID()
{
    return m_nID;
}

CaratManager::CaratManager()
{
    m_pCaratSink = NULL;
    vCarat.clear();
    m_nBaseCaratID = 1;
}

CaratManager::~CaratManager()
{
    
}

void CaratManager::SetCaratSink(CaratSink * pCaratSink)
{
    m_pCaratSink = pCaratSink;
}

int CaratManager::asynConnect(const char * sAddress, unsigned short wPort){
    
    return pubConnect(sAddress,wPort,true);
    
}

int CaratManager::Connect(const char * sAddress, unsigned short wPort)
{
    return pubConnect(sAddress,wPort,false);
    
}

int CaratManager::pubConnect(const char * sAddress, unsigned short wPort,bool isAsyn){
    if (m_pCaratSink == NULL)
    {
        //        fprintf(stderr, "m_pCaratSink is NULL\n");
        return -1;
    }
    
    Carat * pCarat = NULL;
    bool bHave = false;
    for (unsigned int i = 0; i < vCarat.size(); i++)
    {
        if (!vCarat[i]->m_bActive)
        {
            bHave = true;
            pCarat = vCarat[i];
            break;
        }
    }
    
    if (!bHave)
    {
        pCarat = new Carat;
        vCarat.push_back(pCarat);
    }
    
    pCarat->reset();
    pCarat->m_bActive = true;
    pCarat->SetID(m_nBaseCaratID++);
    pCarat->SetCaratSink(m_pCaratSink);
    pCarat->Connect(sAddress, wPort);
    
    return pCarat->GetID();
    
}

void CaratManager::Send(int nID, unsigned short wMainCmdID, unsigned short wSubCmdID, const void * pData, int nSize)
{
    for (int i = 0; i < vCarat.size(); i++)
    {
        Carat * pCarat = vCarat[i];
        if (pCarat->GetID() == nID)
        {
            pCarat->Send(wMainCmdID, wSubCmdID, pData, nSize);
            break;
        }
    }
}

void CaratManager::Tick(float dt)
{
    for (int i = 0; i < vCarat.size(); i++)
    {
        Carat * pCarat = vCarat[i];
        if (pCarat->m_bRecvTimeout) continue;
        pCarat->timer1(dt);
    }
}
