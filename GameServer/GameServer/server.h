#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED
#include <iostream>
#include <winsock2.h>
#include <winbase.h>
#include <vector>
#include <map>
#include <string>
#include <process.h>
#include "sclient.h"

using namespace std;
#pragma comment(lib, "ws2_32.lib")			

/**
 * 宏定义
 */
#define SERVER_SETUP_FAIL			1		//启动客户端失败
#define START_SERVER                1       //显示开始输入提示
#define INPUT_DATA                  2       //提示输入什么数据

#define SERVERPORT			5555			//服务器TCP端口
#define CONN_NUM            10              //连接客户端数量
#define TIMEFOR_THREAD_SLEEP		500		//等待客户端请求线程睡眠时间
#define TIMEFOR_THREAD_HELP			1500	//清理资源线程退出时间
#define TIMEFOR_THREAD_EXIT			5000	//主线程睡眠时间
#define WRITE_ALL   "all"                   
#define WRITE       "write"                 //发送
#define READ        "read"                  //接收
#define READ_ALL    "read all"            
typedef vector<CClient*> ClIENTVECTOR;		//向量容器
typedef vector<string> SVECTOR;             //内容字符

/**
 * 全局变量
 */
extern char	dataBuf[MAX_NUM_BUF];				//写缓冲区
extern BOOL	bConning;							//与客户端的连接状态
extern BOOL    bSend;                              //发送标记位
extern BOOL    clientConn;                         //连接客户端标记
extern SOCKET	sServer;							 //服务器监听套接字
extern CRITICAL_SECTION  cs;			            //保护数据的临界区对象
extern HANDLE	hAcceptThread;						 //数据处理线程句柄
extern HANDLE	hCleanThread;						//数据接收线程

/**
 *函数申明
 */
BOOL initSever(void);                       //初始化
void initMember(void);
bool initSocket(void);						  //初始化非阻塞套接字
void exitServer(void);						//释放资源
bool startService(void);					//启动服务器
void inputAndOutput(void);                    //处理数据
void showServerStartMsg(BOOL bSuc);          //显示错误信息
void showServerExitMsg(void);               //显示退出消息
void showTipMsg(int input);          //显示输入提示信息
void handleData(char* str);                  //数据处理
BOOL createCleanAndAcceptThread(void);      //开启监控函数
DWORD __stdcall acceptThread(void* pParam); //开启客户端请求线程
DWORD __stdcall cleanThread(void* pParam);


#endif // SERVER_H_INCLUDED
