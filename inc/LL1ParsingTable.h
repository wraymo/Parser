#pragma once
#include "Grammar.h"
#include <utility>

class LL1ParsingTable {
    friend class LL1Parser;
public:
    LL1ParsingTable(Grammar*);
private:
    Grammar* grammar;
    map<pair<char, char>, vector<string> > parsingtable;
    void constructTable();
    void checkTable();
};