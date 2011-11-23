// Synergy.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <stdio.h>
#include <windows.h>
#include <tchar.h>


int main(int argc, char **argv)
{
	POINT p;
	int px,py;
	// GetSystemMetrics(SM_CXSCREEN) 함수는 현재 모니터 정보를 읽어온다
	// GetSystemMetrics(SM_CMONITORS) 연결된 모니터 정보를 가져온다. 듀얼지원안되면0 모니터하나면1 다중이면2
	
	while(1){
		GetCursorPos(&p);	//마우스 위치값얻어오기
		//printf("%d, %d \n", p.x, p.y); //마우스위치출력
		px=p.x;
		if(px==(GetSystemMetrics(SM_CXVIRTUALSCREEN)-1))
			SetCursorPos(1,p.y); //마우스 위치를 이동시키기
		if(px==0)
			SetCursorPos(GetSystemMetrics(SM_CXVIRTUALSCREEN)-2,p.y); //마우스 위치를 이동시키기


		py=p.y;
		
	}
	return 0;
	
}