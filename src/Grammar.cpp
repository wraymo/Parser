#include "../inc/Grammar.h"
#include "../inc/LL1Parser.h"
#include <iostream>
#include <fstream>
#include <ctype.h>
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
    map<char, set<char>> last = first;
    for (const auto & _production: production) {
        for (const auto & expression : _production.right) {
            if (terminal.find(expression[0]) != terminal.end()) 
                first[_production.left].insert(expression[0]);
            else {  //the first character is a nonterminal
                for (auto ch : expression) {
                    if (nonterminal.find(ch) != nonterminal.end()) {
                        for (const auto c : first[ch])
                            if (c != '#')
                                first[_production.left].insert(c);
                        if (first[ch].find('#') == first[ch].end())
                            break;
                    }
                    else if (terminal.find(ch) != terminal.end()) {
                        first[_production.left].insert(ch);
                        break;
                    }
                    else
                        break;
                }
            }
        }
    }
    if (last != first)
        constructFirst();
}

bool operator != (map<char, set<char>>& first, map<char, set<char>>& second) {
    for (const auto ch : first)
        if (first[ch.first].size() != second[ch.first].size())
            return true;
    return false;
}

void Grammar::constructFollow() {
    map<char, set<char>> last = follow;
    follow[begin].insert('$');
    for (const auto& _production : production) {
        for (const auto& expression : _production.right) {
            size_t size = expression.size();
            for (int i = 0; i < size; i++) {
                if (nonterminal.find(expression[i]) != nonterminal.end()) {
                    if (i == size - 1 || (nonterminal.find(expression[i + 1]) != nonterminal.end()
                        && first[expression[i + 1]].find('#') != first[expression[i + 1]].end()))
                        for (const auto ch : follow[_production.left])
                            follow[expression[i]].insert(ch);
                    if (terminal.find(expression[i + 1]) != terminal.end())
                        follow[expression[i]].insert(expression[i + 1]);
                    else {
                        for (const auto ch : first[expression[i + 1]])
                            if (ch != '#')
                                follow[expression[i]].insert(ch);
                    }
                }
            }
        }
    }
    if (last != follow)
        constructFollow();
}

int main() {
    string path = "grammar.txt";
    Grammar * grammar = new Grammar(path);	
    grammar->deleteLeftRecursion();
    grammar->constructFirst();
    grammar->constructFollow();
    LL1Parser LL1(grammar);
    LL1.constructTable();
    LL1.parse("6 - (2 + (8 * 9) / 6)");/*
    for(auto i: table.grammar->nonterminal)
        for (auto j : table.grammar->terminal)
        {
            if (table.parsingtable[make_pair(i, j)].left != -1) {
                cout << "M[" << i << "," << j << "]=" << table.parsingtable[make_pair(i, j)].left;
                cout << "->" << table.parsingtable[make_pair(i, j)].right[0] << endl;
            }
        }
    for (auto i : table.grammar->nonterminal)
    {
        char j = '$';
        if (table.parsingtable[make_pair(i, j)].left != -1) {
            cout << "M[" << i << "," << j << "]=" << table.parsingtable[make_pair(i, j)].left;
            cout << "->" << table.parsingtable[make_pair(i, j)].right[0] << endl;
        }
    }*/
    /*
    for (auto i: grammar.production)
    {
        cout << i.left << "->";
        for (auto j : i.right)
            cout << j << "   ";
        cout << endl;
    }
    for (auto c : grammar.first) {
        cout << c.first << ":";
        for (auto b : c.second)
            cout << b << " ";
        cout << endl;
    }
    cout << endl << endl;
    for (auto c : grammar.follow) {
        cout << c.first << ":";
        for (auto b : c.second)
            cout << b << " ";
        cout << endl;
    }*/

}