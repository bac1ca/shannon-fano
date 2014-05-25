/*
 * IOManager.h
 */

#ifndef IOMANAGER_H_
#define IOMANAGER_H_

#include "ShannonFano.h"
#include <stdio.h>

typedef unsigned char byte;

class IOManager {
private:
    int m_cur;
    byte m_data;

public:
    IOManager();
    virtual ~IOManager();

public:
    void flushData(char* fileName, vector<code *> codeTable, vector<code *> codeStream);
    void readData(char* fileName, vector<code *> * codeTable, vector<code *> * codeStream);

private:
    void flushCode(FILE* file, int code, int len);


// TODO Remove it later
public:
    void testIOManager();
};

#endif /* IOMANAGER_H_ */
