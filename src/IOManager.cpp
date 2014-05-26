/*
 * IOManager.cpp
 */

#include "IOManager.h"
#include <stdio.h>
#include <fstream>
#include <bitset>

IOManager::IOManager() {
}

IOManager::~IOManager() {
}

/**
 * char buffer should be removed later
 */
int IOManager::readTextFile(char* fileName, char** memblock) {
    streampos size;
    ifstream file(fileName, ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        *memblock = new char [size];
        file.seekg(0, ios::beg);
        file.read(*memblock, size);
        file.close();
    } else {
        cerr << "Unable to open text file!" << endl;
    }
    return size;
}

void IOManager::writeAsText(char* fileName, vector<code *> codeStream) {
    ofstream outputFile;
    outputFile.open(fileName);
    for (int i = 0; i < codeStream.size(); i++) {
        outputFile << codeStream[i]->symbol;
    }
    outputFile.close();
}


void IOManager::writeAsCode(char* fileName,
        vector<code *> codeTable,
        vector<code *> codeStream) {

    FILE * file= fopen(fileName, "wb");
    if (file != NULL) {
        // write code table to file
        int codeTableLen = codeTable.size();
        fwrite(&codeTableLen, sizeof(int), 1, file);
        for (int i = 0; i < codeTableLen; i ++) {
            fwrite(codeTable[i], sizeof(code), 1, file);
        }

        // write code stream to file
        int codeStreamLen = codeStream.size();
        fwrite(&codeStreamLen, sizeof(int), 1, file);


        int cur = 0;
        byte data = 0;

        for (int i = 0; i < codeStreamLen; i++) {
            unsigned int cipher = codeStream[i]->cipher;
            int lenght = codeStream[i]->lenght;

            for (int i = lenght; i;) {
                int bit = (cipher >> --i) & 0x1;
                data = (data << 1) + bit;
                cur++;

                if (cur == 8) {
                    fwrite(&data, sizeof(byte), 1, file);

                    cur = 0;
                    data = 0;
                }
            }
        }
        // write tail
        data = data << (8 - cur);
        fwrite(&data, sizeof(byte), 1, file);

        // close file
        fclose(file);
    }
}

vector<code *> IOManager::readCodeFile(char* fileName) {

    vector <code *> codeStream;

    FILE * file= fopen(fileName, "rb");
    if (file != NULL) {
        // deserialize code table
        vector<code *> codeTable;

        int codeTableLen = -1;
        fread(&codeTableLen, sizeof(int), 1, file);

        for (int i = 0; i < codeTableLen; i++) {
            code * codeEntity = new code;
            fread(codeEntity, sizeof(code), 1, file);
            codeTable.push_back(codeEntity);
        }

        // deserialize code stream
        int codeStreamLen = -1;
        fread(&codeStreamLen, sizeof(int), 1, file);

        int amount = 0;
        unsigned int cipher = 0;
        unsigned int lenght = 0;

        byte data = 0;
        while (fread(&data, sizeof(byte), 1, file) == 1
               && amount < codeStreamLen) {

            for (int i = 0; i < 8 && amount < codeStreamLen; i++) {
                int bit = ((data >> (7 - i))) & 0x1;
                cipher = (cipher << 1) + bit;
                lenght++;

                int idx = ShannonFano::findCode(codeTable, cipher, lenght);
                if (idx != -1) {

                    cipher = 0;
                    lenght = 0;
                    amount++;

                    code * c = codeTable[idx];
                    codeStream.push_back(c);
                }
            }
        }
        fclose(file);
    } else {
        cerr << "Problem while reading file: " << fileName << endl;
    }
    return codeStream;
}

bool sortByAlpha (row* i, row* j) { return (i->symbol < j->symbol); }
bool sortByFreqn (row* i, row* j) { return (i->count > j->count); }

void IOManager::writeFreqTable(char* fileName,
        vector<row *> freqTable, int sortType) {

    sort(freqTable.begin(), freqTable.end(),
        sortType == BY_ALPHA ? sortByAlpha : sortByFreqn);

    ofstream outputFile;
    outputFile.open(fileName);
    for (unsigned int i = 0; i < freqTable.size(); i++) {
        outputFile << freqTable[i]->symbol << ":  " << freqTable[i]->count << endl;
    }
    outputFile.close();
}

void IOManager::writeCodeTable(char* fileName, vector<code *> codeTable) {
    ofstream outputFile;
    outputFile.open(fileName);
    for (unsigned int i = 0; i < codeTable.size(); i++) {
        const int len = 8 * sizeof(unsigned int);
        outputFile << codeTable[i]->symbol
                   << ": " << bitset<len>(codeTable[i]->cipher)
                   << ": " << codeTable[i]->lenght << endl;
    }
    outputFile.close();
}
