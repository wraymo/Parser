#include "../inc/Grammar.h"
#include <iostream>
#include <ctype.h>
#include <memory.h>
#include <iterator>

Grammar::Grammar(string filename) {
	ifstream file(filename, ios::in);
	if (!file) {
		cerr << "Error opening " << filename << endl;
		exit(1);
	}
	string templine;
	while (getline(file, templine)) {
		templine.erase(0, templine.find_first_not_of(" "));
		templine.erase(templine.find_last_not_of(" ") + 1);
		char start = templine[0];
		nonterminal.insert(start);
		size_t findarrow = templine.find("->");
		size_t findbar;
		string expression;
		vector<string> templist;
		templine = templine.substr(findarrow + 2);
		while ((findbar = templine.find("|")) != string::npos) {
			expression = templine.substr(0, findbar);
			expression.erase(0, expression.find_first_not_of(" "));
			expression.erase(expression.find_last_not_of(" ") + 1);
			addSymbol(expression);
			templist.push_back(expression);
			templine = templine.substr(findbar + 1);
		}
		templine.erase(0, templine.find_first_not_of(" "));
		templine.erase(templine.find_last_not_of(" ") + 1);
		addSymbol(templine);
		templist.push_back(templine);
		production.push_back(Production(start, templist));
	}
	begin = production[0].left;

	for (const auto& i : nonterminal) {
		first[i] = set<char>();
		follow[i] = set<char>();
	}

	terminal.insert('#');
}

void Grammar::addSymbol(string & expression) {
	size_t findblank;
	while ((findblank = expression.find_first_of(" ")) != string::npos)
		expression.erase(findblank, 1);
	for (auto c : expression) 
		if (!isblank(c) && !isupper(c))
			terminal.insert(c);
}

void Grammar::deleteLeftRecursion() {
	size_t size = production.size();
	for (int i = 0; i < size; i++) {
		vector<string> temp;
		bool flag = false;
		char newnonterminal = 0;
		for (int j = 0; j < production[i].right.size(); ) {
			if (production[i].right[j][0] == production[i].left) {
				if (!flag) 
					newnonterminal = findNew(production[i].left);
				flag = true;
				temp.push_back(production[i].right[j].substr(1) + newnonterminal);
				production[i].right.erase(production[i].right.begin() + j);
				continue;
			}
			j++;
		}
		if (flag) {
			for (auto &k : production[i].right)
				k += newnonterminal;
			temp.push_back("#");
			nonterminal.insert(newnonterminal);
			production.push_back(Production(newnonterminal, temp));
		}
	}
}

char Grammar::findNew(char old) {
	do {
		if (old == 'Z')
			old = 'A';
		else
			old++;
	} while (nonterminal.find(old) != nonterminal.end());
	return old;
}

void Grammar::constructFirst() {
	for (const auto & _production: production) {
		for (const auto & expression : _production.right) {
			if (terminal.find(expression[0]) != terminal.end()) 
				first[_production.left].insert(expression[0]);
			else {  //the first character is nonterminal
				
			}
		}
	}
}

void Grammar::constructFollow()
{

}

int main() {
	string path = "grammar.txt";
	Grammar grammar(path);	
	grammar.deleteLeftRecursion();

}