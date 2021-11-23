#include "ack.h"
// FOUR THINGS: timing, keyboard input, bitmap buffer and sound buffer

// ReserveMemory()

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender) {
	// TODO(gthi): assert

	// SDL_assert(sizeof(state) <= memory->PermanentStorageSize);

	if (!memory->IsInitialized) {
		memory->IsInitialized = true;
	}

	buffer->color = 0xfe;
	if (input->key[K_UP]) {
	} else if (input->key[K_DOWN]) {
	}
}
