make: 	
	rm -rf bin
	mkdir bin
	gcc -c -I include src/client.c -o bin/client.o
	gcc -c -I include src/main.c -o bin/main.o
	gcc -c -I include util/helpers.c -o bin/helpers.o
	gcc -c -I include src/tui.c -o bin/tui.o
	gcc bin/client.o bin/main.o bin/helpers.o bin/tui.o -lpanel -lncurses -o bin/airdrop 

debug:
	valgrind ./bin/airdrop

clean: 
	rm -rf bin
	mkdir bin

blue:
	g++ -c -I include src/discovery.cpp -o bin/bluetooth.o
	g++ -c -I include linux/ldiscover.cpp -o bin/ldiscover.o
	g++ -I include bin/bluetooth.o bin/ldiscover.o -o bin/bluetooth
	sudo ./bin/bluetooth
