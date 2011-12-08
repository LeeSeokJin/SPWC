#include <stdio.h>
#include <windows.h>
#include <tchar.h>

int MouseClickTest()
{
	INPUT_RECORD Inrec; //마우스나 키보드 이벤트를 저장한 구조체
	DWORD evRead;
	HANDLE hStdIn=NULL;
	DWORD dwMode;
	int  nClickNum=0;
	 
	GetConsoleMode(hStdIn, &dwMode);
	 
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hStdIn, &dwMode);
	if(!(dwMode & ENABLE_MOUSE_INPUT))
	{
		if(TRUE != SetConsoleMode( hStdIn, dwMode | ENABLE_MOUSE_INPUT))
		{
			return 0;
		}
	}

	PeekConsoleInput(hStdIn, &Inrec, 1, &evRead);
  
	if( evRead>0) 
	{
		ReadConsoleInput(hStdIn, &Inrec, 1, &evRead);
		switch (Inrec.EventType )
		{
			case MOUSE_EVENT:
			{
				//FROM_LEFT_1ST_BUTTON_PRESSED 이건1
				//FROM_LEFT_2ND_BUTTON_PRESSED 이건2
				if(FROM_LEFT_1ST_BUTTON_PRESSED & Inrec.Event.MouseEvent.dwButtonState) //왼쪽클릭 입력처리
					{
						nClickNum |= 0x01;
					}
				if(FROM_LEFT_2ND_BUTTON_PRESSED & Inrec.Event.MouseEvent.dwButtonState) // 휠 클릭 입력처리
					{
						nClickNum |= 0x04;
					}
				if(RIGHTMOST_BUTTON_PRESSED & Inrec.Event.MouseEvent.dwButtonState) // 오른쪽 클릭 입력처리
					{
						nClickNum |= 0x02;
					}
	
				break; 
			}
			case KEY_EVENT:
			{
				_tprintf(_T("No key\n"));
				break;
			}
		}
	}
	
  //gotoxy(0, 0);
  //printf("%d,%d",nClickNum&0x01,  nClickNum&0x02);
	return nClickNum;
}


int _tmain(int argc, TCHAR* argv[])
{
	POINT p;
	DWORD px,py;
	DWORD temp;
	_tprintf(_T("%d\n"), GetSystemMetrics(SM_CYVIRTUALSCREEN));
	_tprintf(_T("%d\n"),GetSystemMetrics(SM_CYSCREEN));
	// GetSystemMetrics(SM_CXSCREEN) 함수는 현재 주모니터의 너비를 리턴한다
	// GetSystemMetrics(SM_CYSCREEN) 함수는 현재 주모니터의 높이를 리턴한다
	// GetSystemMetrics(SM_CYVIRTUALSCREEN) 서브모니터의 높이를 리턴한다
	// GetSystemMetrics(SM_CXVIRTUALSCREEN) 두너모니터의 너비의 합을 리턴한다
	// GetSystemMetrics(SM_CMONITORS) 연결된 모니터 정보를 가져온다. 듀얼지원안되면0 모니터하나면1 다중이면2

/*
	while(2!=MouseClickTest()) //우클릭을하면 탈출
	{
		_tprintf(_T("right click!\n"));
		Sleep(25);
	}*/
	while(1){ // 휠클릭하면 탈출
		GetCursorPos(&p);	//마우스 위치값얻어오기
		//printf("%d, %d \n", p.x, p.y); //마우스위치출력
		px=p.x;
		py=p.y;
		temp = MouseClickTest();
		if(1==temp)
			_tprintf(_T("click!\n"));
		else if(2==temp)
			_tprintf(_T("right click!\n"));
		else if(3==temp)
			break;
		else if(4==temp)
			_tprintf(_T("wheel click!\n"));

		if(px==(GetSystemMetrics(SM_CXVIRTUALSCREEN)-1))
			SetCursorPos(1,py); //마우스 위치를 이동시키기
		
		if(px==0)
			SetCursorPos(GetSystemMetrics(SM_CXVIRTUALSCREEN)-2,py); //마우스 위치를 이동시키기

		//마우스 위쪽 -> 아래쪽, 아래쪽 -> 위쪽
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