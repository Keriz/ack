CC = zig cc
OBJS = src/ack.cc
SDL2LIB = SDL2/x86_64-w64-mingw32/lib/libSDL2main.a SDL2/x86_64-w64-mingw32/lib/libSDL2.dll.a
LIBS += $(SDL2LIB)
SDL2=SDL2/x86_64-w64-mingw32/include
EXE = game.exe

all: 
	$(CC) -I$(SDL2) $(LIBS) $(OBJS) -o $(EXE)
	./game.exe