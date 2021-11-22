
#include "ack.h"
#include <SDL.h>
#include <stdio.h>

#if ACK_DEBUG
#include "windows.h"
#endif

struct game_code {
	void *GameCodeDLL;
	game_update_and_render *UpdateAndRender;
	bool32 IsValid;
};

internal void
UnloadGameCode(game_code *GameCode) {
	//TODO: change DLL to a more cross platform name
	if (GameCode->GameCodeDLL) {
		SDL_UnloadObject(GameCode->GameCodeDLL);
	}

	GameCode->IsValid         = false;
	GameCode->UpdateAndRender = GameUpdateAndRenderStub;
}

internal game_code
LoadGameCode(void) {
	game_code Result;

#if ACK_DEBUG
	CopyFileA((LPCSTR) "game.dll", (LPCSTR) "game_tmp.dll", FALSE);
#endif

	Result.GameCodeDLL = SDL_LoadObject("game_tmp.dll");
	if (Result.GameCodeDLL) {
		Result.UpdateAndRender = (game_update_and_render *)SDL_LoadFunction(Result.GameCodeDLL, "GameUpdateAndRender");
		Result.IsValid         = (Result.UpdateAndRender && 1);
	}
	if (!Result.IsValid) {
		Result.UpdateAndRender = GameUpdateAndRenderStub;
	}
	return Result;
}

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

		game_state state   = {};
		game_buffer buffer = {};
		game_input input   = {};

		buffer.color = 0xff;

		uint32 LoadCounter = 0;

		game_code Game       = {};
		Game.UpdateAndRender = GameUpdateAndRenderStub;

		Game = LoadGameCode();

		while (gameRunning) {
			if (LoadCounter++ > 120) {
				UnloadGameCode(&Game);
				Game        = LoadGameCode();
				LoadCounter = 0;
			}
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
			LastCycleCount       = EndCycles;
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

			Game.UpdateAndRender(&memory, &input, &buffer, &state);

			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, buffer.color, buffer.color, buffer.color, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawLine(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x0, SDL_ALPHA_OPAQUE);

			SDL_RenderPresent(renderer);

			//TODO(gthi): display the value
			char Buffer[256];
			sprintf(Buffer, "MSPerFrame %.02f, FPS %ff/s, CS %.02fmc\n", MSPerFrame, FPS, MCPF);
			OutputDebugStringA(Buffer);
			//SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "%s", Buffer);
		}

		UnloadGameCode(&Game);

		//TODO(gthi): Deinitialize memory mapping
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	atexit(SDL_Quit);

	return 0;
}
