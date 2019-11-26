#pragma once
#include <fstream>
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
};

//Assume that the grammar is context-free, each nonterminal is a single upper-case letter 
//and each terminal is a single character 
class Grammar {
public:
	vector<Production> production;
	set<char> nonterminal;
	set<char> terminal;
	map<char, set<char>> first;
	map<char, set<char>> follow;
	char begin;

	Grammar(string);
	void addSymbol(string &);
	void deleteLeftRecursion();
	void constructFirst();
	void constructFollow();
	char findNew(char);

};