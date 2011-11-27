#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <tchar.h>
#include <process.h>

#define MAX_THREADS 1

void ErrorHandling(char* message);

DWORD cntOfThread = 0;

//클라이어트에 할일 정의하는 함수

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
	SOCKET clnt;
	POINT pon;
	clnt = (SOCKET)lpParam;
	long time = GetCurrentTime();
	while(1){
		
		GetCursorPos(&pon);	
		if(time + 10 < GetCurrentTime() ){
			send(clnt, (char*)&pon, sizeof(pon), 0);
			if(pon.x == 0 && pon.y ==0) break;
			time = GetCurrentTime();
		}		
	}
	//MouseControl();
	closesocket(clnt);
	
	return 0;
}


int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadId[MAX_THREADS];		//쓰레드 아이디
	HANDLE hThread[MAX_THREADS];		//쓰레드 핸들
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int portNum = 8080;
	printf("서버프로그램을 시작합니다.\n");
	
	int szClntAddr;
	/*
	if(argc!=2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	*/

	if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)	//소켓 라이브러리를 초기화
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);	//소켓 생성
	if(hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(portNum);

	if(bind(hServSock, (SOCKADDR*) &servAddr, sizeof(servAddr))==SOCKET_ERROR)	//소켓에 IP주소, PORT번호를 할당
		ErrorHandling("bind() error");

	if(listen(hServSock, 5) == SOCKET_ERROR)	//listen 함수 호출을 통해서, 생성한 소켓을 서버 소켓으로 완성
		ErrorHandling("listen() error");
	
	szClntAddr = sizeof(clntAddr);
	while(1)
	{

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);	//클라이언트의 연결 요청을 수락하기 위해서 accept함수를 호출
		if(hClntSock == INVALID_SOCKET)
			ErrorHandling("accept() error");
		hThread[cntOfThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc,(LPVOID) hClntSock,0,(unsigned *) &dwThreadId[cntOfThread]);
		if(hThread[cntOfThread] == NULL)
		{
			_tprintf(_T("쓰레드를 생성할수가 없습니다.\n"));
		}
	}
	
	closesocket(hServSock);
	WSACleanup();	//프로그램을 종료하기전에 초기화한 소켓 라이브러리를 해제
	CloseHandle(hThread[cntOfThread]);
	return 0;
	
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

