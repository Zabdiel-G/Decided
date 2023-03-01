CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids

asteroids: asteroids.cpp mangeles.cpp ptakkar.cpp sarboleda.cpp zgarcia.cpp log.cpp timers.cpp
	g++ $(CFLAGS) asteroids.cpp mangeles.cpp ptakkar.cpp sarboleda.cpp zgarcia.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

