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
    for (int i = 0; i < table.size(); i++) {
        delete table[i];
    }

    // CLEAN TREE

    // CLEAN CODE TABLE
}

bool sortFunction (row* i, row* j) { return (i->count > j->count); }

void ShannonFano::createTable(char* str, long len) {
    for (int i = 0; i < len; i++) {
        char symbol = str[i];
        int idx = find(table, symbol);
        if (idx == -1) {
            row* tableRow  = new row;
            tableRow->key = symbol;
            tableRow->count = 1;
            table.push_back(tableRow);
        } else {
            row* tableRow = table[idx];
            tableRow->count++;
        }
    }

    sort(table.begin(), table.end(), sortFunction);
    printTable(table);

    // TODO
    buildTree(m_root, table);
    printTree(m_root);

//    // TODO
    generateCodeTable(m_root);
    printCodes(codeTable);
}

void ShannonFano::buildTree(tree* root, vector<row *> list) {
    int listWeight = 0;
    for (int i = 0; i < list.size(); i++) {
        listWeight += list[i]->count;
    }
    buildTree(root, list, listWeight);
}

void ShannonFano::buildTree(tree* t, vector<row *> list, int listWeight) {
    int size = list.size();

    cout << "size: " << size << ", listWeight: " << listWeight;

    if (size <= 2) {
        cout << " LEAF " << endl;

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

        cout << " ............. " << "mid: " << middle << ", midIdx: " << midIdx << ", subWeight: " << subWeight  << endl;

        // divide list
        vector<row *> leftList(list.begin(), list.begin() + midIdx + 1);
        vector<row *> rightList(list.begin() + midIdx + 1, list.end());

//        // TODO debug
//        if (true) {
//            cout << "-------------------------------" << endl;
//            printTable(leftList);
//            cout << "-------" << endl;
//            printTable(rightList);
//            cout << "-------------------------------" << endl;
//        }

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
        codeEntity->symbol = leaf->key;
        codeEntity->cipher = value;
        codeEntity->lenght = count;
        // adding new entry
        codeTable.push_back(codeEntity);
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

void ShannonFano::encode(char* str, char** data, int size) {

}

int ShannonFano::find(vector<row *> table, char symbol) {
    for (int i = 0; i < table.size(); i++) {
        if (symbol == table[i]->key) {
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


void ShannonFano::printTable(vector<row *> table) {
    for (int i = 0; i < table.size(); i++) {
        cout << table[i]->key << ":  " << table[i]->count << endl;
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

void ShannonFano::printTree(tree* tree) {
    if (tree->left != NULL) {
        //cout << "L " << endl;
        printTree(tree->left);
    }
    if (tree->right != NULL) {
        //cout << "R " << endl;
        printTree(tree->right);
    }
    if (tree->left == NULL && tree->right == NULL) {
        cout << "LEAF " << tree->data->key << endl;
    }
}
