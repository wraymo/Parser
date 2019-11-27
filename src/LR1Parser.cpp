#include "../inc/LR1Parser.h"
#include <iostream>

void LR1Parser::parse(string expression) {
    expression += '$';
    while (!state.empty())
        state.pop();
    while (!symbol.empty())
        symbol.pop();
    state.push(0);
    symbol.push('$');
    size_t index;
    char second;
    int i = 0;
    while (true) {
        while (isblank(expression[i]))
            i++;
        if (isdigit(expression[i])) {
            stof(expression.substr(i), &index);
            second = 'n';
        }
        else {
            index = 0;
            second = expression[i];
        }

        if (table.actiontable.find({ state.top(), second }) != table.actiontable.end()) {
            if (table.actiontable[{ state.top(), second }] >= 0) {
                if (index == 0)
                    i++;
                else
                    i += index;
                state.push(table.actiontable[{ state.top(), second }]);
                symbol.push(second);
            }
            else {
                int statetop = state.top();
                int size = table.prodmap[-1 * table.actiontable[{ statetop, second }]].second.size();
                cout << table.prodmap[-1 * table.actiontable[{ statetop, second }]].first << "->" <<
                    table.prodmap[-1 * table.actiontable[{ statetop, second }]].second << endl;
                while (size--) {
                    state.pop();
                    symbol.pop();
                }
                symbol.push(table.prodmap[-1 * table.actiontable[{ statetop, second }]].first);
                int currentstate = state.top();
                state.push(table.gototable[{currentstate, symbol.top()}]);
                if (symbol.top() == grammar->begin) {
                    cout << "Right Expression" << endl;
                    return;
                }
            }
        }
        else {
            cerr << "Wrong expression!" << endl;
            return;
        }
    }
}