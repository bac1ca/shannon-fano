/*
 * ShannonFano.h
 */

#ifndef SHANNONFANO_H_
#define SHANNONFANO_H_

#include <vector>
#include <iostream>

using namespace std;

typedef struct _row {
    char key;
    int count;
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
    vector<row *> table;
    vector<code *> codeTable;
    tree* m_root;

public:
    ShannonFano();
    virtual ~ShannonFano();

public:
    void createTable(char* str, long len);
    void buildTree(tree* t, vector<row *> list);
    void generateCodeTable(tree* root);
    void encode(char* str, char** data, int size);

private:
    int find(vector<row *> table, char symbol);
    int findCode(vector<code *> codes, char symbol);

    void buildTree(tree* t, vector<row *> table, int treeWeight);
    void bypassTree(tree* t, int value, int count);

public:
    static void printTable(vector<row *> table);
    static void printCodeTable(vector<code *> codeTable);
    static void printTree(tree* root);

    static int findCode(vector<code *> codes, unsigned int cipher, int count);


};

#endif /* SHANNONFANO_H_ */
