#include "../inc/LL1ParsingTable.h"
#include <iostream>

LL1ParsingTable::LL1ParsingTable(Grammar* _grammar) : grammar(_grammar) {
    grammar->deleteLeftRecursion();
    grammar->constructFirst();
    grammar->constructFollow();
    for (const auto i : grammar->nonterminal)
        for (const auto j : grammar->terminal)
            if (j != '#')
                parsingtable[{i, j}] = vector<string>();
    for (const auto i : grammar->nonterminal)
        parsingtable[{i, '$'}] = vector<string>();
    constructTable();
}

void LL1ParsingTable::constructTable() {
    for (const auto& _production : grammar->production) {
        for (const auto& expression : _production.second) {
            if (expression[0] != '#' && grammar->isTerminal(expression[0]))
                parsingtable[{_production.first, expression[0]}].push_back(expression);
            else {
                for (auto ch : grammar->first[expression[0]]) 
                    if (ch != '#') 
                        parsingtable[{_production.first, ch}].push_back(expression);
            }

            if (expression[0] == '#' || (grammar->isNonterminal(expression[0])
                && grammar->first[expression[0]].find('#') != grammar->first[expression[0]].end())) {
                for (auto ch : grammar->follow[_production.first]) {
                    parsingtable[{_production.first, ch}].push_back(expression);
                }
            }
        }
    }
    checkTable();
}

void::LL1ParsingTable::checkTable() {
    for (const auto i : grammar->nonterminal)
        for (const auto j : grammar->terminal)
            if (j != '#' && parsingtable[{i, j}].size() > 1)
                throw "wrong table!";
}