#pragma once
#include "Grammar.h"

class SingleProduction {
    int index;
    set<char> follow;
    pair<char, string> production;
};

class Item {
    int ID;
    vector<SingleProduction> productionList;
    map<char, int> go;
};

class LR1ParsingTable {
public:
    LR1ParsingTable(Grammar*);

private:
    Grammar* grammar;
    map<pair<int, char>, int> actiontable;
    map<pair<int, char>, int> gototable;
    vector<Item> itemset;

    void constructDFA();
};