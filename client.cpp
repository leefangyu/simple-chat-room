#include <stdio.h>
#include<string.h>
#include <winsock.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<direct.h>
#define MAXLINE 1024
using namespace std;

void *ThreadMain(void *arg);
void *ThreadBr(void *arg);
struct ThreadArgs
{
    SOCKET sd,sd1;struct sockaddr_in serv1;int serv_len;
};

int main(int argc, char** argv)
{
    //廣播
   SOCKET        	sd1;
   struct sockaddr_in serv1,cli;
   char  		str1[1024];
   WSADATA 		wsaData;
   int n0,serv_len;
   DWORD threadID1;
   struct ThreadArgs *threadArgs1;
   WSAStartup(0x101,(LPWSADATA) &wsaData); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
   sd1=socket(AF_INET, SOCK_DGRAM, 0);
   SOCKET client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client == -1)
	{
		cout << "Socket failed." << endl;
		return -1;
	}
   BOOL broadcast = TRUE;
   if(	setsockopt(sd1, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast))<0)
   printf("setsockopt() error!\n");
   cli.sin_family       = AF_INET;
   cli.sin_addr.s_addr  = 0;
   cli.sin_port         = htons(5679);
   if( bind(sd1, (LPSOCKADDR) &cli, sizeof(cli)) <0 ){
       printf("bind error!\n");
    }
        serv_len=sizeof(serv1);
        threadArgs1=(struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
        threadArgs1->sd1=sd1;
        threadArgs1->serv1=serv1;
        threadArgs1->serv_len=serv_len;
    //


    //傳訊息
    SOCKET        	sd;
    struct sockaddr_in serv;
    WSADATA 		wsadata;
    char  str[1024];
    DWORD threadID;
    struct ThreadArgs *threadArgs;
    int n;

    WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用

    sd=socket(AF_INET, SOCK_STREAM, 0);

    threadArgs=(struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
    threadArgs->sd=sd;

    serv.sin_family       = AF_INET;
    serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
    serv.sin_port         = htons(5678);

    connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // 連接至 echo server


    cout<<"Enter your name:"<<endl;
    char name[10],name1[10];
    char type[10];
    gets(name);name[strlen(name)]=':';
    cout<<"Hello~chat or server sends?"<<endl;
    gets(type);
    send(sd,type,strlen(type)+1,0);
    strcpy(name1,name);
    if(type[0]=='c')
    {
    	if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadBr,threadArgs1,0,(LPDWORD)&threadID1)==NULL)
    	printf("CreateThread() failed");
    	while(1)
    	{
    		memset(str,0,1024);
    		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadMain,threadArgs,0,(LPDWORD)&threadID)==NULL)
               printf("CreateThread() failed");
            memset(str,0,1024);
            gets(str);
            strcat(name,str);strcpy(str,name);
        	n=send(sd,str,strlen(str)+1,0);cout<<str<<endl;
        	strcpy(name,name1);
        	if(n<0)
            	printf("send error.\n");
        }
    }
    else if(type[0]=='s')
    {
	struct fileMessage
	{
			char fileName[256];
			long int fileSize;
	} fileMsg;
    long int filelen;
    char filename[500]="C:/Users/Administrator/Desktop/np期末pj";
    char ok[3]="OK";
    char fileBuffer[1000];
	_mkdir(filename);
	int r;
	r=recv(sd,(char *)&fileMsg,sizeof(fileMsg)+1,0);
	cout<<r<<endl;
	cout<<"file address:"<<filename<<endl;
	if(r<=0)
	{
		cout<<"recv error"<<endl;
		closesocket(sd);
		WSACleanup();
        return 0;
	}
	else if(r>0)
	{
		cout<<"recv file!"<<endl;
	}
	filelen=ntohl(fileMsg.fileSize);
    strcat(filename,fileMsg.fileName);
	ofstream outFile(filename,ios::out|ios::binary);
	if(!outFile.is_open())
	{
		cout<<"cannot open "<<filename<<endl;
		closesocket(sd);
		WSACleanup();
		return 0;
	}
	else
	{
		cout<<"file open"<<endl;
	}
	send(sd,ok,sizeof(ok),0);
  int size=0;
  do
  {
		size=recv(sd,fileBuffer,sizeof(fileBuffer),0);
		outFile.write(fileBuffer,size);
		filelen-=size;
	}while(size!=0 && filelen>0);
    cout<<"Transfer finished!\n";
    system("pause");
    outFile.close();
  }
}

void *ThreadMain(void *threadArgs)//收別的cli的訊息
{
    SOCKET sd;
    char str[MAXLINE];
    int n;

    sd=((struct ThreadArgs *)threadArgs)->sd;
    while(1)
    {
        n=recv(sd,str,MAXLINE,0);
        if(n<=0)
            break;
        cout<<str<<endl;
    }
    closesocket(sd); //關閉TCP socket
    WSACleanup();  // 結束 WinSock DLL 的使用
    return NULL;
}
void *ThreadBr(void *threadArgs)//收廣播訊息
{
    SOCKET sd1;
    char str1[1024];struct sockaddr_in serv1;int serv_len;
    sd1=((struct ThreadArgs *)threadArgs)->sd1;
    serv1=((struct ThreadArgs *)threadArgs)->serv1;
    serv_len=((struct ThreadArgs *)threadArgs)->serv_len;

    while(1){

        recvfrom(sd1, str1, MAXLINE, 0,(LPSOCKADDR) &serv1,&serv_len ); //由echo server接收
        if(strlen(str1)>0)
        printf("broadcast:%s\n",str1);

    }
    closesocket(sd1); //關閉 socket
    WSACleanup();  // 結束 WinSock DLL 的使用
}
