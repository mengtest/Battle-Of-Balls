#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED
#define _WINSOCK_DEPRECATED_NO_WAININGS

#include <iostream>
#include <winsock2.h>
#include <process.h>

#pragma comment(lib, "WS2_32.lib")
using namespace std;

#define	SERVERIP			"192.168.3.29"		
#define	SERVERPORT			5555		
#define	MAX_NUM_BUF			60			

//��������
BOOL InitClient(void);              
void InitMember(void);              //��ʼ��ȫ�ֱ���
BOOL InitSockt(void);               
BOOL ConnectServer(void);           
void ExitClient(void);             
DWORD __stdcall	RecvDataThread(void* pParam);
DWORD __stdcall	SendDataThread(void* pParam);
BOOL CreateSendAndRecvThread(void);
void InputAndOutput(void);


#endif
