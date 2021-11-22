CC = zig cc
OBJS = src/ack.cc
#SDL2LIB = SDL2/x86_64-w64-mingw32/lib/libSDL2main.a SDL2/x86_64-w64-mingw32/lib/libSDL2.dll.a
SDL2L=SDL2VC/lib/x64/SDL2.lib SDL2VC/lib/x64/SDL2main.lib 
SDL2=SDL2VC/include
LIBS += $(SDL2L)
EXE = game.exe

all: dll
	clang-cl s/Zi /Wall /Fegame /I $(SDL2) src/sdl_ack.cc /link $(LIBS) shell32.lib /SUBSYSTEM:CONSOLE
#	zig cc -MD -MFgame.o -gcodeview -g -I$(SDL2) $(LIBS) src/sdl_ack.cc -o $(EXE)
#	CommonFlags="-DDEBUG -g -Og -Wall -Werror -Wno-write-strings -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function -Wno-sign-compare -Wno-unused-result -Wno-strict-aliasing -Wno-switch -std=gnu++11 -fno-rtti -fno-exceptions -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_SDL=1"

dll:
#	zig cc -MD -MFgamedll.o -dynamic src/ack.cc -o game.dll


#	./game.exe
