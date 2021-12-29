build:
	gcc libtun.c -c -o libtun.o
	gcc alpha.c *.o -o alphad
	chmod +x alphad

clean:
	rmdir alphad
	rmdir *.o
