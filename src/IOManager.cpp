/*
 * IOManager.cpp
 */

#include "IOManager.h"

IOManager::IOManager() {
}

IOManager::~IOManager() {
}

void IOManager::flushData(char* fileName,
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

vector<code *> IOManager::readData(char* fileName) {

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



void IOManager::testIOManager() {

    code A;
    A.symbol = 'A';
    A.cipher = 0;
    A.lenght = 2;

    code N;
    N.symbol = 'N';
    N.cipher = 4;
    N.lenght = 3;

    code M;
    M.symbol = 'M';
    M.cipher = 5;
    M.lenght = 3;

    vector<code *> codeTable;
    codeTable.push_back(&A);
    codeTable.push_back(&N);
    codeTable.push_back(&M);

    vector<code *> codeStream;
    codeStream.push_back(&M);
    codeStream.push_back(&N);
    codeStream.push_back(&M);
    codeStream.push_back(&A);
    codeStream.push_back(&M);

    ShannonFano::printCodes(codeTable);

    flushData("result.bin", codeTable, codeStream);


    // Deserialize data
    vector<code *> fileCodeStream = readData("result.bin");
    ShannonFano::printCodes(fileCodeStream);

    cout << "END OF THE TEST!!!" << endl;
}
