#include <process.h>
#include <stdio.h>
#include "sclient.h"
#include "server.h"



 //构造函数
CClient::CClient(const SOCKET sClient, const sockaddr_in &addrClient)
{

	m_hThreadRecv = NULL;
	m_hThreadSend = NULL;
	m_socket = sClient;
	m_addr = addrClient;
	m_bConning = FALSE;
	m_bExit = FALSE;
	m_bSend = FALSE;
	memset(m_data.buf, 0, MAX_NUM_BUF);


	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);//手动设置信号状态，初始化为无信号状态

	//初始化临界区
	InitializeCriticalSection(&m_cs);
}

//析构函数
CClient::~CClient()
{
	closesocket(m_socket);			
	m_socket = INVALID_SOCKET;	
	DeleteCriticalSection(&m_cs);	
	CloseHandle(m_hEvent);			
}

//创建发送和接收数据线程
BOOL CClient::StartRuning(void)
{
	m_bConning = TRUE;

	//创建接收数据线程
	unsigned long ulThreadId;
	m_hThreadRecv = CreateThread(NULL, 0, RecvDataThread, this, 0, &ulThreadId);
	if(NULL == m_hThreadRecv)
	{
		return FALSE;
	}else{
		CloseHandle(m_hThreadRecv);
	}

	//创建接收客户端数据的线程
	m_hThreadSend =  CreateThread(NULL, 0, SendDataThread, this, 0, &ulThreadId);
	if(NULL == m_hThreadSend)
	{
		return FALSE;
	}else{
		CloseHandle(m_hThreadSend);
	}

	return TRUE;
}


//接收客户端数据
DWORD  CClient::RecvDataThread(void* pParam)
{
	CClient *pClient = (CClient*)pParam;	//客户端对象指针
	int		reVal;							
	char	temp[MAX_NUM_BUF];				


	while(pClient->m_bConning)				
	{
	    memset(temp, 0, MAX_NUM_BUF);
		reVal = recv(pClient->m_socket, temp, MAX_NUM_BUF, 0);	

		//处理错误返回值
		if (SOCKET_ERROR == reVal)
		{
			int nErrCode = WSAGetLastError();

			if ( WSAEWOULDBLOCK == nErrCode )	//接受数据缓冲区不可用
			{
				continue;					
			}else if (WSAENETDOWN == nErrCode ||//客户端关闭了连接
					 WSAETIMEDOUT == nErrCode ||
					WSAECONNRESET == nErrCode )
			{
				break;							
			}
		}

		//客户端关闭了连接
		if ( reVal == 0)
		{
			break;
		}

		//收到数据
		if (reVal > 0)
		{
		    EnterCriticalSection(&pClient->m_cs);
		    char *pClientIP = inet_ntoa(pClient->m_addr.sin_addr);
            u_short  clientPort = ntohs(pClient->m_addr.sin_port);
			cout<<"IP: "<<pClientIP<<"\tPort: "<<clientPort<<":"<<temp<<endl;      
			LeaveCriticalSection(&pClient->m_cs);
    


		}

	}
	pClient->m_bConning = FALSE;			//与客户端的连接断开
	return 0;								
}

//向客户端发送数据
DWORD CClient::SendDataThread(void* pParam)
{
	CClient *pClient = (CClient*)pParam;
	while(pClient->m_bConning)
	{
        if(pClient->m_bSend || bSend)
        {

			EnterCriticalSection(&pClient->m_cs);
		
			int val = send(pClient->m_socket, dataBuf, strlen(dataBuf),0);
		
			if (SOCKET_ERROR == val)
			{
				int nErrCode = WSAGetLastError();
				if (nErrCode == WSAEWOULDBLOCK)
				{
					continue;
				}else if ( WSAENETDOWN == nErrCode ||
						  WSAETIMEDOUT == nErrCode ||
						  WSAECONNRESET == nErrCode)//客户端关闭了连接
				{
					//离开临界区
					LeaveCriticalSection(&pClient->m_cs);

					pClient->m_bConning = FALSE;	
					pClient->m_bSend = FALSE;
					break;
				}else {
					//离开临界区
					LeaveCriticalSection(&pClient->m_cs);
					pClient->m_bConning = FALSE;	
					pClient->m_bSend = FALSE;
					break;
				}
			}
			memset(dataBuf, 0, MAX_NUM_BUF);		
			LeaveCriticalSection(&pClient->m_cs);
			//设置事件为无信号状态
			pClient->m_bSend = FALSE;
			bSend = FALSE;
		}

	}

	return 0;
}

