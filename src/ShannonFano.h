/*
 * ShannonFano.h
 */

#ifndef SHANNONFANO_H_
#define SHANNONFANO_H_

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

typedef struct _row {
    char symbol;
    int  count;
} row;

typedef struct _tree {
    row* data;
    struct _tree *left;
    struct _tree *right;
} tree;

typedef struct _code {
    char symbol;
    unsigned int cipher; // according to lab restrictions
    int lenght;
} code;


class ShannonFano {
private:
    vector<row  *> m_freqTable;
    vector<code *> m_codeTable;
    vector<tree *> m_treeCache;

public:
    ShannonFano();
    virtual ~ShannonFano();

public:
    /**
     * кодировние входных тестровых данных
     */
    vector<code *> encode(char* str, long len, int reduceCodeLen = 0);
    vector<code *> getCodeTable();
    vector<row  *> getFreqTable();

private:
    /**
     * генерация таблицы частот
     */
    vector<row *> createFreqTable(char* str, long len);

    /**
     * построние кодового дерева
     */
    tree* buildTree(vector<row *> list);

    /**
     * генерация таблицы кодов по кодовому дереву
     */
    void  generateCodeTable(tree* root);

    /**
     * методы предназначенные для согращения длины кода
     */
    void reduceCodeLen(vector<code *> codeTable);
    int findWithMinLenght(vector<code *> codeTable);
    int findByLenght(vector<code *> codeTable, int len);

private:
    void buildTree(tree* t, vector<row *> table, int treeWeight);
    void bypassTree(tree* t, int value, int count);

    // утилитные методы поиска в таюлице частот и кодов по символу
    int findRow(vector<row *> table, char symbol);
    int findCode(vector<code *> codes, char symbol);
    void clearResources();

    // утилитные методы для вывода информации на экран
public:
    static void printFreqTable(vector<row *> table);
    static void printCodes(vector<code *> codeTable);
    static void printTree(tree* root);

    static int findCode(vector<code *> codes, unsigned int cipher, int count);

};

#endif /* SHANNONFANO_H_ */
