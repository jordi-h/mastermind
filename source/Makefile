include ../Makefile.compilation

## Rules

all: $(EXEC)

mastermind: $(OBJECTS)
	$(LD) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(GTKFLAGS)

main_mastermind.o: main_mastermind.c
	$(CC) -c main_mastermind.c -o main_mastermind.o $(CFLAGS) $(GTKFLAGS)

controleur_mastermind.o: controleur_mastermind.c
	$(CC) -c controleur_mastermind.c -o controleur_mastermind.o $(CFLAGS) $(GTKFLAGS)

modele_mastermind.o: modele_mastermind.c
	$(CC) -c modele_mastermind.c -o modele_mastermind.o $(CFLAGS) $(GTKFLAGS)

vue_mastermind.o: vue_mastermind.c
	$(CC) -c vue_mastermind.c -o vue_mastermind.o $(CFLAGS) $(GTKFLAGS)

clean:
	rm -f *.o $(EXEC) *~