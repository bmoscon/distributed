all: libdistribute


libdistribute: message.cc logger.cc connection.cc util.cc
	g++ -Wall -c message.cc logger.cc connection.cc util.cc -std=c++11
	ar -cvq libdistribute.a message.o logger.o connection.o util.o


clean:
	rm -f *.o *.a
