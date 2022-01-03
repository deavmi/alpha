build:
	curl http://deavmi.assigned.network/git/deavmi/libtun/raw/branch/master/source/libtun/tunctl.c > libtun.c

	# TODO: Maybe a nicer way to do this non-manually
	gcc libtun.c -c  -o libtun.o
	gcc config.c -c -o config.o
	gcc session.c -c -o session.o

	# Link all toghether
	gcc alpha.c *.o -ljansson -o alphad
	# TODO: sort/figure-out out -ljansson

	# Make the executable, well... executable
	chmod +x alphad

clean:
	rm alphad
	rm *.o
