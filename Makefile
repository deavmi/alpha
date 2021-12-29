build:
	curl http://deavmi.assigned.network/git/deavmi/libtun/raw/branch/master/source/libtun/tunctl.c > libtun.c
	gcc libtun.c -c -o libtun.o
	gcc alpha.c *.o -o alphad
	chmod +x alphad

clean:
	rm alphad
	rm *.o
