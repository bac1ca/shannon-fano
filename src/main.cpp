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




int main(int argc, char *argv[]) {

    if (argc != 2) {
        cout <<"Invalid arguments! usage: "
             << argv[0] << " <bookname>" << endl;
        return 0;
    }
    char * path = argv[1];


    streampos size;
    char * memblock;
    ifstream file(path,ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        memblock = new char [size];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();
    } else {
        cerr << "Unable to open bookfile!" << endl;
        return -1;
    }

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

    delete[] memblock;
    return 0;
}
