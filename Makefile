lib1: lib1.c
	cc -o lib1 lib1.c -l6502

clean:
	rm -f lib1

example:
	xa chars.s -o chars.bin
	run6502 -l 1000 chars.bin -R 1000 -P FFEE -X 0

echo:
	xa echo.s -o echo.bin
	run6502 -l 1000 echo.bin -R 1000 -G FFCC -P FFEE -X 0

echolib:
	cc -o bin/echo echo.c -l6502

screen:
	xa screen.s -o screen.bin
	bin2c screen.bin -o screen_bin.h
	cc -o bin/screen screen.c -l6502
