###########################################################
#Makefile for buildling LibLog and testapp
###########################################################


targets: liblog.so
OBJS := log.o

CCFLAGS = -g  -Wall

liblog.so: log.o
	gcc -shared -Wl,-soname,liblog.so -o liblog.so $(OBJS)
	cp liblog.so /usr/lib/liblog.so
	ldconfig

log.o: log_intf.h 
	gcc -fPIC -c log.c 

clean:
	rm -rf *.o *.so  *.out test

####################TESTAPP###############################h

test: test.o
	gcc -llog -I. -fPIC -o test test.c
	
