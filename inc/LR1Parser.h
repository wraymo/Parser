#pragma once
#include "Grammar.h"
#include "LR1ParsingTable.h"

class LR1Parser {
    Grammar* grammar;
    LR1ParsingTable table;

    LR1Parser(Grammar* _grammar) : grammar(_grammar), table(_grammar) {}
};