#pragma once
#include "Grammar.h"
#include <utility>

class LL1ParsingTable {
public:
	Grammar* grammar;
	map<pair<char, char>, Production> parsingtable;
	LL1ParsingTable(Grammar* _grammar): grammar(_grammar)
	{
		for (const auto i : grammar->nonterminal)
			for (const auto j : grammar->terminal)
				if(j != '#')
					parsingtable[make_pair(i, j)] = Production();
		for(const auto i : grammar->nonterminal)
			parsingtable[make_pair(i, '$')] = Production();
	}
	void constructTable();
	void checkTable();
};