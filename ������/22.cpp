#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD val1, val2;
	val1 = _ttoi(argv[1]);
	val2 = _ttoi(argv[2]);
	_tprintf(_T("%d + %d = %d \n"), val1, val2, val1+val2);
	_gettchar();
	return 0;
}