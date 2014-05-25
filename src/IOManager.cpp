/*
 * IOManager.cpp
 */

#include "IOManager.h"

IOManager::IOManager() {
    m_cur = 0;
    m_data = 0;
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

        m_cur = 0;
        m_data = 0;
        for (int i = 0; i < codeStreamLen; i++) {
            flushCode(file, codeStream[i]->cipher, codeStream[i]->lenght);
        }
        // flush the tail of the codeStream
        m_data = m_data << (8 - m_cur);
        fwrite(&m_data, sizeof(byte), 1, file);

        // close file
        fclose(file);
    }
}

void IOManager::flushCode(FILE* file, int code, int len) {

    int shift = (m_cur + len) > 8 ? (m_cur + len) - 8 : 0;

    m_data = m_data << (len - shift);
    code = code >> shift;
    m_data += (byte) code;

    m_cur = m_cur + (len - shift);

    if (m_cur == 8) {
        fwrite(&m_data, sizeof(byte), 1, file);
        m_data = 0;
        m_cur  = 0;
    }

    if (shift > 0) {
        cout << "recursive call" << endl;
        flushCode(file, code, shift);
    }
}


void IOManager::readData(char* fileName,
        vector<code *> * codeTable,
        vector<code *> * codeStream) {

    FILE * file= fopen(fileName, "rb");
    if (file != NULL) {
        // deserialize code table
        int codeTableLen = -1;
        fread(&codeTableLen, sizeof(int), 1, file);

        for (int i = 0; i < codeTableLen; i++) {
            code * codeEntity = new code;
            fread(codeEntity, sizeof(code), 1, file);
            codeTable->push_back(codeEntity);
        }

        // deserialize code stream
        int codeStreamLen = -1;
        fread(&codeStreamLen, sizeof(int), 1, file);
        cout << "codeStreamLen: " << codeStreamLen << endl;

        int amount = 0;
        m_data = 0;
        m_cur =  0;

        unsigned int cipher = 0;
        unsigned int lenght = 0;
        vector<code *> codes = *codeTable;

        while (fread(&m_data, sizeof(byte), 1, file) == 1
               && amount < codeStreamLen) {

            for (int i = 0; i < 8; i++) {
                int bit = ((m_data >> (7 - i))) & 0x1;
                cipher = (cipher << 1) + bit;
                lenght++;

                int idx = ShannonFano::findCode(*codeTable, cipher, lenght);
                if (idx != -1) {
                    cout << "found: " << idx << endl;

                    cipher = 0;
                    lenght = 0;
                    amount++;

                    code * c = codes[i];
                    codeStream->push_back(c);
                }
            }
        }

        cout << "Still live" << endl;
        fclose(file);
    }
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
    codeStream.push_back(&A);
    codeStream.push_back(&N);
    codeStream.push_back(&M);

    ShannonFano::printCodeTable(codeTable);

    flushData("result.bin", codeTable, codeStream);


    // Deserialize data
    vector<code *> inCodeTable;
    vector<code *> inCodeStream;
    readData("result.bin", &inCodeTable, &inCodeStream);

    ShannonFano::printCodeTable(inCodeTable);

    cout << "END OF THE TEST!!!" << endl;
}
