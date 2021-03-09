#include "mrin.h"
#include <stdio.h>


LONG mRIN::X = 0;
LONG mRIN::Y = 0;
HWND mRIN::ihWnd = NULL;
BOOL mRIN::registred = FALSE;
tSetCursorPos mRIN::oSetCursorPos = NULL;
tGetCursorPos mRIN::oGetCursorPos = NULL;

unsigned int mRIN::PollInput() {
	MSG msg;
	while (GetMessage(&msg, mRIN::ihWnd, 0, 0) != 0)
		DispatchMessage(&msg);
	return msg.message;
}

LRESULT __stdcall mRIN::wpInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_INPUT: {
		UINT uiSize = RAWPTRSIZE;
		static unsigned char lpb[RAWPTRSIZE];
		RAWINPUT* rwInput;
		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &uiSize, RAWINPUTHDRSIZE) != -1)
		{
			rwInput = (RAWINPUT*)lpb;
			if (!rwInput->header.dwType)
			{
				mRIN::X = 0;
				mRIN::Y += rwInput->data.mouse.lLastY;
			}
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProcA(hWnd, message, wParam, lParam);
	}
}

BOOL mRIN::Activate() {
	WNDCLASSEXA wcexa;
	memset(&wcexa, 0, sizeof(wcexa));
	wcexa.cbSize = sizeof(WNDCLASSEX);
	wcexa.lpfnWndProc = (WNDPROC)(mRIN::wpInput);
	wcexa.lpszClassName = "mRIN";
	if (!RegisterClassExA(&wcexa)) {
		return FALSE;
	}
	mRIN::ihWnd = CreateWindowExA(NULL, wcexa.lpszClassName, wcexa.lpszClassName, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
	if (!mRIN::ihWnd) {
		return FALSE;
	}
	UnregisterClassA(MRINCLASS, NULL);
	mRIN::X = mRIN::Y = 0;
	RAWINPUTDEVICE rMouse;
	memset(&rMouse, 0, sizeof(RAWINPUTDEVICE));
	rMouse.dwFlags = 0;
	rMouse.hwndTarget = mRIN::ihWnd;
	rMouse.usUsage = 0x02;
	rMouse.usUsagePage = 0x01;
	if (!RegisterRawInputDevices(&rMouse, 1, sizeof(RAWINPUTDEVICE))) {
		return FALSE;
	}
	if (MH_Initialize() != MH_OK)
		return FALSE;
	if (MH_CreateHook(SetCursorPos, mRIN::hkSetCursorPos, (LPVOID*)&mRIN::oSetCursorPos) != MH_OK
		|| MH_CreateHook(GetCursorPos, mRIN::hkGetCursorPos, (LPVOID*)&mRIN::oGetCursorPos) != MH_OK)
		return FALSE;
	return TRUE;
}

BOOL mRIN::Deactivate() {
	if (mRIN::registred) {
		RAWINPUTDEVICE rMouse;
		memset(&rMouse, 0, sizeof(RAWINPUTDEVICE));
		rMouse.dwFlags |= RIDEV_REMOVE;
		rMouse.hwndTarget = NULL;
		rMouse.usUsagePage = 0x01; 
		rMouse.usUsage = 0x02;
		RegisterRawInputDevices(&rMouse, 1, sizeof(RAWINPUTDEVICE));
		DestroyWindow(mRIN::ihWnd);
		if (MH_RemoveHook(SetCursorPos) != MH_OK || MH_RemoveHook(GetCursorPos))
			return FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL __stdcall mRIN::hkSetCursorPos(int x, int y) {
	//if (!mRIN::oSetCursorPos(x, y)) return 1;
	mRIN::X = x;
	mRIN::Y = y;
	return TRUE;
}

BOOL __stdcall mRIN::hkGetCursorPos(LPPOINT lpPoint) {
	lpPoint->x = mRIN::X;
	lpPoint->y = mRIN::Y;
	return TRUE;
}

mRIN RInput;