#include "../inc/LR1ParsingTable.h"
#include <iostream>

LR1ParsingTable::LR1ParsingTable(Grammar* _grammar) : grammar(_grammar) {
    //grammar->printGrammar();
    grammar->modifyGrammar();
    grammar->constructFirst();
    grammar->constructFollow();
    //grammar->printFirst();
    //grammar->printFollow();
    //grammar->printGrammar();
    prodmap.push_back({-1, ""});
    for (auto n : grammar->nonterminal) {
        for (auto exp : grammar->production[n])
            prodmap.push_back({ n, exp });
    }
    constructDFA();
    //printDFA();
    constructTable();
    //printTable();
}

void LR1ParsingTable::constructDFA() {
    char start = grammar->begin;
    set<char> followset;
    followset.insert('$');
    Production init(followset, { start, grammar->production[start][0] });
    Item temp = constructClosure(vector<Production>(1, init));
    itemset.push_back(temp);
    for (size_t i = 0; i < itemset.size(); i++) {
        bool* test = new bool[itemset[i].productionList.size()];
        memset(test, false, sizeof(bool) * itemset[i].productionList.size());
        for (size_t j = 0; j < itemset[i].productionList.size(); j++) {
            if (test[j] || //is tested or . in the last location
                itemset[i].productionList[j].index == itemset[i].productionList[j].production.second.size())
                continue;
            test[j] = true;
            vector<Production> list;
            Production nextprod = itemset[i].productionList[j];
            nextprod.index = itemset[i].productionList[j].index + 1;
            list.push_back(nextprod);
            for (size_t k = j + 1; k < itemset[i].productionList.size(); k++) {
                if (itemset[i].productionList[k].index != itemset[i].productionList[k].production.second.size()
                    && itemset[i].productionList[k].production.second[itemset[i].productionList[k].index]
                    == itemset[i].productionList[j].production.second[itemset[i].productionList[j].index]) {
                    Production next = itemset[i].productionList[k];
                    next.index = itemset[i].productionList[k].index + 1;
                    list.push_back(next);
                    test[k] = true;
                }
            }
            Item newItem = constructClosure(list);
            int ret;
            if ((ret = checkRepeation(newItem)) == -1) {
                newItem.ID = itemset.size();
                itemset.push_back(newItem);
                itemset[i].go[itemset[i].productionList[j].production.second[itemset[i].productionList[j].index]] = newItem.ID;
            }
            else {
                itemset[i].go[itemset[i].productionList[j].production.second[itemset[i].productionList[j].index]] = ret;        
            }
        }
    }
}

void LR1ParsingTable::constructTable() {
    for (auto item : itemset) {
        for (auto prod : item.productionList) {
            if (prod.index < prod.production.second.size()) {
                if (grammar->isNonterminal(prod.production.second[prod.index]))
                    gototable[{item.ID, prod.production.second[prod.index]}] = item.go[prod.production.second[prod.index]];
                else
                    actiontable[{item.ID, prod.production.second[prod.index]}] = item.go[prod.production.second[prod.index]];
            }
            else {
                for (auto follow : prod.follow)
                    actiontable[{item.ID, follow}] = pro2no(prod.production) * (-1);
            }
        }
    }
}

int LR1ParsingTable::pro2no(pair<char, string> prod) {
    for (size_t i = 0; i < prodmap.size(); i++)
        if (prodmap[i] == prod)
            return i;
}

void LR1ParsingTable::printTable() {
    for (size_t i = 0; i < itemset.size(); i++) {
        for (auto ch : grammar->terminal) {
            if (actiontable.find({ i, ch }) != actiontable.end())
                cout << "action[" << i << "," << ch << "]" << actiontable[{i, ch}] << endl;
        }
        if (actiontable.find({ i, '$' }) != actiontable.end())
            cout << "action[" << i << ",$]" << actiontable[{i, '$'}] << endl;
    }

    for (size_t i = 0; i < itemset.size(); i++) {
        for (auto ch : grammar->nonterminal) {
            if (actiontable.find({ i, ch }) != actiontable.end())
                cout << "goto[" << i << "," << ch << "]" << actiontable[{i, ch}] << endl;
        }
    }
}

void LR1ParsingTable::printDFA() {
    for (Item item : itemset) {
        cout << "I" << item.ID << endl;
        for (Production prod : item.productionList) {
            cout << prod.production.first << "->";
            size_t i;
            for (i = 0; i < prod.production.second.size(); i++) {
                if (i == prod.index)
                    cout << "¡¤";
                cout << prod.production.second[i];
            }
            if (i == prod.index)
                cout << "¡¤";
            cout << "  ";
            for (auto ch : prod.follow) {
                cout << ch << ",";
            }
            cout << endl;
        }
        for (auto i : item.go)
            cout << "go[" << item.ID << "," << i.first << "]=" << i.second << endl;
    }
}

bool operator == (Item& first, Item& second) {
    vector<Production> firstlist = first.productionList;
    vector<Production> secondlist = second.productionList;
    if (firstlist.size() != secondlist.size())
        return false;
    for (auto prod1 : firstlist) {
        bool flag = false;
        for (auto prod2 : secondlist) {
            if (prod1.follow == prod2.follow && prod1.index == prod2.index
                && prod1.production == prod2.production) {
                flag = true;
                break;
            }
        }
        if (!flag)
            return false;
    }
    return true;
}

int LR1ParsingTable::checkRepeation(Item& newItem) {
    for (size_t i = 0; i < itemset.size(); i++) {
        if (itemset[i] == newItem)
            return i;
    }
    return -1;
}

Item LR1ParsingTable::constructClosure(vector<Production> productionList) {
    for (size_t j = 0; j < productionList.size(); j++) {
        int current_index = productionList[j].index;
        string right = productionList[j].production.second;
        if (current_index != right.size() //. not in the last location
            && grammar->isNonterminal(right[current_index]) //is a nonterminal
            && grammar->production[right[current_index]].size() != 0) {
            auto iter = grammar->production[right[current_index]].begin();
            while (iter != grammar->production[right[current_index]].end()) {
                set<char> temp;
                int index = current_index + 1;
                while (index < right.size()) {
                    set<char> tempfirst = grammar->getFirst(right[index]);
                    temp.insert(tempfirst.begin(), tempfirst.end());
                    if (grammar->isTerminal(right[index]) || (grammar->isNonterminal(right[index]) &&
                        grammar->first[right[index]].find('#') == grammar->first[right[index]].end()))
                        break;
                    temp.erase('#');
                    index++;
                }
                if (index == right.size())
                    temp.insert(productionList[j].follow.begin(), productionList[j].follow.end());
                Production wait(temp, { right[current_index], *iter });
                int what;
                int ret = checkProd(productionList, wait, what);
                if (ret == 0) 
                    productionList.push_back(wait);
                else if (ret == 1) 
                    productionList[what].follow.insert(temp.begin(), temp.end());
                
                iter++;
            }
        }
    }
    return Item(productionList);
}

int LR1ParsingTable::checkProd(vector<Production> &productionList, Production& prod, int &index) {
    int ret = 0;
    for (size_t i = 0; i < productionList.size(); i++) {
        if (prod.follow == productionList[i].follow &&
            prod.index == productionList[i].index && prod.production == productionList[i].production) {
            index = -1;
            return -1;
        }
        else if (prod.index == productionList[i].index && prod.production == productionList[i].production) {
            ret = 1;
            index = i;
        }
    }
    return ret;
}

