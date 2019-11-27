#pragma once
#include "Grammar.h"
#include "LR1ParsingTable.h"
#include <stack>
class LR1Parser {
public:
    LR1Parser(Grammar* _grammar) : grammar(_grammar), table(_grammar) {}
    void parse(string);

private:
    Grammar* grammar;
    LR1ParsingTable table;
    stack<int> state;
    stack<char> symbol;
    bool debug;
};