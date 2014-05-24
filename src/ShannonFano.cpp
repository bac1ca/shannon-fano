/*
 * ShannonFano.cpp
 */

#include "ShannonFano.h"
#include <algorithm>

ShannonFano::ShannonFano() {
    vector<row *> table;
}

ShannonFano::~ShannonFano() {
    for (int i = 0; i < table.size(); i++) {
        delete table[i];
    }
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
