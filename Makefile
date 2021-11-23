CC = zig cc
OBJS = src/ack.cc
SDL2L=SDL2VC/lib/x64/SDL2.lib SDL2VC/lib/x64/SDL2main.lib 
SDL2I=SDL2VC/include
LIBS += $(SDL2L)

FLAGS=-MT -WX -W4 -Wno-unused-parameter -Wno-unused-variable -GR -Oi -Zi -Wno-writable-strings
LFLAGS=-incremental:no -opt:ref

DLL=game.dll 
EXE =game.exe

all: dll
	@clang-cl $(FLAGS) /Fegame.exe /I $(SDL2I) src/sdl_ack.cc /link $(LFLAGS) $(LIBS) shell32.lib /SUBSYSTEM:CONSOLE
#	zig cc -MD -MFgame.o -gcodeview -g -I$(SDL2) $(LIBS) src/sdl_ack.cc -o $(EXE)

dll:
	@clang-cl $(FLAGS) src/ack.cc /LD /Fegame /Fd /link /PDB:"game_" /EXPORT:GameUpdateAndRender 
#	$(shell date)


#	zig cc -MD -MFgamedll.o -dynamic src/ack.cc -o game.dll
