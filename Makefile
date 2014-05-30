CFLAGS = $(shell pkg-config --cflags opencv)
LIBS = $(shell pkg-config --libs opencv)

CC=g++

all: funciones.h logica.cpp
	$(CC) funciones.h logica.cpp -o brain $(CFLAGS) $(LIBS)

despertar:
	./brain

clean:
	${RM} brain *.o *~