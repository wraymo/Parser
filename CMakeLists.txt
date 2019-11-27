
cmake_minimum_required (VERSION 3.8)
project(Parser)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}) 
add_executable (Parser "src/Grammar.cpp" "src/LL1Parser.cpp" "src/LL1ParsingTable.cpp" "src/LR1Parser.cpp" "src/LR1ParsingTable.cpp" "src/main.cpp"
"inc/Grammar.h" "inc/LL1Parser.h" "inc/LL1ParsingTable.h" "inc/LR1Parser.h" "inc/LR1ParsingTable.h")


