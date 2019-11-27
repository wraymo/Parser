#pragma once
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

using namespace std;

//Assume that the grammar is context-free, each nonterminal is a single upper-case letter 
//and each terminal is a single character (not upper-case letter) 
class Grammar {
    friend class LL1ParsingTable;
    friend class LR1ParsingTable;
    friend class LL1Parser;
    friend class LR1Parser;
public:
    Grammar(string);
    void printFirst();
    void printFollow();
    void printGrammar();

private:
    map<char, vector<string>> production;
    set<char> nonterminal;
    set<char> terminal;
    map<char, set<char>> first;
    map<char, set<char>> follow;
    char begin;

    set<char> getFirst(char);
    bool isTerminal(char);
    bool isNonterminal(char);
    void deleteLeftRecursion();
    void addSymbol(string&);
    void constructFirst();
    void constructFollow();
    void modifyGrammar();
    char findNew(char);
};