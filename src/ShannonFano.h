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
     * encoding input text data
     */
    vector<code *> encode(char* str, long len, int reduceCodeLen = 0);
    vector<code *> getCodeTable();
    vector<row  *> getFreqTable();

private:
    /**
     * generate frequency table 
     */
    vector<row *> createFreqTable(char* str, long len);

    /**
     * build code tree
     */
    tree* buildTree(vector<row *> list);

    /**
     * generate code table using code tree
     */
    void  generateCodeTable(tree* root);

    /**
     * methods which are implemented for code length restrictions
     */
    void reduceCodeLen(vector<code *> codeTable);
    int findWithMinLenght(vector<code *> codeTable);
    int findByLenght(vector<code *> codeTable, int len);

private:
    void buildTree(tree* t, vector<row *> table, int treeWeight);
    void bypassTree(tree* t, int value, int count);

    // utility methods for searching in frequency table 
    // and for searching code by symbol    
    int findRow(vector<row *> table, char symbol);
    int findCode(vector<code *> codes, char symbol);
    void clearResources();

    // utility methods for printing information
public:
    static void printFreqTable(vector<row *> table);
    static void printCodes(vector<code *> codeTable);
    static void printTree(tree* root);

    static int findCode(vector<code *> codes, unsigned int cipher, int count);

};

#endif /* SHANNONFANO_H_ */
