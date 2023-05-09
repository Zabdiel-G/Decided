CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: decided

decided: Decided.cpp mangeles.cpp ptakkar.cpp sarboleda.cpp zgarcia.cpp log.cpp timers.cpp
	g++ $(CFLAGS) Decided.cpp mangeles.cpp ptakkar.cpp sarboleda.cpp zgarcia.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -odecided

clean:
	rm -f decided
	rm -f *.o
