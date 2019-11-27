#include "../inc/LL1Parser.h"
#include <iostream>

void LL1Parser::parse(string expression) {
    expression += '$';
    while (!S.empty())
        S.pop();
    S.push('$');
    S.push(grammar->begin);
    size_t index;
    char second;
    int i = 0;
    while (S.top() != '$') {
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
        
        if (grammar->isTerminal(S.top())) {
            if (S.top() == second) {
                S.pop();
                if (index == 0)
                    i++;
                else
                    i += index;
            }
            else {
                cerr << "Wrong expression(type: 1)!" << endl;
                return;
            }
        }

        else if (table.parsingtable[{S.top(), second}].size() > 0) {
            char top = S.top();
            S.pop();
            for (int i = table.parsingtable[{top, second}][0].size() - 1; i >= 0; i--)
                if (table.parsingtable[{top, second}][0][i] != '#')
                    S.push(table.parsingtable[{top, second}][0][i]);
            if (debug)
                cout << top << "->" << table.parsingtable[{top, second}][0] << endl;
        }
        else {
            cerr << "Wrong expression(type: 2)!" << endl;
            return;
        }
    }
    if (S.top() == '$' && expression[i] == '$')
        cerr << "Right Expression!" << endl;
    else
        cerr << "Wrong expression(type: 3)!" << endl;
}