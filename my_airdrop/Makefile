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
	g++ -I include src/bluetooth.cpp -lsimpleble -o bin/bluetooth $(shell pkg-config --cflags --libs dbus-1)
	sudo ./bin/bluetooth
