#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>     /* for exit() */
#include <iostream>
#include<fstream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void *ThreadMain(void *arg);             /* Main program of a thread */
void *ThreadB(void *arg);
struct ThreadArgs /* Structure of arguments to pass to client thread */
{
    int clntSock1;
    int clntSock2;                      /* Socket descriptor for client */
    int clntSock3;
    SOCKET	serv_sd; int   	cli_len;struct 	sockaddr_in  cli;
};
int main(int argc, char *argv[])
{
    //廣播
    struct ThreadArgs *threadArgs1;DWORD  threadID1;
    SOCKET	serv_sd;        /* socket 描述子 */
  	int   	cli_len, n,i=1;
  	struct 	sockaddr_in   	serv, cli;
  	WSADATA wsaData;
  	WSAStartup(0x101, &wsaData); //呼叫 WSAStartup() 註冊 WinSock DLL 的使用
   	serv_sd=socket(AF_INET, SOCK_DGRAM, 0);// 開啟 UDP socket
  	BOOL broadcast = TRUE;
  	setsockopt(serv_sd, SOL_SOCKET, SO_BROADCAST, ( const char* )&broadcast, sizeof(broadcast));
  	cli_len = sizeof(cli);
   	//printf("server start broadcasting on port:%d\n",BraodcastPort);
  	cli.sin_family      = AF_INET;
   	cli.sin_addr.s_addr = inet_addr("255.255.255.255");
   	cli.sin_port        = htons(5679);
    threadArgs1=(struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
    threadArgs1->serv_sd=serv_sd;
    threadArgs1->cli_len=cli_len;
    threadArgs1->cli=cli;

    //


    //傳訊息
    struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
    DWORD  threadID;                 /* Thread ID from CreateThread() */
    int servSock;                    /* Socket descriptor for server */
    int clntSock1,clntSock2,clntSock3;                     /* Socket descriptor for client */
    WSADATA wsadata;                 /* Structure for WinSock setup communication */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    int clntLen; /* Length of client address data structure */

    if (WSAStartup(0x101, &wsadata) != 0) /* Load Winsock 2.0 DLL */
    {
        printf("WSAStartup() failed");
        exit(1);
    }
    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        printf("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(5678);              /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        printf("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, 3) < 0)
        printf("listen() failed");

    printf("Server is waiting for clients.\n");



    clntLen = sizeof(echoClntAddr);
    /* Wait for a client to connect */
    clntSock1 = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen);
    printf("1 client connects\n");
    clntSock2 = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen);
    printf("2 clients connect\n");
    clntSock3 = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen);
    printf("3 clients connect\n");
    char str1[10];
    n=recv(clntSock1,str1, 100, 0);
    n=recv(clntSock2,str1, 100, 0);
    n=recv(clntSock3,str1, 100, 0);
    if(n<=0)return 0;

    if(str1[0]=='c') /* Run forever */
    {
    	cout<<"--------------ChatRoom--------------"<<endl;
    	printf("New chat-room with thread ID: %ld and sockets%d and %d and %d\n",threadID,clntSock1,clntSock2,clntSock3);
    	if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadB,threadArgs1,0,(LPDWORD)&threadID1)==NULL)
           printf("thread_create() failed");
    	for(;;)
    	{
           threadArgs=(struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
           threadArgs->clntSock1=clntSock1;
           threadArgs->clntSock2=clntSock2;
           threadArgs->clntSock3=clntSock3;
        if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadMain,threadArgs,0,(LPDWORD)&threadID)==NULL)
            printf("start chatting \n");
        }
    }
    //傳檔案
    else if(str1[0]=='s')
	{
		cout<<"----------Send file-----------"<<endl;
		SOCKET newsock;
		char filename[500];
		cout<<"輸入檔案名:";
		gets(filename);
		for(int i=0;i<7;i++)
		{
			cout<<filename[i];
		}cout<<endl;

		char OK[3], fileBuffer[1000];  //接收“OK”和發送的緩衝區
		struct fileMessage
		{
			char fileName[256];
			long int fileSize;
		} fileMsg;
		int size = strlen(filename);
		while (filename[size] != '\\' && size>0)
			size--;
		strcpy(fileMsg.fileName, filename + size);
		//cout<<fileMsg.fileName<<endl;
		/***open transfer file***/
		ifstream inFile(filename, ios::in | ios::binary);
		if (!inFile.is_open())
		{
			cout << "Cannot open " << filename << endl;
			closesocket(newsock);
			closesocket(serv_sd);
			WSACleanup();
			return 0;
		}
		else
		{
			cout<<"file open"<<endl;
		}
		/***文件長度***/
		inFile.seekg(0, ios::end);
		size = inFile.tellg();
		//cout<<size<<endl;
		inFile.seekg(0, ios::beg);
		fileMsg.fileSize = htonl(size);
		send(clntSock1, (char *)&fileMsg, sizeof(fileMsg)+1, 0);
		send(clntSock2, (char *)&fileMsg, sizeof(fileMsg)+1, 0);
		send(clntSock3, (char *)&fileMsg, sizeof(fileMsg)+1, 0);
		/***accept message***/
		int y;
		y=recv(clntSock1, OK, sizeof(OK), 0);
		y=recv(clntSock2, OK, sizeof(OK), 0);
		y=recv(clntSock3, OK, sizeof(OK), 0);

		if ( y<= 0)
		{
			cout << "接收error，程序退出！\n";
			closesocket(newsock);
			closesocket(serv_sd);
			WSACleanup();
			return 0;
		}
		else
		{
			cout<<"已接收"<<endl;
		}
		/***send file content***/
		if (strcmp(OK, "OK") == 0)
		{
			while (!inFile.eof())
			{
				inFile.read(fileBuffer, sizeof(fileBuffer));
				size = inFile.gcount();
				send(clntSock1, fileBuffer, size+1, 0);
				send(clntSock2, fileBuffer, size+1, 0);
				send(clntSock3, fileBuffer, size+1, 0);
			}
			cout << "file transfer finished"<<endl;
			inFile.close();
		}
		else
			cout << "無法接收文件! "<<endl;
			system("pause");
		/***finish transfer***/
		closesocket(newsock);
	}


}// end main

