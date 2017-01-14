#ifndef carat_h
#define carat_h

class CaratSink
{
public:
    virtual void OnEventTCPSocketLink(int nID, int nError) = 0;
    virtual void OnEventTCPSocketShut(int nID, int nError) = 0;
    virtual void OnEventTCPSocketRead(int nID, const void * pData, int nSize) = 0;
};

class CaratManager
{
public:
    CaratManager();
    ~CaratManager();
    
    void SetCaratSink(CaratSink * pCaratSink);
    int asynConnect(const char * sAddress, unsigned short wPort);
    int Connect(const char * sAddress, unsigned short wPort);
    void Send(int nID, unsigned short wMainCmdID, unsigned short wSubCmdID, const void * pData, int nSize);
    void Tick(float dt);
    
private:
    CaratSink * m_pCaratSink;
    int m_nBaseCaratID;
    int pubConnect(const char * sAddress, unsigned short wPort,bool isAsyn);
};

#endif /* carat_h */
