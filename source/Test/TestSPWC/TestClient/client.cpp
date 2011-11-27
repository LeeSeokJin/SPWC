#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
void ErrorHandling(char* message);
void Mousecontrol(POINT p);
void MouseControl2(POINT p);

void MouseControl(POINT p)
{
	int px,py;
	SetCursorPos(p.x,p.y); //마우스 위치를 이동시키기
	while(1){
		GetCursorPos(&p);	//마우스 위치값얻어오기
		//printf("%d, %d \n", p.x, p.y); //마우스위치출력
		px=p.x;
		py=p.y;

		/*마우스 왼쪽 -> 오른쪽, 오른쪽 -> 왼쪽*/
		if(px==(GetSystemMetrics(SM_CXVIRTUALSCREEN)-1))
			SetCursorPos(1,py); //마우스 위치를 이동시키기
		if(px==0)
			SetCursorPos(GetSystemMetrics(SM_CXVIRTUALSCREEN)-2,py); //마우스 위치를 이동시키기

		/*마우스 위쪽 -> 아래쪽, 아래쪽 -> 위쪽*/
		if(GetSystemMetrics(SM_CXVIRTUALSCREEN)-GetSystemMetrics(SM_CXSCREEN) > px){
			if(py==(GetSystemMetrics(SM_CYSCREEN)-1))
				SetCursorPos(px,1); //마우스 위치를 이동시키기
			if(py==0)
				SetCursorPos(px,GetSystemMetrics(SM_CYSCREEN)-2); //마우스 위치를 이동시키기
		}else{
			if(py==(GetSystemMetrics(SM_CYVIRTUALSCREEN)-1))
				SetCursorPos(px,1); //마우스 위치를 이동시키기
			if(py==0)
				SetCursorPos(px,GetSystemMetrics(SM_CYVIRTUALSCREEN)-2); //마우스 위치를 이동시키기
		}
	}
}

void MouseControl2(POINT p)
{
	SetCursorPos(p.x,p.y); //마우스 위치를 이동시키기

}
int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	char message[30];
	int strLen;
	if(argc!=3)
	{
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)	//소켓 라이브러리를 초기화
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);	//소켓 생성
	if(hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR) //생성된 소켓을 바탕으로 서버에 연결 요청
		ErrorHandling("connect() error");
	
	while(1)
	{
		POINT pon;
		strLen = recv(hSocket, message, sizeof(pon), 0);	//recv함수 호출을 통해서 서버로부터 전송되는 데이터를 수
				
		memcpy(&pon,message,sizeof(POINT));
		if(strLen == -1){
			ErrorHandling("read() error!");
			break;
		}
		printf("Message from server: %d, %d  \n", pon.x,pon.y);
		MouseControl2(pon);
		if(pon.x == 0 && pon.y ==0) break;
	}
	closesocket(hSocket);
	WSACleanup();	//소켓 라이브러리를 해제
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

