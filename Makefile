lib1: lib1.c
	cc -o lib1 lib1.c -l6502

clean:
	rm -f lib1

example:
	xa chars.s -o chars.bin
	run6502 -l 1000 chars.bin -R 1000 -P FFEE -X 0
