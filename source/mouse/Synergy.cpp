// Synergy.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <stdio.h>
#include <windows.h>
#include <tchar.h>


int main(int argc, char **argv)
{
	POINT p;
	int px,py;
	printf("%d\n", GetSystemMetrics(SM_CYVIRTUALSCREEN));

	printf("%d\n",GetSystemMetrics(SM_CYSCREEN));
	// GetSystemMetrics(SM_CXSCREEN) 함수는 현재 주모니터의 너비를 리턴한다
	// GetSystemMetrics(SM_CYSCREEN) 함수는 현재 주모니터의 높이를 리턴한다
	// GetSystemMetrics(SM_CYVIRTUALSCREEN) 서브모니터의 높이를 리턴한다
	// GetSystemMetrics(SM_CXVIRTUALSCREEN) 두너모니터의 너비의 합을 리턴한다
	// GetSystemMetrics(SM_CMONITORS) 연결된 모니터 정보를 가져온다. 듀얼지원안되면0 모니터하나면1 다중이면2
	
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
	return 0;
	
}