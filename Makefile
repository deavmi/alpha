build:
	curl http://deavmi.assigned.network/git/deavmi/libtun/raw/branch/master/source/libtun/tunctl.c > libtun.c
	gcc libtun.c -c  -o libtun.o
	gcc config.c -c -o config.o
	gcc alpha.c *.o -ljansson -o alphad
	# TODO: sort/figure-out out -ljansson
	chmod +x alphad

clean:
	rm alphad
	rm *.o
