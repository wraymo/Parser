#pragma once
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

using namespace std;

class Production {
public:
    char left;
    vector<string> right;
    Production(char _left, vector<string> _right) : left(_left), right(_right) {}
    Production(): left(-1) {}
};

//Assume that the grammar is context-free, each nonterminal is a single upper-case letter 
//and each terminal is a single character (not upper-case letter) 
class Grammar {
    friend class LL1ParsingTable;
    friend class LR1ParsingTable;
    friend class LL1Parser;
    friend class LR1Parser;
public:
    Grammar(string);

private:
    vector<Production> production;
    set<char> nonterminal;
    set<char> terminal;
    map<char, set<char>> first;
    map<char, set<char>> follow;
    char begin;

    void deleteLeftRecursion();
    void addSymbol(string&);
    void constructFirst();
    void constructFollow();
    void modifyGrammar();
    char findNew(char);
};