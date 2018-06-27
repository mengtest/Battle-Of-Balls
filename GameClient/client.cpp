#include "client.h"

//����
SOCKET	sClient;							//�׽���
HANDLE	hThreadSend;						//���������߳�
HANDLE	hThreadRecv;						//���������߳�
char    bufSend[MAX_NUM_BUF];				//�������ݻ�����
BOOL    bSend = FALSE;                      //���ͱ��λ
BOOL	bConnecting;						//�������������״̬
HANDLE	arrThread[2];						//���߳�����
CRITICAL_SECTION cs;					//�ٽ�����������bufSend

//��ʼ��������
BOOL InitClient(void)
{

	InitMember();

	
	if (!InitSockt())
	{
		return FALSE;
	}

	return TRUE;
}
//��ʼ��ȫ�ֱ���
void InitMember(void)
{
	InitializeCriticalSection(&cs);

	sClient = INVALID_SOCKET;	
    hThreadRecv = NULL;			//���������߳̾��
	hThreadSend = NULL;			//���������߳̾��
	bConnecting = FALSE;		

   
	memset(bufSend, 0, MAX_NUM_BUF);
	memset(arrThread, 0, 2);
}


 //�����������׽���
BOOL  InitSockt(void)
{
	int			reVal;	
	WSADATA		wsData;
	reVal = WSAStartup(MAKEWORD(2,2),&wsData);//��ʼ����̬��


	sClient = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == sClient)
		return FALSE;


	//�����׽��ַ�����ģʽ
	unsigned long ul = 1;
	reVal = ioctlsocket(sClient, FIONBIO, (unsigned long*)&ul);
	if (reVal == SOCKET_ERROR)
		return FALSE;

	return TRUE;
}


//���ӷ�����
BOOL ConnectServer(void)
{
	int reVal;			
	sockaddr_in serAddr;
	//����Ҫ���ӵ�������ַ
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(SERVERPORT);
    serAddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);

	while(true)
	{
	
		reVal = connect(sClient, (struct sockaddr*)&serAddr, sizeof(serAddr));
	
		if(SOCKET_ERROR == reVal)
		{
			int nErrCode = WSAGetLastError();
			if( WSAEWOULDBLOCK == nErrCode || WSAEINVAL == nErrCode)   
			{
				continue;
			}
			else if (WSAEISCONN == nErrCode)
			{
				break;
			}
			else
			{
				return FALSE;
			}
		}

		if ( reVal == 0 )
			break;
	}

	bConnecting = TRUE;

	return TRUE;
}

//�������ͺͽ��������߳�
BOOL	CreateSendAndRecvThread(void)
{
	//�����������ݵ��߳�
	unsigned long ulThreadId;
	hThreadRecv = CreateThread(NULL, 0, RecvDataThread, NULL, 0, &ulThreadId);
	if (NULL == hThreadRecv)
		return FALSE;

	//�����������ݵ��߳�
	hThreadSend = CreateThread(NULL, 0, SendDataThread, NULL, 0, &ulThreadId);
	if (NULL == hThreadSend)
		return FALSE;

	//��ӵ��߳�����
	arrThread[0] = hThreadRecv;
	arrThread[1] = hThreadSend;
	return TRUE;
}

// ���������߳�
DWORD __stdcall	RecvDataThread(void* pParam)
{
	int		reVal;				   
	char    bufRecv[MAX_NUM_BUF];   

	while(bConnecting)			   
	{
        memset(bufRecv, 0, MAX_NUM_BUF);
		reVal = recv(sClient, bufRecv, MAX_NUM_BUF, 0);
		if (SOCKET_ERROR == reVal)
		{
			int nErrCode = WSAGetLastError();
			if (WSAEWOULDBLOCK == nErrCode)			
			{
				continue;							
			}else{
				bConnecting = FALSE;
				return 0;							
			}
		}

		if ( reVal == 0)							
		{
			ShowConnectMsg(FALSE);
            bConnecting = FALSE;
            bSend = FALSE;
            memset(bufRecv, 0, MAX_NUM_BUF);		
            ExitClient();
			return 0;								
		}
		if(reVal > 0)
        {

            if(('E'==bufRecv[0] || 'e'==bufRecv[0]))     
            {
                ShowConnectMsg(FALSE);
                bConnecting = FALSE;
                bSend = FALSE;
                memset(bufRecv, 0, MAX_NUM_BUF);	
                ExitClient();
            }
            
            cout<<bufRecv<<endl;

        }
	}
	return 0;
}

  //���������߳�
DWORD __stdcall	SendDataThread(void* pParam)
{
	while(bConnecting)						
	{
		if (bSend)						
		{
            EnterCriticalSection(&cs);	//�����ٽ���
			while(TRUE)
            {
                int val = send(sClient, bufSend, MAX_NUM_BUF,0);

                if (SOCKET_ERROR == val)
                {
                    int nErrCode = WSAGetLastError();
                    if(WSAEWOULDBLOCK == nErrCode)		//���ͻ�����������
                    {
                        continue;						
                    }else
                    {
                        LeaveCriticalSection(&cs);	
                        return 0;
                    }
                }

                    bSend = FALSE;			
                    break;					
            }
            LeaveCriticalSection(&cs);	
		}
    }
	return 0;
}
// �������ݺ���ʾ���
void	InputAndOutput(void)
{
    char cInput[MAX_NUM_BUF];	
    while(bConnecting)			
	{
		memset(cInput, 0, MAX_NUM_BUF);
		cin >> cInput;			      
        EnterCriticalSection(&cs);	
		memcpy(bufSend, cInput, strlen(cInput));
		LeaveCriticalSection(&cs);		
		bSend = TRUE;
	}
}

//�ͻ����˳�
void ExitClient(void)
{
	DeleteCriticalSection(&cs);
    CloseHandle(hThreadRecv);
	CloseHandle(hThreadSend);
    memset(bufSend, 0, MAX_NUM_BUF);
	closesocket(sClient);
	WSACleanup();
}

 //��ʾ���ӷ�����ʧ����Ϣ
void ShowConnectMsg(BOOL bSuc)
{
	if (bSuc)
	{
		cout << "* Succeed to connect server! *" << endl;
	}
	else
    {
		cout << "* Client has to exit! *" << endl;
	}
}
