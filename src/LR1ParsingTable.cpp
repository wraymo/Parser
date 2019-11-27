#include "../inc/LR1ParsingTable.h"

LR1ParsingTable::LR1ParsingTable(Grammar* _grammar) : grammar(_grammar) {
    grammar->modifyGrammar();
    grammar->constructFirst();
    grammar->constructFollow();
}

void LR1ParsingTable::constructDFA() {

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

}