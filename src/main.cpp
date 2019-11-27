#include "../inc/LL1Parser.h"
#include "../inc/LR1Parser.h"

int main() {
    //grammar1.txt arithmetic expression
    Grammar* grammar1_1 = new Grammar("grammar1.txt");
    LR1Parser LR1parser1(grammar1_1);
    LR1parser1.parse("7 - (2 + (4 * 6)) / 5");
    LR1parser1.parse("25.89 + (36.72 - (42.51 / 66.02)) * 58.29");

    Grammar* grammar1_2 = new Grammar("grammar1.txt");
    LL1Parser LL1parser1(grammar1_2);
    LL1parser1.parse("7 - (2 + (4 * 6)) / 5");
    LL1parser1.parse("25.89 + (36.72 - (42.51 / 66.02)) * 58.29");

    //grammar2.txt Grammar 4.6(P107)
    Grammar* grammar2 = new Grammar("grammar2.txt");
    LR1Parser LR1parser2(grammar2);
    LR1parser2.parse("accd");
    LR1parser2.parse("bccd");

    //grammar3.txt Grammar 4.7(P112)
    Grammar* grammar3 = new Grammar("grammar3.txt");
    LR1Parser LR1parser3(grammar3);
    LR1parser3.parse("5 =* 5");
    LR1parser3.parse("**7=8");

    //grammar4.txt Grammar 4.8(P113)
    Grammar* grammar4 = new Grammar("grammar4.txt");
    LR1Parser LR1parser4(grammar4);
    LR1parser4.parse("cccdd");
    LR1parser4.parse("dcd");

    //grammar5.txt Grammar 4.4(P84)
    Grammar* grammar5_1 = new Grammar("grammar5.txt");
    LR1Parser LR1parser5(grammar5_1);
    LR1parser5.parse("(5+2)*3");
    LR1parser5.parse("3*(6+2)");

    Grammar* grammar5_2 = new Grammar("grammar5.txt");
    LR1Parser LL1parser2(grammar5_2);
    LL1parser2.parse("(5+2)*3");
    LL1parser2.parse("3*(6+2)");

}