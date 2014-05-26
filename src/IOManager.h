/*
 * IOManager.h
 */

#ifndef IOMANAGER_H_
#define IOMANAGER_H_

#include "ShannonFano.h"

typedef unsigned char byte;

#define BY_FREQN 0
#define BY_ALPHA 1

class IOManager {

public:
    IOManager();
    virtual ~IOManager();

public:
    int readTextFile(char* fileName, char** memblock);
    void writeAsText(char* fileName, vector<code *> codeStream);

    vector<code *> readCodeFile(char* fileName);
    void writeAsCode(char* fileName, vector<code *> codeTable, vector<code *> codeStream);

    void writeFreqTable(char* fileName, vector<row  *> freqTable, int sortType);
    void writeCodeTable(char* fileName, vector<code *> codeTable);

};

#endif /* IOMANAGER_H_ */
