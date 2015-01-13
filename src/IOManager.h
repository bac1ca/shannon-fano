/*
 * IOManager.h
 */

#ifndef IOMANAGER_H_
#define IOMANAGER_H_

#include "ShannonFano.h"

typedef unsigned char byte;

#define BY_FREQN 0
#define BY_ALPHA 1

/**
 * Unility class, resposible for writing/reading data
 */
class IOManager {

public:
    IOManager();
    virtual ~IOManager();

public:
    /**
     * read text file to memory buffer
     */
    int readTextFile(char* fileName, char** memblock);

    /**
     * write code stream to file
     */
    void writeAsText(char* fileName, vector<code *> codeStream);

    /**
     * read stream of codes from binary file
     */
    vector<code *> readCodeFile(char* fileName);

    /**
     * write code table and stream of codes to bonary file
     */
    void writeAsCode(char* fileName, vector<code *> codeTable, vector<code *> codeStream);

    /**
     * write frequency table to text file
     */
    void writeFreqTable(char* fileName, vector<row  *> freqTable, int sortType);

    /**
     * write code table to text file
     */
    void writeCodeTable(char* fileName, vector<code *> codeTable);

};

#endif /* IOMANAGER_H_ */
