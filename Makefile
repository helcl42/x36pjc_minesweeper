CC     = g++ -Wall -pedantic -Wno-long-long
CFLAGS = `pkg-config gtkmm-2.4 --cflags`
LIBS   = `pkg-config gtkmm-2.4 --libs`
OBJECTS = Bombs.o OneButton.o Game.o Hiscores.o main.o

#clean: 
#	rm mines *.o

mines: $(OBJECTS)
	$(CC) $(CSFLAGS) $(LIBS) $(OBJECTS) -o mines

Bombs.o: Bombs.cpp Hiscores.h OneButton.h
	$(CC) $(CFLAGS) -o Bombs.o -c Bombs.cpp

OneButton.o: OneButton.cpp OneButton.h
	$(CC) $(CFLAGS) -o OneButton.o -c OneButton.cpp

Game.o: Game.cpp OneButton.h Hiscores.h
	$(CC) $(CFLAGS) -o Game.o -c Game.cpp 

Hiscores.o: Hiscores.cpp Hiscores.h
	$(CC) $(CFLAGS) -o Hiscores.o -c Hiscores.cpp

main.o: main.cpp Bombs.h
	$(CC) $(CFLAGS) -o main.o -c main.cpp

