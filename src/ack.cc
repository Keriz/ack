#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>

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

#define K_UP (SDL_SCANCODE_UP)
#define K_DOWN (SDL_SCANCODE_DOWN)
#define K_LEFT (SDL_SCANCODE_LEFT)
#define K_RIGHT (SDL_SCANCODE_RIGHT)

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

struct game_input {
	uint32 key[512];
	uint32 key_state[512];
};

game_input input = {};

struct game_memory {
	bool32 IsInitialized;
	uint64 PermanentStorageSize;
	void *PermanentStorage;
	uint64 TransientStorageSize;
	void *TransientStorage;
};

//FOUR THINGS: timing, keyboard input, bitmap buffer and sound buffer
void GameUpdateAndRender(game_memory *memory, game_input *input, SDL_Renderer *renderer, game_state *state);

global_variable uint64 GlobalPerCountFrequency;

inline real32
GetSecondsElapsed(uint64 Start, uint64 End) {
	return (real32)(Start - End) / (real32)GlobalPerCountFrequency;
}

int main(int argc, char *args[]) {
	//The window we'll be rendering to
	SDL_Window *window       = NULL;
	bool32 windowInitialized = 0;
	SDL_Renderer *renderer   = NULL;

	//TODO(gthi): how to query this on Windows?
	int MonitorRefreshHz         = 60;
	int GameUpdateHz             = MonitorRefreshHz / 2;
	real32 TargetSecondsPerFrame = 1.0 / (real32)GameUpdateHz;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		//Create window
		window = SDL_CreateWindow("Ack", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		} else {
			windowInitialized = 1;
			renderer          = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
			if (renderer == NULL) {
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			}

		} //TODO: Handle wrong initialization properly.
	}

	if (windowInitialized) {
		bool32 gameRunning = 1;

		GlobalPerCountFrequency = SDL_GetPerformanceFrequency();
		uint64 LastCounter      = SDL_GetPerformanceCounter();
		uint64 LastCycleCount   = _rdtsc();

		game_memory memory          = {};
		memory.IsInitialized        = 0;
		memory.PermanentStorageSize = Megabytes(64);
		memory.TransientStorageSize = Terabytes((uint64)4);

		game_state state = {};

		while (gameRunning) {
			//TODO(gthi): handle input
			SDL_Event event;

			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_WINDOWEVENT) {
					switch (event.window.event) {
						case SDL_WINDOWEVENT_CLOSE:
							SDL_Log("Window %d closed", event.window.windowID);
							gameRunning = 0;
							break;
					}
				} else if (event.type == SDL_KEYDOWN) {
					//TODO(gthi): change to SDLK_Keys
					input.key[event.key.keysym.scancode]       = SDL_KEYDOWN;
					input.key_state[event.key.keysym.scancode] = event.key.state;
				} else if (event.type == SDL_KEYUP) {
					input.key[event.key.keysym.scancode]       = SDL_KEYUP;
					input.key_state[event.key.keysym.scancode] = event.key.state;
				}
			}

			//handle sound

			uint64 WorkCounter = SDL_GetPerformanceCounter();
			uint64 EndCycles   = _rdtsc();

			uint64 CounterElapsed = WorkCounter - LastCounter;
			LastCounter           = WorkCounter;
			real64 MSPerFrame     = ((1000 * CounterElapsed) / GlobalPerCountFrequency);
			real64 FPS            = (real64)GlobalPerCountFrequency / (real64)CounterElapsed;

			uint64 CyclesElapsed = EndCycles - LastCycleCount;
			real64 MCPF          = ((real64)CyclesElapsed / (1000.f * 1000.f));

			real32 WorksSecondsElapsed    = GetSecondsElapsed(LastCounter, WorkCounter);
			real32 SecondsElapsedForFrame = WorksSecondsElapsed;

			if (SecondsElapsedForFrame < TargetSecondsPerFrame) {
				while (SecondsElapsedForFrame < TargetSecondsPerFrame) {
					uint32 SleepMS = (uint32)(1000.f) * (TargetSecondsPerFrame - SecondsElapsedForFrame);

					if (SleepMS > 0)
						SDL_Delay(SleepMS);

					SecondsElapsedForFrame = GetSecondsElapsed(LastCounter, SDL_GetPerformanceCounter());
				}
			} else {
				//TODO(gthi):MISSED FRAME!
				//TODO(gthi): log the missed frame
			}
			GameUpdateAndRender(&memory, &input, renderer, &state);

			//TODO(gthi): display the value
			char Buffer[256];
			sprintf(Buffer, "MSPerFrame %.02f, FPS %ff/s, CS %.02fmc\n", MSPerFrame, FPS, MCPF);
			SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "%s", Buffer);
		}

		//TODO(gthi): Deinitialize memory mapping
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	atexit(SDL_Quit);

	return 0;
}

//ReserveMemory()

void GameUpdateAndRender(game_memory *memory, game_input *input, SDL_Renderer *renderer, game_state *state) {
	SDL_assert(sizeof(state) <= memory->PermanentStorageSize);

	if (!memory->IsInitialized) {
		memory->IsInitialized = true;
	}

	if (input->key[K_UP]) {
	} else if (input->key[K_DOWN]) {
	}

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0x2, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x0, SDL_ALPHA_OPAQUE);

	SDL_RenderPresent(renderer);
}
