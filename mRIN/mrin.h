#pragma once
#include <Windows.h>
#include <MinHook.h>

#define MRINCLASS "mRIN"
#define RAWINPUTHDRSIZE sizeof(RAWINPUTHEADER)
#define RAWPTRSIZE 40


typedef BOOL(__stdcall* tSetCursorPos)(int x, int y);
typedef BOOL(__stdcall* tGetCursorPos)(LPPOINT lpPoint);

class mRIN {
private:
	static LONG X;
	static LONG Y;
	static HWND ihWnd;
	static BOOL registred;
public:
	unsigned int PollInput();
	static BOOL Activate();
	static BOOL Deactivate();
	static LRESULT __stdcall wpInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL __stdcall hkSetCursorPos(int x, int y);
	static BOOL __stdcall hkGetCursorPos(LPPOINT lpPoint);
	static tSetCursorPos oSetCursorPos;
	static tGetCursorPos oGetCursorPos;
};

extern mRIN RInput;