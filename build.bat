@echo off

set CurrTime=%DATE:~6,4%%DATE:~3,2%%DATE:~0,2%__%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%

set SDLLib=SDL2VC/lib/x64/SDL2.lib SDL2VC/lib/x64/SDL2main.lib 
set SDLIncl=SDL2VC/include
set CompilerFlags=-MT -WX -W4 -Wno-unused-parameter -Wno-unused-variable -GR -Oi -Zi -Wno-writable-strings
set LinkerFlags=-incremental:no -opt:ref

del *.pdb > NUL 2> NUL
clang-cl %CompilerFlags% src/ack.cc /LD /Fegame /Fd /link %LinkerFlags% /PDB:game_%CurrTime%.pdb /EXPORT:GameUpdateAndRender 
clang-cl %CompilerFlags% /Fegame.exe /I %SDLIncl% src/sdl_ack.cc /link %LinkerFlags% %SDLLib% shell32.lib /SUBSYSTEM:CONSOLE