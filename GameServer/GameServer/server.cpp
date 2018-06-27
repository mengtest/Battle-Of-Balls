#include "server.h"
#include "sclient.h"



 //全局变量
char	dataBuf[MAX_NUM_BUF];				//写缓冲区
BOOL	bConning;							//与客户端的连接状态
BOOL    bSend;                              //发送标记位
BOOL    clientConn;                         //连接客户端标记
SOCKET	sServer;							//服务器监听套接字
CRITICAL_SECTION  cs;			            //保护数据的临界区对象
HANDLE	hAcceptThread;						//数据处理线程句柄
HANDLE	hCleanThread;						//数据接收线程
ClIENTVECTOR clientvector;                  //存储子套接字
SVECTOR recvvector(CONN_NUM, "");           //存储接收数据
bool flag[CONN_NUM] = {0};


//初始化
BOOL initSever(void)
{
  
	initMember();

	if (!initSocket())
		return FALSE;

	return TRUE;
}


 //初始化全局变量
void initMember(void)
{
	InitializeCriticalSection(&cs);				           
	memset(dataBuf, 0, MAX_NUM_BUF);
	bSend = FALSE;
	clientConn = FALSE;
	bConning = FALSE;									    
	hAcceptThread = NULL;									
	hCleanThread = NULL;
	sServer = INVALID_SOCKET;							
	clientvector.clear();								
}


  //初始化SOCKET
bool initSocket(void)
{
	
	int reVal;


	WSADATA  wsData;
	reVal = WSAStartup(MAKEWORD(2,2),&wsData);

	sServer = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET== sServer)
		return FALSE;

	unsigned long ul = 1;
	reVal = ioctlsocket(sServer, FIONBIO, (unsigned long*)&ul);
	if (SOCKET_ERROR == reVal)
		return FALSE;

	//绑定套接字
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(SERVERPORT);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	reVal = bind(sServer, (struct sockaddr*)&serAddr, sizeof(serAddr));
	if(SOCKET_ERROR == reVal )
		return FALSE;

	//监听
	reVal = listen(sServer, CONN_NUM);
	if(SOCKET_ERROR == reVal)
		return FALSE;

	return TRUE;
}


// 启动服务
 
bool startService(void)
{
    BOOL reVal = TRUE;	//返回值

	showTipMsg(START_SERVER);	//提示用户输入

	char cInput;		
	do
	{
		cin >> cInput;
		if ('s' == cInput || 'S' == cInput)
		{
			if (createCleanAndAcceptThread())	//接受客户端请求的线程
			{
				showServerStartMsg(TRUE);		
			}else{
				reVal = FALSE;
			}
			break;
		}else{
			showTipMsg(START_SERVER);
		}

	} while(cInput != 's' && cInput != 'S'); 

    cin.sync();                     //清空输入缓冲区

	return reVal;

}


 //产生清理资源和接受客户端连接线程
BOOL createCleanAndAcceptThread(void)
{
    bConning = TRUE;//设置服务器为运行状态

	//创建释放资源线程
	unsigned long ulThreadId;
	//创建接收客户端请求线程
	hAcceptThread = CreateThread(NULL, 0, acceptThread, NULL, 0, &ulThreadId);
	if( NULL == hAcceptThread)
	{
		bConning = FALSE;
		return FALSE;
	}
	else
    {
		CloseHandle(hAcceptThread);
	}
	//创建接收数据线程
	hCleanThread = CreateThread(NULL, 0, cleanThread, NULL, 0, &ulThreadId);
	if( NULL == hCleanThread)
	{
		return FALSE;
	}
	else
    {
		CloseHandle(hCleanThread);
	}
	return TRUE;
}


//接受客户端连接
 
DWORD __stdcall acceptThread(void* pParam)
{
    SOCKET  sAccept;							                        //接受客户端连接的套接字
	sockaddr_in addrClient;						                        //客户端SOCKET地址

	while(bConning)						                                //服务器的状态
	{
		memset(&addrClient, 0, sizeof(sockaddr_in));					
		int	lenClient = sizeof(sockaddr_in);				        	
		sAccept = accept(sServer, (sockaddr*)&addrClient, &lenClient);
		if(INVALID_SOCKET == sAccept )
		{
			int nErrCode = WSAGetLastError();
			if(nErrCode == WSAEWOULDBLOCK)	                            
			{
				Sleep(TIMEFOR_THREAD_SLEEP);
				continue;                                             
			}
			else
            {
				return 0;                                               //线程退出
			}

		}
		else//接受客户端的请求
		{
		    clientConn = TRUE;          
		    CClient *pClient = new CClient(sAccept, addrClient);
		    EnterCriticalSection(&cs);
            //显示客户端的IP和端口
            char *pClientIP = inet_ntoa(addrClient.sin_addr);
            u_short  clientPort = ntohs(addrClient.sin_port);
            cout<<"Accept a client IP: "<<pClientIP<<"\tPort: "<<clientPort<<endl;
			clientvector.push_back(pClient);          
            LeaveCriticalSection(&cs);

            pClient->StartRuning();
		}
	}
	return 0;//线程退出
}


