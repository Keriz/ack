#include <stdint.h>

#define WIN32 1

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

#define internal static
#define local_persist static
#define global_variable static

typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

typedef uint8_t bool8;
typedef int32 bool32;

typedef float real32;
typedef float real64;

#define false (bool32)0
#define true (bool32)1

#define K_UP (82)
#define K_DOWN (81)
#define K_LEFT (80)
#define K_RIGHT (79)

#define Kilobytes(Value) ((Value)*1024)
#define Megabytes(Value) (Kilobytes(Value) * 1024)
#define Gigabytes(Value) (Megabytes(Value) * 1024)
#define Terabytes(Value) (Gigabytes(Value) * 1024)
/* 
#define Assert(Expression)     \
	if (!(Expression)) {   \
		*(int *)0 = 0; \
	} */

//
struct game_state {
};

struct game_buffer {
	uint8 color;
};

struct game_input {
	uint32 key[512];
	uint32 key_state[512];
};

struct game_memory {
	bool32 IsInitialized;
	uint64 PermanentStorageSize;
	void *PermanentStorage;
	uint64 TransientStorageSize;
	void *TransientStorage;
};

#define GAME_UPDATE_AND_RENDER(name) void name(game_memory *memory, game_input *input, game_buffer *buffer, game_state *state)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

GAME_UPDATE_AND_RENDER(GameUpdateAndRenderStub) {
}

// /void GameUpdateAndRender(game_memory *memory, game_input *input, SDL_Renderer *renderer, game_state *state);
