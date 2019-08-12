PROGRAM   = main
CC        = g++
CFLAGS_LINUX    = -g -Wall -Wextra -I/usr/X11R6/include -I/usr/pkg/include 
LDFLAGS_LINUX   = -L/usr/X11R6/lib -L/usr/pkg/lib
CFLAGS    = -g -Wall -Wextra 
LDFLAGS_MAC   = 
LDLIBS_MAC   = -framework OpenGL -framework GLUT -lglut

$(PROGRAM): main.o
	$(CC) $(LDFLAGS_MAC) $(CFLAGS) -o $(PROGRAM) main.o $(LDLIBS_MAC)

.PHONY: beauty clean dist

beauty:
	-indent -kr -nut $(PROGRAM).c
	-rm *~ *BAK

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