void *ThreadMain(void *threadArgs)//轉傳訊息
{
    //int clntSock[MaxClient];                   /* Socket descriptor for client connection */
    //int Totalclnt;
    int clntSock1;
    int clntSock2;
    int clntSock3;
    clntSock1=((struct ThreadArgs *)threadArgs)->clntSock1;
    clntSock2=((struct ThreadArgs *)threadArgs)->clntSock2;
    clntSock3=((struct ThreadArgs *)threadArgs)->clntSock3;
    char echoBuffer[1000];
    int recvMsgSize;
    u_long iMode=1;
    ioctlsocket(clntSock1,FIONBIO,&iMode);
    ioctlsocket(clntSock2,FIONBIO,&iMode);
    ioctlsocket(clntSock3,FIONBIO,&iMode);
    int nError;
    /* Send received string and receive again until end of transmission */
    while (1)      /* zero indicates end of transmission */
    {
        if ( recvMsgSize = recv(clntSock1, echoBuffer, 1000, 0)>0)
        {
            send(clntSock2, echoBuffer, strlen(echoBuffer)+1, 0);
            send(clntSock3, echoBuffer, strlen(echoBuffer)+1, 0);
        }


        nError=WSAGetLastError();
        if(nError!=WSAEWOULDBLOCK&&nError!=0)
        {
            printf("Disconnected! error code:%d\n",nError);
            break;
        }
        if(recvMsgSize = recv(clntSock2, echoBuffer, 1000, 0)>0)
        {
            send(clntSock1, echoBuffer, strlen(echoBuffer)+1, 0);
            send(clntSock3, echoBuffer, strlen(echoBuffer)+1, 0);
        }

        nError=WSAGetLastError();
        if(nError!=WSAEWOULDBLOCK&&nError!=0)
        {
            printf("Disconnected! error code:%d\n",nError);
            break;
        }
        if ( recvMsgSize = recv(clntSock3, echoBuffer, 1000, 0)>0)
        {
            send(clntSock2, echoBuffer, strlen(echoBuffer)+1, 0);
            send(clntSock1, echoBuffer, strlen(echoBuffer)+1, 0);
        }


        nError=WSAGetLastError();
        if(nError!=WSAEWOULDBLOCK&&nError!=0)
        {
            printf("Disconnected! error code:%d\n",nError);
            break;
        }
        Sleep(1000);
    }
    closesocket(clntSock1);    /* Close client socket */
    closesocket(clntSock2);
    closesocket(clntSock3);
    free(threadArgs);
    return (NULL);
}

void *ThreadB(void *threadArgs)//發廣播訊息
{
    SOCKET	serv_sd; int   	cli_len;struct 	sockaddr_in  cli;
    char str[1024];
    serv_sd=((struct ThreadArgs *)threadArgs)->serv_sd;
    cli_len=((struct ThreadArgs *)threadArgs)->cli_len;
    cli=((struct ThreadArgs *)threadArgs)->cli;
    while(1)
    {
        memset(str, '0', sizeof(str));
        gets(str);
        sendto(serv_sd, str, strlen(str), 0,(LPSOCKADDR)&cli,cli_len);
        printf("server broadcast: %s\n",str);
        system("pause");
    }
	//結束 WinSock DLL 的使用
    closesocket(serv_sd);
   	//closesocket(cli_sd);
    WSACleanup();
}
