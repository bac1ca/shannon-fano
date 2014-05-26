/**
 * Main of ShannonFano alg
 */

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
#define COMPRESS_FILE_1    "compress1.bin"
#define COMPRESS_FILE_3    "compress3.bin"
#define COMPRESS_FILE_2    "compress2.bin"

#define DECOMPRESS_FILE    "decompress.txt"
#define DECOMPRESS_FILE_1  "decompress1.txt"
#define DECOMPRESS_FILE_2  "decompress2.txt"
#define DECOMPRESS_FILE_3  "decompress3.txt"

#define CODES_FILE_0       "codes.txt"
#define CODES_FILE_1       "codes1.txt"
#define CODES_FILE_2       "codes2.txt"
#define CODES_FILE_3       "codes3.txt"


#define CODE_LEN_REDUCE_LEVEL1  1
#define CODE_LEN_REDUCE_LEVEL2  2
#define CODE_LEN_REDUCE_LEVEL3  3

/**
 * Запуск Алгоритма Шеннона-Фано с сокращение длины кода на
 * reduceLenLevel соответственно
 */
void compressWithCodeLenRestrictions(char* memblock, int size,
        IOManager * ioManager, int reduceLenLevel) {
    char* compressFName = "";
    char* decompressFName = "";
    char* codesFName = "";

    if (reduceLenLevel == CODE_LEN_REDUCE_LEVEL1) {
        compressFName   = COMPRESS_FILE_1;
        decompressFName = DECOMPRESS_FILE_1;
        codesFName      = CODES_FILE_1;
    } else if (reduceLenLevel == CODE_LEN_REDUCE_LEVEL2) {
        compressFName   = COMPRESS_FILE_2;
        decompressFName = DECOMPRESS_FILE_2;
        codesFName      = CODES_FILE_2;
    } else if (reduceLenLevel == CODE_LEN_REDUCE_LEVEL3) {
        compressFName   = COMPRESS_FILE_3;
        decompressFName = DECOMPRESS_FILE_3;
        codesFName      = CODES_FILE_3;
    } else {
        cerr << "warn incorrect level of code reducing: "
             << reduceLenLevel << endl;
    }

    ShannonFano shannonFano;
    vector<code *> codeStream = shannonFano.encode(memblock, size, reduceLenLevel);
    vector<code *> codeTable  = shannonFano.getCodeTable();
    ioManager->writeCodeTable(codesFName, codeTable);

    ioManager->writeAsCode(compressFName, codeTable, codeStream);
    vector<code *> decompress = ioManager->readCodeFile(compressFName);
    ioManager->writeAsText(decompressFName, decompress);
}

/**
 * запуск программы: должен быть указан путь,
 * как аргумент коммандной строки
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout <<"Invalid arguments! usage: "
             << argv[0] << " <bookname>" << endl;
        return 0;
    }
    char* path = argv[1];
    //for quick start....
    //char* path = "/home/vasisa/workspaces/cppworkspace42/shanon2/book.txt";
    //char* path = "/home/vasisa/workspaces/cppworkspace42/shanon2/potter.txt";

    /*
     * Считывание файла
     */
    char* memblock;
    IOManager ioManager;
    int size = ioManager.readTextFile(path, &memblock);

    /*
     * Запуск алгоритма Шеннона-Фано без ограниечений на длину кода
     */
    ShannonFano shannonFano;
    vector<code *> codeStream = shannonFano.encode(memblock, size);
    vector<code *> codeTable  = shannonFano.getCodeTable();
    vector<row  *> freqTable  = shannonFano.getFreqTable();

    /**
     * запись промежуточных файлов (в соответствии с требованиями
     * к лаб. работе)
     */
    ioManager.writeFreqTable(SYMBOLS_ALPHA_FILE, freqTable, BY_ALPHA);
    ioManager.writeFreqTable(SYMBOLS_FREQN_FILE, freqTable, BY_FREQN);
    ioManager.writeCodeTable(CODES_FILE_0, codeTable);

    ioManager.writeAsCode(COMPRESS_FILE, codeTable, codeStream);
    vector<code *> decompress = ioManager.readCodeFile(COMPRESS_FILE);
    ioManager.writeAsText(DECOMPRESS_FILE, decompress);

    /*
     * Сокращение длины кода на 1, 2 и 3 соответственно
     */
    compressWithCodeLenRestrictions(memblock, size, &ioManager, CODE_LEN_REDUCE_LEVEL1);
    compressWithCodeLenRestrictions(memblock, size, &ioManager, CODE_LEN_REDUCE_LEVEL2);
    compressWithCodeLenRestrictions(memblock, size, &ioManager, CODE_LEN_REDUCE_LEVEL3);

    cout << endl << "size of input text file: " << size << endl;
    delete[] memblock;
    return 0;
}
