all: main.cc server.cc
	g++ -Wall -o server -I../lib -Ihandlers/ main.cc server.cc -L../lib -ldistribute -lpthread -std=c++11


clean:
	rm -f server
