CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: Decided

asteroids: Decided.cpp mangeles.cpp ptakkar.cpp sarboleda.cpp zgarcia.cpp log.cpp timers.cpp
	g++ $(CFLAGS) Decided.cpp mangeles.cpp ptakkar.cpp sarboleda.cpp zgarcia.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oDecided

clean:
	rm -f Decided
	rm -f *.o

