#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
using namespace std;

#define STORAGE_FILENAME ".todo.txt"



void list() {
	ifstream storage(STORAGE_FILENAME);	
	string line;
	if (storage.is_open()) {
		while (getline(storage, line)) {
			cout << line << endl;	
		}
		storage.close();
	}
	else {
		cout << "Unable to open file .todo.txt" << endl;
		exit(EXIT_FAILURE);
	}
}

void addItem(string item) {
	ofstream storage(STORAGE_FILENAME);
	ifstream sout(STORAGE_FILENAME);


	string line;
	if (storage.is_open()) {
		getline(sout, line, '\n');
		storage.close();
	}
	
	else {
		cout << "Unable to open file .todo.txt" << endl;
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "Usage: todo [larh] [option]" << endl;
		return EXIT_FAILURE;
	}
	
	int opt;
	string item;
	while ((opt = getopt(argc, argv, "la:r:h")) != -1) {
		switch(opt) {
			case 'l':
				list();
				break;
			case 'a':
				addItem(optarg);
				break;
			case 'r':
	//			removeItem(optarg);
				break;
			case 'h':
	//			printHelpMenu();
				break;
			default:
				cout << "Unknown option: " << opt << endl;
				exit(EXIT_FAILURE);
		}
	}

	return EXIT_SUCCESS;
}





