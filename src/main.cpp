//============================================================================
// Name        : shanon2.cpp
// Author      : vasily
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ShannonFano.h"
#include "IOManager.h"

using namespace std;


int main() {

    streampos size;
    char * memblock;

    char * path = "/home/vasisa/workspaces/cppworkspace42/shanon2/potter.txt";
//    char * path = "/home/vasisa/workspaces/cppworkspace42/shanon2/book.txt";

    ifstream file(path,ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        memblock = (char *) malloc(size * sizeof(char));
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();

        ShannonFano s;
        vector<code *> codeStream = s.encode(memblock, size);
        vector<code *> codeTable  = s.getCodeTable();

        IOManager ioManager;
        ioManager.flushData("man.bin", codeTable, codeStream);

        vector<code *> codeStreamZipped = ioManager.readData("man.bin");



        ofstream outputFile;
        outputFile.open ("out.txt");
        for (int i = 0; i < codeStreamZipped.size(); i++) {
            outputFile << codeStreamZipped[i]->symbol;
        }
        outputFile.close();



        cout << endl << "size of file: " << size << endl;
        free(memblock);
    } else {
        cout << "Unable to open file";
    }

//    IOManager ioManager;
//    ioManager.testIOManager();
    return 0;
}
