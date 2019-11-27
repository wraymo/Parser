#pragma once
#include "Grammar.h"

class Production {
public:
    int index;
    set<char> follow;
    pair<char, string> production;
    Production(set<char> _follow, pair<char, string> _production, int _index = 0):
        index(_index), follow(_follow), production(_production){}
    Production() : index(0) {}
};

class Item {
public:
    int ID;
    vector<Production> productionList;
    map<char, int> go;
    Item(vector<Production> _productionList, int _ID = 0) :ID(_ID), productionList(_productionList){}
};

class LR1ParsingTable {
    friend class LR1Parser;
public:
    LR1ParsingTable(Grammar*);
    void printDFA();
    void printTable();

private:
    Grammar* grammar;
    map<pair<int, char>, int> actiontable;
    map<pair<int, char>, int> gototable;
    vector<pair<char, string>> prodmap;
    vector<Item> itemset;
    Item constructClosure(vector<Production>);
    int checkRepeation(Item&);
    int checkProd(vector<Production>&, Production&, int&);
    void constructDFA();
    void constructTable();
    int pro2no(pair<char, string>);
};