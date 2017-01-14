#ifndef _NETWORKSERVICE_H_
#define _NETWORKSERVICE_H_

class CLocker;
class CNetEvent;
class CNetInfo;

class CNetworkServiceSink
{
public:
	CNetworkServiceSink() {}
	virtual ~CNetworkServiceSink() {}

	virtual void OnEventTCPSocketLink(CNetInfo* pNetInfo, int nError, const char* sErrMsg = NULL) = 0;
	virtual void OnEventTCPSocketShut(CNetInfo* pNetInfo, int nError) = 0;
	virtual void OnEventTCPSocketRead(CNetInfo* pNetInfo, const void* pData, int nSize) = 0;
};

class CNetworkService// : public CCNode
{
public:
	CNetworkService();
	virtual ~CNetworkService();

	bool Init();
	void SetNetworkServiceSink(CNetworkServiceSink * pNetworkServiceSink);

	CNetInfo * GetNetInfo(int nNetID);
	int GetNetID(CNetInfo * pNetInfo);

	CNetInfo* Connect(const char* sIP, unsigned short nPort, const char* sLocalIP, CNetInfo* pNetInfo = NULL, bool bNameResolve = false);
	void Disconnect(int nNetID, int nReason = 0);
	void Disconnect(CNetInfo* pNetInfo, int nReason = 0);
	void Send(CNetInfo* pNetInfo, const void* pData, int nSize, const TCP_Head* pHeadData = NULL);

    void Tick(unsigned int nCurrentTick);

protected:
	void UnInit();

	CNetInfo * CreateNetInfo();
	void ReleaseNetInfo(CNetInfo * pNetInfo);

	CNetEvent * CreateNetEvent(CNetInfo * pNetInfo, int nEventType, int nErrorCode = 0);
	void ReleaseNetEvent(CNetEvent * pEvent);
	CNetEvent * GetNetEvent();
	void AddNetEvent(CNetEvent * pEvent);

	void ServiceRun();

	void Close(CNetInfo* pNetInfo, int nCallType = 1, int nReason = 0, const char* sReason = NULL);

	//字节映射
	inline WORD SeedRandMap(WORD wSeed);

	int m_nConnSeed;
	CNetInfo * m_vNetInfo;
	CNetworkServiceSink * m_pNetworkServiceSink;
	CNetEvent * m_pEventQueue;
	CNetEvent * m_pFreeEventQueue;
	CLocker * m_EventLock;

	void * m_pNetworkHandle;

	friend class CASIOConnHandle;
};

#endif
