#define _WINSOCK_DEPRECATED_NO_WAININGS
#include <iostream>
#include "client.h"


using namespace std;

int main()
{
    
	if (!InitClient())
	{
		ExitClient();
	}


	if(ConnectServer())
	{
		ShowConnectMsg(TRUE);
	}
	else
    {
		ShowConnectMsg(FALSE);
		ExitClient();
	}

	
	if (!CreateSendAndRecvThread())
	{
		ExitClient();

	}

	InputAndOutput();

	system("pause");
	return 0;
}
