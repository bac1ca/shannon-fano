/*
 * ShannonFano.h
 */

#ifndef SHANNONFANO_H_
#define SHANNONFANO_H_

#include <vector>
#include <iostream>

using namespace std;

class ShannonFano {
private:
    typedef struct _row {
        char key;
        int count;
    } row;

    vector<row *> table;

    typedef struct _node {
        row tableRow;
        struct _node *left;
        struct _node *right;
    } node;

    node *m_head;

public:
    ShannonFano();
    virtual ~ShannonFano();

public:
    void createTable(char * str, long len);

private:
    int find(vector<row *> table, char symbol);
    void printTable(vector<row *> table);

};

#endif /* SHANNONFANO_H_ */
