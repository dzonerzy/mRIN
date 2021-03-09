#include "mrin.h"
#include <stdio.h>


void Init() {
    if (RInput.Activate()) {
        RInput.PollInput();
    }
    else {
        MessageBoxA(0, "Error while enabling raw input", "Error", MB_ICONERROR | MB_OK);
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

/*int main() {
	if (RInput.Activate()) {
		printf("RInput activted!");
		if (!RInput.PollInput())
			printf("Failed to poll input");
	}
	else {
		printf("Unable to activate RInput");
	}
	return getchar();
}*/