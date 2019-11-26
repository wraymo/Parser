#pragma once
#include "Grammar.h"
#include "LL1ParsingTable.h"
#include <stack>

class LL1Parser {
public:
    LL1ParsingTable table;
    Grammar* grammar;
    stack<char> S;

    LL1Parser(Grammar* _grammar): grammar(_grammar), table(_grammar), debug(true){}
    void parse(string);

    void closedebug() {
        debug = false;
    }

    void opendebug() {
        debug = true;
    }

    void constructTable() {
        table.constructTable();
    }
private:
    bool debug;
};