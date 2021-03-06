/*
 * ShannonFano.cpp
 */

#include "ShannonFano.h"
#include <bitset>

ShannonFano::ShannonFano() {
}

ShannonFano::~ShannonFano() {
    clearResources();
}

vector<code *> ShannonFano::encode(char* str, long len, int reduceLen) {
    clearResources();

    // create frequency table
    m_freqTable = createFreqTable(str, len);
    ShannonFano::printFreqTable(m_freqTable);

    // build code tree
    tree* root = buildTree(m_freqTable);

    // generate code table using code tree
    generateCodeTable(root);
    for (int i = 0; i < reduceLen; i++) {
        reduceCodeLen(m_codeTable);
    }
    printCodes(m_codeTable);

    // encode input data (ASCII) using code table 
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

vector<row *>  ShannonFano::getFreqTable() {
    return m_freqTable;
}

bool sortFunction (row* i, row* j) { return (i->count > j->count); }

vector<row *> ShannonFano::createFreqTable(char* str, long len) {
    vector<row *> freqTable;

    for (int i = 0; i < len; i++) {
        char symbol = str[i];
        int idx = findRow(freqTable, symbol);
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
    m_treeCache.push_back(root);
    int listWeight = 0;
    for (unsigned int i = 0; i < list.size(); i++) {
        listWeight += list[i]->count;
    }
    buildTree(root, list, listWeight);
    return root;
}

void ShannonFano::buildTree(tree* t, vector<row *> list, int listWeight) {
    int size = list.size();

    if (size <= 2) {
        tree* left = new tree;
        m_treeCache.push_back(left);
        left->data = list[0];
        left->left = NULL;
        left->right = NULL;
        t->left = left;

        if (size == 2) {
            tree* right = new tree;
            m_treeCache.push_back(right);
            right->data = list[1];
            right->left = NULL;
            right->right = NULL;
            t->right = right;
        }
    } else {
        int subWeight = 0;
        int middle = listWeight / 2;

        unsigned int midIdx = 0;
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
        m_treeCache.push_back(left);
        left->left = NULL;
        left->right = NULL;
        t->left = left;

        tree* right = new tree;
        m_treeCache.push_back(right);
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

void ShannonFano::reduceCodeLen(vector<code *> codeTable) {
    int maxLen = -1;
    for (int i = 0; i < codeTable.size(); i++) {
        int len = codeTable[i]->lenght;
        maxLen = maxLen < len ? len : maxLen;
    }

    int iter = 0;
    while(1) {
        int idx = findByLenght(codeTable, maxLen);
        int minIdx = findWithMinLenght(codeTable);
        if (idx < 0) {
            break;
        }
        code* maxCode = codeTable[idx];
        code* minCode = codeTable[minIdx];
        if (minCode->lenght >= maxLen) {
            cerr << "WARN: minCode->lenght >= maxLen" << endl;
            break;
        }

        if (iter % 2 == 0) {
            minCode->cipher = minCode->cipher << 1;
            minCode->lenght++;

            maxCode->cipher = minCode->cipher +  1;
            maxCode->lenght = minCode->lenght;
        } else {
            maxCode->cipher = maxCode->cipher >> 1;
            maxCode->lenght--;
        }
        iter++;
    }
}

int ShannonFano::findWithMinLenght(vector<code *> codeTable) {
    int result = -1;
    int minLen = 65535;
    for (int i = 0; i < codeTable.size(); i++) {
        int len = codeTable[i]->lenght;
        if (minLen > len) {
            minLen = len;
            result = i;
        }
    }
    return result;
}

int ShannonFano::findByLenght(vector<code *> codeTable, int len) {
    for (int i = 0; i < codeTable.size(); i++) {
        if (codeTable[i]->lenght == len) {
            return i;
        }
    }
    return -1;
}

int ShannonFano::findRow(vector<row *> table, char symbol) {
    for (unsigned int i = 0; i < table.size(); i++) {
        if (symbol == table[i]->symbol) {
            return i;
        }
    }
    return -1;
}

int ShannonFano::findCode(vector<code *> codes, char symbol) {
    for (unsigned int i = 0; i < codes.size(); i++) {
        if (symbol == codes[i]->symbol) {
            return i;
        }
    }
    return -1;
}

int ShannonFano::findCode(vector<code *> codes, unsigned int cipher, int lenght) {
    for (unsigned int i = 0; i < codes.size(); i++) {
        if (cipher == codes[i]->cipher && lenght == codes[i]->lenght) {
            return i;
        }
    }
    return -1;
}

void ShannonFano::printFreqTable(vector<row *> table) {
    for (unsigned int i = 0; i < table.size(); i++) {
        cout << table[i]->symbol << ":  " << table[i]->count << endl;
    }
}

void ShannonFano::printCodes(vector<code *> codeTable) {
    for (unsigned int i = 0; i < codeTable.size(); i++) {
        const int len = 8 * sizeof(unsigned int);
        cout << codeTable[i]->symbol
                << ": " << bitset<len>(codeTable[i]->cipher)
                << ": " << codeTable[i]->lenght << endl;
    }
}

void ShannonFano::printTree(tree* t) {
    if (t->left == NULL && t->right == NULL) {
        cout << "LEAF: " << t->data->symbol <<endl;
    }

    if (t->right != NULL) {
        printTree(t->right);
    }

    if (t->left != NULL) {
        printTree(t->left);
    }
}

void ShannonFano::clearResources() {
    for (unsigned int i = 0; i < m_freqTable.size(); i++) {
        delete m_freqTable[i];
    }
    for (unsigned int i = 0; i < m_codeTable.size(); i++) {
        delete m_codeTable[i];
    }
    for (unsigned int i = 0; i < m_treeCache.size(); i++) {
        delete m_treeCache[i];
    }
    m_freqTable.clear();
    m_codeTable.clear();
    m_treeCache.clear();
}
