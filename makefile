# makefile

all: dataserver client #simpleclient

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -std=c++11 -w -c -g reqchannel.cpp

threadManager.o: threadManager.h threadManager.cpp reqchannel.o
	g++ -std=c++11 -c -g threadManager.cpp reqchannel.o -pthread

dataserver: dataserver.cpp reqchannel.o
	g++ -std=c++11 -w -g -o dataserver dataserver.cpp reqchannel.o -lpthread

simpleclient: simpleclient.cpp reqchannel.o
	g++ -std=c++11 -g -o simpleclient simpleclient.cpp reqchannel.o

client: client.cpp threadManager.o reqchannel.o
	g++ -std=c++11 -w -g -o client client.cpp threadManager.o reqchannel.o -pthread

clean:
	rm *o dataserver client simpleclient
