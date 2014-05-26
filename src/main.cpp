//============================================================================
// Name        : shanon2.cpp
// Author      : vasily
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ShannonFano.h"
#include "IOManager.h"

using namespace std;

#define SYMBOLS_FREQN_FILE "symbols_freqn.txt"
#define SYMBOLS_ALPHA_FILE "symbols_alpha.txt"

#define COMPRESS_FILE      "compress.bin"
#define UNCOMPRESS_FILE    "uncompress.txt"


int main(int argc, char *argv[]) {

//    if (argc != 2) {
//        cout <<"Invalid arguments! usage: "
//             << argv[0] << " <bookname>" << endl;
//        return 0;
//    }
//    char* path = argv[1];
//    char* path = "/home/vasisa/workspaces/cppworkspace42/shanon2/book.txt";
    char* path = "/home/vasisa/workspaces/cppworkspace42/shanon2/potter.txt";

    char* memblock;

    IOManager ioManager;
    int size = ioManager.readTextFile(path, &memblock);

    ShannonFano shannonFano;
    vector<code *> codeStream = shannonFano.encode(memblock, size, 3);
    vector<code *> codeTable  = shannonFano.getCodeTable();
    vector<row  *> freqTable  = shannonFano.getFreqTable();

    ioManager.writeFreqTable(SYMBOLS_ALPHA_FILE, freqTable, BY_ALPHA);
    ioManager.writeFreqTable(SYMBOLS_FREQN_FILE, freqTable, BY_FREQN);

    ioManager.writeAsCode(COMPRESS_FILE, codeTable, codeStream);
    vector<code *> uncompress = ioManager.readCodeFile(COMPRESS_FILE);

    ioManager.writeAsText(UNCOMPRESS_FILE, uncompress);

    cout << endl << "size of file: " << size << endl;

    delete[] memblock;
    return 0;
}
