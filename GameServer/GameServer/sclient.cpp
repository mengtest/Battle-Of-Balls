#include <process.h>
#include <stdio.h>
#include "sclient.h"
#include "server.h"



 //���캯��
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


	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);//�ֶ������ź�״̬����ʼ��Ϊ���ź�״̬

	//��ʼ���ٽ���
	InitializeCriticalSection(&m_cs);
}

//��������
CClient::~CClient()
{
	closesocket(m_socket);			
	m_socket = INVALID_SOCKET;	
	DeleteCriticalSection(&m_cs);	
	CloseHandle(m_hEvent);			
}

//�������ͺͽ��������߳�
BOOL CClient::StartRuning(void)
{
	m_bConning = TRUE;

	//�������������߳�
	unsigned long ulThreadId;
	m_hThreadRecv = CreateThread(NULL, 0, RecvDataThread, this, 0, &ulThreadId);
	if(NULL == m_hThreadRecv)
	{
		return FALSE;
	}else{
		CloseHandle(m_hThreadRecv);
	}

	//�������տͻ������ݵ��߳�
	m_hThreadSend =  CreateThread(NULL, 0, SendDataThread, this, 0, &ulThreadId);
	if(NULL == m_hThreadSend)
	{
		return FALSE;
	}else{
		CloseHandle(m_hThreadSend);
	}

	return TRUE;
}


//���տͻ�������
DWORD  CClient::RecvDataThread(void* pParam)
{
	CClient *pClient = (CClient*)pParam;	//�ͻ��˶���ָ��
	int		reVal;							
	char	temp[MAX_NUM_BUF];				


	while(pClient->m_bConning)				
	{
	    memset(temp, 0, MAX_NUM_BUF);
		reVal = recv(pClient->m_socket, temp, MAX_NUM_BUF, 0);	

		//������󷵻�ֵ
		if (SOCKET_ERROR == reVal)
		{
			int nErrCode = WSAGetLastError();

			if ( WSAEWOULDBLOCK == nErrCode )	//�������ݻ�����������
			{
				continue;					
			}else if (WSAENETDOWN == nErrCode ||//�ͻ��˹ر�������
					 WSAETIMEDOUT == nErrCode ||
					WSAECONNRESET == nErrCode )
			{
				break;							
			}
		}

		//�ͻ��˹ر�������
		if ( reVal == 0)
		{
			break;
		}

		//�յ�����
		if (reVal > 0)
		{
		    EnterCriticalSection(&pClient->m_cs);
		    char *pClientIP = inet_ntoa(pClient->m_addr.sin_addr);
            u_short  clientPort = ntohs(pClient->m_addr.sin_port);
			cout<<"IP: "<<pClientIP<<"\tPort: "<<clientPort<<":"<<temp<<endl;      
			LeaveCriticalSection(&pClient->m_cs);
    


		}

	}
	pClient->m_bConning = FALSE;			//��ͻ��˵����ӶϿ�
	return 0;								
}

//��ͻ��˷�������
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
						  WSAECONNRESET == nErrCode)//�ͻ��˹ر�������
				{
					//�뿪�ٽ���
					LeaveCriticalSection(&pClient->m_cs);

					pClient->m_bConning = FALSE;	
					pClient->m_bSend = FALSE;
					break;
				}else {
					//�뿪�ٽ���
					LeaveCriticalSection(&pClient->m_cs);
					pClient->m_bConning = FALSE;	
					pClient->m_bSend = FALSE;
					break;
				}
			}
			memset(dataBuf, 0, MAX_NUM_BUF);		
			LeaveCriticalSection(&pClient->m_cs);
			//�����¼�Ϊ���ź�״̬
			pClient->m_bSend = FALSE;
			bSend = FALSE;
		}

	}

	return 0;
}

