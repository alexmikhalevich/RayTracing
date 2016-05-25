CC=g++
CFLAGS=-c -std=c++11 -Wall -g
LFLAGS=-lSDL2

all: rt

rt: main.o csphere.o ctriangle.o cquadrangle.o cscene.o ckdtreecpu.o cparser.o crenderer.o
	$(CC) $(LFLAGS) main.o csphere.o ctriangle.o cquadrangle.o cscene.o ckdtreecpu.o cparser.o crenderer.o -o rt
main.o: main.cpp iobject3d.h geometry.hpp ccamera.hpp ikdtree.h iparser.h clighter.hpp cexception.hpp
	$(CC) $(CFLAGS) main.cpp iobject3d.h geometry.hpp ccamera.hpp ikdtree.h iparser.h clighter.hpp cexception.hpp
csphere.o: csphere.h csphere.cpp
	$(CC) $(CFLAGS) csphere.h csphere.cpp
ctriangle.o: ctriangle.h ctriangle.cpp
	$(CC) $(CFLAGS) ctriangle.h ctriangle.cpp
cquadrangle.o: cquadrangle.h cquadrangle.cpp
	$(CC) $(CFLAGS) cquadrangle.h cquadrangle.cpp
cscene.o: cscene.h cscene.cpp
	$(CC) $(CFLAGS) cscene.h cscene.cpp
ckdtreecpu.o: ckdtreecpu.h ckdtreecpu.cpp
	$(CC) $(CFLAGS) ckdtreecpu.h ckdtreecpu.cpp
cparser.o: cparser.h cparser.cpp
	$(CC) $(CFLAGS) cparser.h cparser.cpp
crenderer.o: crenderer.h crenderer.cpp
	$(CC) $(CFLAGS) crenderer.h crenderer.cpp
clean:
	rm -rf *.o rt

