# cat Makefile
all: mmm.o rtclock.o main.c
	gcc -Wall -g -o mmm main.c mmm.o rtclock.o -lpthread

mmm.o: mmm.h mmm.c
	gcc -Wall -c mmm.c

rtclock.o: rtclock.h rtclock.c
	gcc -Wall -c rtclock.c

clean:
	rm -f *.o mmm