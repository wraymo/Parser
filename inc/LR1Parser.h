#pragma once
#include "Grammar.h"
#include "LR1ParsingTable.h"

class LR1Parser {
    Grammar* grammar;
    LR1ParsingTable table;
};