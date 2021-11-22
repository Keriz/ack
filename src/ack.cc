#include "ack.h"
//FOUR THINGS: timing, keyboard input, bitmap buffer and sound buffer

//ReserveMemory()

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender) {
	//TODO(gthi): assert

	//SDL_assert(sizeof(state) <= memory->PermanentStorageSize);

	if (!memory->IsInitialized) {
		memory->IsInitialized = true;
	}

	if (input->key[K_UP]) {
		buffer->color--;
	} else if (input->key[K_DOWN]) {
	}
}

//TODO(gthi):PORTABLE CODE!

#ifdef WIN32
#include "windows.h"

int CALLBACK
WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow) {
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL, // handle to DLL module
    DWORD fdwReason,    // reason for calling function
    LPVOID lpReserved)  // reserved
{
	return TRUE;
}

#endif