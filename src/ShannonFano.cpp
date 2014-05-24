/*
 * ShannonFano.cpp
 */

#include "ShannonFano.h"
#include <algorithm>
#include <bitset>

ShannonFano::ShannonFano() {
    root = new tree;
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
    buildTree(root, table);

    // TODO
    generateCodeTable(root);

    printCodeTable(codeTable);
}

void ShannonFano::buildTree(tree* root, vector<row *> list) {
    int listWeight = 0;
    for (int i = 0; i < list.size(); i++) {
        listWeight += table[i]->count;
    }
    cout << "listWeight: " << listWeight << endl;

    buildTree(root, table, listWeight);
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

        //TODO assert
        if (leftList.size() + rightList.size() != size) {
            cout << "-------------------------------" << endl;
            cout << "AAAAAAAAAAAA SIZE!!!!!!!!!!!!" << endl;
            cout << "LEFT SIZE:" << leftList.size() << endl;
            cout << "RIGHT SIZE:" << rightList.size() << endl;
            cout << "-------------------------------" << endl;
        }

        tree* left = new tree;
        t->left = left;
        tree* right = new tree;
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
    } else if (t->left != NULL) {
        value = value << 1;
        count++;
        // recursive call
        bypassTree(t->left, value, count);
    } else if (t->right != NULL) {
        value = value << 1;
        value++;
        count++;
        // recursive call
        bypassTree(t->right, value, count);
    }
}

int ShannonFano::find(vector<row *> table, char symbol) {
    for (int i = 0; i < table.size(); i++) {
        if (symbol == table[i]->key) {
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

void ShannonFano::printCodeTable(vector<code *> codeTable) {
    for (int i = 0; i < codeTable.size(); i++) {
        const int len = sizeof(unsigned int);
        cout << codeTable[i]->symbol << ": " << bitset<len>(codeTable[i]->cipher) << endl;
    }
}
