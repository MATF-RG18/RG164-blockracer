PROGRAM			= main
CC				= g++

ifeq '$(findstring ;,$(PATH))' ';'
	detected_OS := Windows
else
	detected_OS := $(shell uname 2>/dev/null || echo Unknown)
	detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
	detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
	detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

ifeq ($(detected_OS),Linux)
	CFLAGS		= -g -Wall -Wextra -I/usr/X11R6/include -I/usr/pkg/include 
	LDFLAGS		= -L/usr/X11R6/lib -L/usr/pkg/lib
endif
ifeq ($(detected_OS),Darwin)
	CFLAGS		= -g -Wall -Wextra 
	LDFLAGS		= -framework OpenGL -framework GLUT -lglut
endif

$(PROGRAM): main.o vehicle.o grid.o block.o
	$(CC) $(CFLAGS) -o $(PROGRAM) *.o $(LDFLAGS)

vehicle.o: Vehicle.cpp Vehicle.h
	$(CC) $(CFLAGS) -c Vehicle.cpp $(LDFLAGS)

grid.o: Grid.cpp Grid.h
	$(CC) $(CFLAGS) -c Grid.cpp $(LDFLAGS)

block.o: Block.cpp Block.h
	$(CC) $(CFLAGS) -c Block.cpp $(LDFLAGS)


.PHONY: beauty clean dist

beauty:
	-indent -kr -nut $(PROGRAM).c
	-rm *~ *BAK

clean:
	-rm *.o $(PROGRAM)

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
