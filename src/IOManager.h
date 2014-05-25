/*
 * IOManager.h
 */

#ifndef IOMANAGER_H_
#define IOMANAGER_H_

#include "ShannonFano.h"

typedef unsigned char byte;

class IOManager {

public:
    IOManager();
    virtual ~IOManager();

public:
    void flushData(char* fileName, vector<code *> codeTable, vector<code *> codeStream);
    vector<code *> readData(char* fileName);

};

#endif /* IOMANAGER_H_ */
