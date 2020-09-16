all: deflate inflate

inflate: inflate.o codes.o bitbuffer.o
	gcc inflate.o codes.o bitbuffer.o -o inflate

deflate: deflate.o codes.o bitbuffer.o
	gcc deflate.o codes.o bitbuffer.o -o deflate

inflate.o: inflate.c codes.h bitbuffer.h
	gcc -g -Wall -std=c99 -c inflate.c

deflate.o: deflate.c codes.h bitbuffer.h
	gcc -g -Wall -std=c99 -c deflate.c

codes.o: codes.c codes.h
	gcc -g -Wall -std=c99 -c codes.c

bitbuffer.o: bitbuffer.c bitbuffer.h
	gcc -g -Wall -std=c99 -c bitbuffer.c

clean:
	rm -f inflate.o deflate.o codes.o bitbuffer.o
	rm -f inflate
	rm -f deflate
	rm -f compressed.bin original


