CC = gcc
CFLAGS = -c -Wall
OBJS = main.o graphe.o

.PHONY: run clean zip install

run: main
	./main
	
main: $(OBJS)
	$(CC) -o $@ $(OBJS) -g

main.o: main.c *.h
	$(CC) $(CFLAGS) $<

%.o: %.c %.h
	$(CC) $(CFLAGS) $<


clean: 
	rm -f $(OBJS)
	rm -f main
	ls -l

DOSSIER = BENAYED_DEMANGE_VILLA_AMMAD

zip:
	rm -rf $(DOSSIER)
	rm -f $(DOSSIER).zip
	mkdir $(DOSSIER)
	cp *.c *.h Makefile $(DOSSIER)
	zip -r $(DOSSIER).zip $(DOSSIER)
	rm -rf $(DOSSIER)
	ls -l
