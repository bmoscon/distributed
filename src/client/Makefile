all: spawn client



spawn: spawn.cc
	g++ -Wall -ggdb3 -o spawn  -I../lib spawn.cc -L../lib -ldistribute -std=c++11

client: client_main.cc
	g++ -Wall -ggdb3 -o client  -I../lib -Ihandlers/ client_main.cc -L../lib -ldistribute -std=c++11


clean:
	rm -f spawn client