//清理资源线程
DWORD __stdcall cleanThread(void* pParam)
 {
    while (bConning)                  
	{
		EnterCriticalSection(&cs);//进入临界区

		//清理已经断开的连接客户端内存空间
		ClIENTVECTOR::iterator iter = clientvector.begin();
		for (iter; iter != clientvector.end();)
		{
			CClient *pClient = (CClient*)*iter;
			if (!pClient->IsConning())			//客户端线程已经退出
			{
				clientvector.erase(iter);	
				delete pClient;				
				pClient = NULL;
			}else{
				iter++;						
			}
		}
		if(clientvector.size() == 0)
        {
            clientConn = FALSE;
        }

		LeaveCriticalSection(&cs);          //离开临界区

		Sleep(TIMEFOR_THREAD_HELP);
	}


	//服务器停止工作
	if (!bConning)
	{
		//断开每个连接,线程退出
		EnterCriticalSection(&cs);
		ClIENTVECTOR::iterator iter = clientvector.begin();
		for (iter; iter != clientvector.end();)
		{
			CClient *pClient = (CClient*)*iter;
			
			if (pClient->IsConning())
			{
				pClient->DisConning();
			}
			++iter;
		}
		//离开临界区
		LeaveCriticalSection(&cs);

		//给连接客户端线程时间，使其自动退出
		Sleep(TIMEFOR_THREAD_HELP);
	}

	clientvector.clear();		//清空链表
	clientConn = FALSE;

	return 0;
 }


 //处理数据
 void inputAndOutput(void)
 {
    char sendBuf[MAX_NUM_BUF];

    showTipMsg(INPUT_DATA);

    while(bConning)
    {
        memset(sendBuf, 0, MAX_NUM_BUF);	
        cin.getline(sendBuf,MAX_NUM_BUF);	
        
        handleData(sendBuf);
    }
 }



//选择模式处理数据
 void handleData(char* str)
 {
    CClient *sClient ;
    string recvsting;
    char cnum;                //显示几号字套接字
    int num;

    if(str != NULL)
    {
       if(!strncmp(WRITE, str, strlen(WRITE))) 
        {
            EnterCriticalSection(&cs);
            str += strlen(WRITE);
            cnum = *str++;
            num = cnum - '1';
          
            if(num<clientvector.size())
            {
                sClient = clientvector.at(num);     //发送到指定客户端
                strcpy_s(dataBuf, str);
                sClient->IsSend();
                LeaveCriticalSection(&cs);
            }
            else                                    //不在范围
            {
                cout<<"The client isn't in scope!"<<endl;
                LeaveCriticalSection(&cs);
            }

        }
        else if(!strncmp(WRITE_ALL, str, strlen(WRITE_ALL)))
        {
            EnterCriticalSection(&cs);
            str += strlen(WRITE_ALL);
            strcpy_s(dataBuf, str);
            bSend = TRUE;
            LeaveCriticalSection(&cs);
        }
        else if('e'==str[0] || 'E'== str[0])    
        {
            bConning = FALSE;
            showServerExitMsg();
            Sleep(TIMEFOR_THREAD_EXIT);
            exitServer();
        }
        else
        {
            cout <<"Input error!!"<<endl;
        }

    }
 }


  //释放资源

void  exitServer(void)
{
	closesocket(sServer);					//关闭SOCKET
	WSACleanup();							//卸载Windows Sockets DLL
}

void showTipMsg(int input)
{
    EnterCriticalSection(&cs);
	if (START_SERVER == input)          //启动服务器
	{
		cout << "Please input:" ;
	}
	else if(INPUT_DATA == input)
    {
		cout << "Please input:" <<endl;
    }
	 LeaveCriticalSection(&cs);
}


//显示启动服务器成功与失败消息
void  showServerStartMsg(BOOL bSuc)
{
	if (bSuc)
	{
		cout << "* Server succeeded!  *" << endl;
	}else{	
		cout << "* Server failed   !  *" << endl;	
	}
}


//服务器退出消息
void  showServerExitMsg(void)
{
	cout << "* Server exit...     *" << endl;
}

