make:
	x86_64-w64-mingw32-gcc main.c -Wall -ffunction-sections -O2 -o noimports.exe -nostdlib -fno-ident -Wl,-eWinMain,-subsystem,windows,--no-seh
