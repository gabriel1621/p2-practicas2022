OPTS=-g -Wall -std=c++11
OBJS=prac3.o Player.o Ship.o Coordinate.o Util.o
CC=g++

prac3: $(OBJS)
	$(CC) $(OPTS) $(OBJS) -o prac3

prac3.o: prac3.cc Player.h Ship.h Coordinate.h Util.h
	$(CC) $(OPTS) -c prac3.cc

Player.o: Player.cc Player.h Ship.h Coordinate.h Util.h
	$(CC) $(OPTS) -c Player.cc

Ship.o: Ship.cc Ship.h Coordinate.h Util.h
	$(CC) $(OPTS) -c Ship.cc

Util.o: Util.cc Util.h
	$(CC) $(OPTS) -c Util.cc

Coordinate.o: Coordinate.cc Coordinate.h Util.h
	$(CC) $(OPTS) -c Coordinate.cc

clean:
	rm $(OBJS)
