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
 * Утилитный класс, отвечает за запись/считывание данных
 */
class IOManager {

public:
    IOManager();
    virtual ~IOManager();

public:
    /**
     * Считывание текстового файла в память
     */
    int readTextFile(char* fileName, char** memblock);

    /**
     * Конвертация и запись потока кодов в текстовый файл
     */
    void writeAsText(char* fileName, vector<code *> codeStream);

    /**
     * считывание потока кодов с бинарного файла
     */
    vector<code *> readCodeFile(char* fileName);

    /**
     * запись таблицы кодов и потока кодов в бинарный файл
     */
    void writeAsCode(char* fileName, vector<code *> codeTable, vector<code *> codeStream);

    /**
     * запись таблицы частот в текстовый файл
     */
    void writeFreqTable(char* fileName, vector<row  *> freqTable, int sortType);

    /**
     * запись таблицы кодов в тестовый файл
     */
    void writeCodeTable(char* fileName, vector<code *> codeTable);

};

#endif /* IOMANAGER_H_ */
