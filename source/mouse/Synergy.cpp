// Synergy.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.

#include <stdio.h>
#include <windows.h>
#include <tchar.h>


//마우스 클릭 이벤트 함수.
//마우스 이벤트에 해당하는 값을 리턴함
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
					nClickNum |=0x01;
				}
				if(FROM_LEFT_2ND_BUTTON_PRESSED & Inrec.Event.MouseEvent.dwButtonState) // 휠 클릭 입력처리
				{
					nClickNum |=0x04;
				}
				if(RIGHTMOST_BUTTON_PRESSED & Inrec.Event.MouseEvent.dwButtonState) // 오른쪽 클릭 입력처리
				{
					nClickNum|=0x02;
				}
				if(DOUBLE_CLICK & Inrec.Event.MouseEvent.dwEventFlags) //
				{
					nClickNum = 0x08;
					_tprintf(_T("%d\n"),nClickNum);
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


//마우스 커서를 보내는 쓰레드
//이 쓰레드는 마우스를 맨 왼쪽으로 보내면 종료
DWORD WINAPI ThreadProc(LPVOID lpParam){ 
	POINT p;
	DWORD px=1, py=0;
	
	while(px!=0)
	{
		GetCursorPos(&p);
		px=p.x;
		py=p.y;	

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

		Sleep(100); //0.1초마다 커서의 좌표를 얻어옴
	}
	return px;
}

//마우스 클릭관련 쓰레드
//이 쓰레드는 우클릭과 좌클릭을 동시에하면 종료
DWORD WINAPI ThreadProc1(LPVOID lpParam){ 
	DWORD temp;

	while(1)
	{
		temp = MouseClickTest();
		if(1==temp)
			_tprintf(_T("click!\n"));
		else if(2==temp)
			_tprintf(_T("right click!\n"));
		else if(3==temp)
			return 0;
		else if(4==temp)
			_tprintf(_T("wheel click!\n"));
		else if((8&temp)==8)
			_tprintf(_T("double click!\n"));
		Sleep(100);
	}
	return 0;
}


int _tmain(int argc, TCHAR* argv[])
{
//	POINT p;
//	DWORD px,py; 
//	DWORD temp; //이상 마우스 관련 변수

	HANDLE tHandle[2];//이상 쓰레드 관련변수
	DWORD threadId[2];
	DWORD counter=0, tmp=0;

	_tprintf(_T("%d\n"), GetSystemMetrics(SM_CYVIRTUALSCREEN));
	_tprintf(_T("%d\n"),GetSystemMetrics(SM_CYSCREEN));
	// GetSystemMetrics(SM_CXSCREEN) 함수는 현재 주모니터의 너비를 리턴한다
	// GetSystemMetrics(SM_CYSCREEN) 함수는 현재 주모니터의 높이를 리턴한다
	// GetSystemMetrics(SM_CYVIRTUALSCREEN) 서브모니터의 높이를 리턴한다
	// GetSystemMetrics(SM_CXVIRTUALSCREEN) 두너모니터의 너비의 합을 리턴한다
	// GetSystemMetrics(SM_CMONITORS) 연결된 모니터 정보를 가져온다. 듀얼지원안되면0 모니터하나면1 다중이면2

	//쓰레드 생성
	tHandle[0] = CreateThread( NULL, 0, ThreadProc, NULL, 0, &threadId[0]);
	tHandle[1] = CreateThread( NULL, 0, ThreadProc1, NULL, 0, &threadId[1]); 
	
	//쓰레드 검사
	if(tHandle[0] == NULL || tHandle[1] == NULL){
		_tprintf(_T("thread creation fault!") );
		return -1;
	}

	//2개의 쓰레드가 signaled 상태 될때까지 기다림
	WaitForMultipleObjects(2, tHandle, TRUE, INFINITE);


	//쓰레드의 반환을 받음
	GetExitCodeThread(tHandle[0], &tmp);
	counter += tmp;
	GetExitCodeThread(tHandle[1], &tmp);
	counter += tmp;
	
	_tprintf(_T("total count : %d"), counter);
	/*
	while(1){ 

		
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
		

	}*/

	CloseHandle(tHandle[0]);
	CloseHandle(tHandle[1]);

	return 0;
}
	


/*
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD); 
VOID MouseEventProc(MOUSE_EVENT_RECORD); 
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD); 
 
int _tmain(int argc, TCHAR* argv[])
{ 
    HANDLE hStdin; 
    DWORD cNumRead, fdwMode, fdwSaveOldMode, i; 
    INPUT_RECORD irInBuf[128]; 


    int counter=0;
 
    // Get the standard input handle. 
 
    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    if (hStdin == INVALID_HANDLE_VALUE) 
        ErrorExit("GetStdHandle"); 
 
    // Save the current input mode, to be restored on exit. 
 
    if (! GetConsoleMode(hStdin, &fdwSaveOldMode) ) 
        ErrorExit("GetConsoleMode"); 
 
    // Enable the window and mouse input events. 
 
    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT; 
    if (! SetConsoleMode(hStdin, fdwMode) ) 
        ErrorExit("SetConsoleMode"); 
 
    // Loop to read and handle the input events. 
 
    while(1) 
    { 
        // Wait for the events. 
 
        if (! ReadConsoleInput( 
                hStdin,      // input buffer handle 
                irInBuf,     // buffer to read into 
                128,         // size of read buffer 
                &cNumRead) ) // number of records read 
            ErrorExit("ReadConsoleInput"); 
 
        // Dispatch the events to the appropriate handler. 
 
        for (i = 0; i < cNumRead; i++) 
        {

            switch(irInBuf[i].EventType) 
            { 
                case KEY_EVENT: // keyboard input 
                    KeyEventProc(irInBuf[i].Event.KeyEvent); 
                    break; 
 
                case MOUSE_EVENT: // mouse input 
                    MouseEventProc(irInBuf[i].Event.MouseEvent); 
                    break; 
 
                case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
                    ResizeEventProc( 
                        irInBuf[i].Event.WindowBufferSizeEvent); 
                    break; 
 
                case FOCUS_EVENT:  // disregard focus events 
 
                case MENU_EVENT:   // disregard menu events 
                    break; 
 
                default: 
                    ErrorExit("Unknown event type"); 
                    break; 
            } 
        }
		counter++;

    } 
 
    return 0; 
}

VOID ErrorExit (LPSTR lpszMessage) 
{ 
    fprintf(stderr, "%s\n", lpszMessage); 
    ExitProcess(0); 
}

VOID KeyEventProc(KEY_EVENT_RECORD ker)
{
    printf("Key event: ");

    if(ker.bKeyDown)
        printf("key pressed\n");
    else printf("key released\n");
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	

	
    printf("Mouse event: ");
    
    switch(mer.dwEventFlags)
    {
	case 0:
            printf("button press\n");
            break;
        case DOUBLE_CLICK:
            printf("double click\n");
            break;
        case MOUSE_HWHEELED:
            printf("horizontal mouse wheel\n");
            break;
        case MOUSE_MOVED:
            printf("mouse moved\n");



            break;
        case MOUSE_WHEELED:
            printf("vertical mouse wheel\n");
            break;
        default:
            printf("unknown\n");
            break;
    }
}

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
    printf("Resize event\n");
}
	*/