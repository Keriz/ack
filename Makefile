CC = zig cc
OBJS = src/ack.cc
SDL2LIB = SDL2/x86_64-w64-mingw32/lib/libSDL2main.a SDL2/x86_64-w64-mingw32/lib/libSDL2.dll.a
LIBS += $(SDL2LIB)
SDL2=SDL2/x86_64-w64-mingw32/include
EXE = game.exe

all: dll
	zig cc -MD -MFgame.o -g -I$(SDL2) $(LIBS) src/sdl_ack.cc -o $(EXE) 

dll:
	zig cc -MD -MFgamedll.o -dynamic src/ack.cc -o game.dll

#	./game.exe