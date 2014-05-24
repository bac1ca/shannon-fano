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

using namespace std;

typedef unsigned char byte;

int cur = 0;
byte data = 0;

void flushCode(ofstream* outbin, unsigned int code, int len) {
    int shift = (cur + len) > 8 ? (cur + len) - 8 : 0;

    data = data << (len - shift);
    code = code >> shift;
    data += (byte) code;

    cur = cur + (len - shift);

    if (cur == 8) {
        *outbin << data;
        cur = 0;
    }

    if (shift > 0) {
        cout << "recursive call" << endl;
        flushCode(outbin, code, shift);
    }
}

int main() {

    streampos size;
    char * memblock;

//    char * path = "/home/vasisa/workspaces/cppworkspace42/shanon2/potter.txt";
    char * path = "/home/vasisa/workspaces/cppworkspace42/shanon2/book.txt";

    ifstream file(path,ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        memblock = (char *) malloc(size * sizeof(char));
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();

        ShannonFano s;
        s.createTable(memblock, size);

        cout << "size: " << size << endl;
        free(memblock);
    } else {
        cout << "Unable to open file";
    }

    ofstream outbin("myfile", ios::out | ios::binary);
    flushCode(&outbin, 3, 2);
    flushCode(&outbin, 2, 2);
    flushCode(&outbin, 3, 2);
    flushCode(&outbin, 2, 2);

    flushCode(&outbin, 5, 3);
    flushCode(&outbin, 5, 3);
    flushCode(&outbin, 3, 2);
    //flushCode(&outbin, 5, 3);
    outbin.close();

//    byte b = 5;
//    b = b << 3;
//    b += 5;
//    b = b << 2;

    cout << "The end!" << endl;
    return 0;
}
