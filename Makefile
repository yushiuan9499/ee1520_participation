
# Makefile for f2022, ecs36b
# https://macappstore.org/libjson-rpc-cpp/
# https://github.com/cinemast/libjson-rpc-cpp

CC = g++ -std=c++17
CFLAGS = -g -I/usr/include/jsoncpp

LDFLAGS = 	-L/opt/homebrew/lib/ -ljsoncpp			\
		-lmicrohttpd -ljsonrpccpp-common		\
		-ljsonrpccpp-server -ljsonrpccpp-client -lcurl

INC	=	ee1520_Common.h

INC_CL	=	JvTime.h
OBJ	=	JvTime.o ee1520_JSON.o ee1520_Exception.o

# rules.
all: 	ee1520server ee1520client

#
#
# <target> : [... tab...] <dependency>
# [... tab ....] <action>

ee1520client.h:		ee1520_participation.json
	jsonrpcstub ee1520_participation.json --cpp-server=ee1520Server --cpp-client=ee1520Client

ee1520server.h:		ee1520_participation.json
	jsonrpcstub ee1520_participation.json --cpp-server=ee1520Server --cpp-client=ee1520Client

ee1520client.o:		ee1520client.cpp ee1520client.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) ee1520client.cpp

ee1520server.o:		ee1520server.cpp ee1520server.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) ee1520server.cpp

ee1520_JSON.o:		ee1520_JSON.cpp $(INC)
	$(CC) -c $(CFLAGS) ee1520_JSON.cpp

ee1520_Exception.o:	ee1520_Exception.cpp $(INC)
	$(CC) -c $(CFLAGS) ee1520_Exception.cpp

JvTime.o:	JvTime.cpp JvTime.h $(INC)
	$(CC) -c $(CFLAGS) JvTime.cpp

ee1520server:	ee1520server.o $(OBJ)
	$(CC) -o ee1520server ee1520server.o $(OBJ) $(LDFLAGS)

ee1520client:	ee1520client.o $(OBJ)
	$(CC) -o ee1520client ee1520client.o $(OBJ) $(LDFLAGS)

clean:
	rm -f *.o *~ core ee1520client.h ee1520server.h ee1520client ee1520server


