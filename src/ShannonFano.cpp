/*
 * ShannonFano.cpp
 */

#include "ShannonFano.h"
#include <algorithm>
#include <bitset>

ShannonFano::ShannonFano() {
    m_root = new tree;
}

ShannonFano::~ShannonFano() {
    for (int i = 0; i < m_freqTable.size(); i++) {
        delete m_freqTable[i];
    }
    for (int i = 0; i < m_codeTable.size(); i++) {
        delete m_codeTable[i];
    }

    // CLEAN TREE
    // TODO METHOD CLEAR_ALL
}

vector<code *> ShannonFano::encode(char* str, long len) {
    // TODO clear _All

    m_freqTable = createFreqTable(str, len);

    m_root = buildTree(m_freqTable);

    generateCodeTable(m_root);

    vector<code *> codeStream;
    for (int i = 0; i < len; i++) {
        char symbol = str[i];

        int idx = findCode(m_codeTable, symbol);
        if (idx == -1) {
            cerr << "WARN: unknown symbol: " << symbol << endl;
        } else {
            codeStream.push_back(m_codeTable[idx]);
        }
    }
    return codeStream;
}

vector<code *> ShannonFano::getCodeTable() {
    return m_codeTable;
}


bool sortFunction (row* i, row* j) { return (i->count > j->count); }

vector<row *> ShannonFano::createFreqTable(char* str, long len) {
    vector<row *> freqTable;

    for (int i = 0; i < len; i++) {
        char symbol = str[i];
        int idx = find(freqTable, symbol);
        if (idx == -1) {
            row* tableRow  = new row;
            tableRow->symbol = symbol;
            tableRow->count = 1;
            freqTable.push_back(tableRow);
        } else {
            row* tableRow = freqTable[idx];
            tableRow->count++;
        }
    }
    sort(freqTable.begin(), freqTable.end(), sortFunction);
    return freqTable;
}

tree* ShannonFano::buildTree(vector<row *> list) {
    tree* root = new tree;
    int listWeight = 0;
    for (int i = 0; i < list.size(); i++) {
        listWeight += list[i]->count;
    }
    buildTree(root, list, listWeight);
    return root;
}

void ShannonFano::buildTree(tree* t, vector<row *> list, int listWeight) {
    int size = list.size();

    if (size <= 2) {
        tree* left = new tree;
        left->data = list[0];
        left->left = NULL;
        left->right = NULL;
        t->left = left;

        if (size == 2) {
            tree* right = new tree;
            right->data = list[1];
            right->left = NULL;
            right->right = NULL;
            t->right = right;
        }
    } else {
        int subWeight = 0;
        int middle = listWeight / 2;

        int midIdx = 0;
        for (midIdx = 0; midIdx < list.size(); midIdx++) {
            subWeight += list[midIdx]->count;
            if (subWeight >= middle) {
                break;
            }
        }

        // divide list
        vector<row *> leftList(list.begin(), list.begin() + midIdx + 1);
        vector<row *> rightList(list.begin() + midIdx + 1, list.end());

        tree* left = new tree;
        left->left = NULL;
        left->right = NULL;
        t->left = left;

        tree* right = new tree;
        right->left = NULL;
        right->right = NULL;
        t->right = right;

        // recursive call
        buildTree(left, leftList, subWeight);
        buildTree(right, rightList, listWeight - subWeight);
    }
}

void ShannonFano::generateCodeTable(tree* root) {
    bypassTree(root, 0, 0);
}

void ShannonFano::bypassTree(tree* t, int value, int count) {
    if (t == NULL) {
        cerr << "WARN: input tree equals NULL" << endl;
        return;
    }

    if (t->left == NULL && t->right == NULL) {
        row* leaf = t->data;
        code* codeEntity = new code;
        codeEntity->symbol = leaf->symbol;
        codeEntity->cipher = value;
        codeEntity->lenght = count;
        // adding new entry
        m_codeTable.push_back(codeEntity);
        return;
    }

    value = value << 1;
    count++;
    if (t->left != NULL) {
        // recursive call
        bypassTree(t->left, value, count);
    }
    if (t->right != NULL) {
        value++;
        // recursive call
        bypassTree(t->right, value, count);
    }
}


int ShannonFano::find(vector<row *> table, char symbol) {
    for (int i = 0; i < table.size(); i++) {
        if (symbol == table[i]->symbol) {
            return i;
        }
    }
    return -1;
}

int ShannonFano::findCode(vector<code *> codes, char symbol) {
    for (int i = 0; i < codes.size(); i++) {
        if (symbol == codes[i]->symbol) {
            return i;
        }
    }
    return -1;
}

int ShannonFano::findCode(vector<code *> codes, unsigned int cipher, int lenght) {
    for (int i = 0; i < codes.size(); i++) {
        if (cipher == codes[i]->cipher && lenght == codes[i]->lenght) {
            return i;
        }
    }
    return -1;
}


void ShannonFano::printFreqTable(vector<row *> table) {
    for (int i = 0; i < table.size(); i++) {
        cout << table[i]->symbol << ":  " << table[i]->count << endl;
    }
}

void ShannonFano::printCodes(vector<code *> codeTable) {
    for (int i = 0; i < codeTable.size(); i++) {
        const int len = 8 * sizeof(unsigned int);
        cout << codeTable[i]->symbol
                << ": " << bitset<len>(codeTable[i]->cipher)
                << ": " << codeTable[i]->lenght << endl;
    }
}
