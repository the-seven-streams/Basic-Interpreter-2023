/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include <cctype>
#include <iostream>
#include <string>

/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */
Expression *ex = nullptr;
int main() {
  EvalState state;
  Program program;
  // cout << "Stub implementation of BASIC" << endl;
  while (true) {
    try {
      std::string input;
      getline(std::cin, input);
      if (input.empty())
        return 0;
      processLine(input, program, state);
    } catch (ErrorException &exc) {
      delete ex;
      std::cout << exc.getMessage() << std::endl;
    }
  }
  return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
  TokenScanner scanner;
  scanner.ignoreWhitespace();
  scanner.scanNumbers();
  scanner.setInput(line);
  std::string token = scanner.nextToken();
  bool is_number = 1;
  for (int i = 0; i < token.size(); i++) {
    if ((token[i] < '0') || (token[i] > '9')) {
      is_number = 0;
      break;
    }
  }
  if (is_number) {
    int res_num = stringToInteger(token);
    program.addSourceLine(res_num, line);
    return;
  }
  switch (token[0]) {
  case 'Q':
  {
  program.clear();
    exit(0);
    break;
  }
  case 'C':
    program.clear();
    state.Clear();
    break;
  case 'R':
    program.run(state);
    break;
  case 'L': {
    if(token[1] == 'E') {
        token = scanner.nextToken();
        if(token == "LET") {
          error("SYNTAX ERROR");
        }
        if(token == "REM") {
          error("SYNTAX ERROR");
        }
        if(token == "PRINT") {
          error("SYNTAX ERROR");
        }
        if(token == "INPUT") {
          error("SYNTAX ERROR");
        }
        if(token == "IF") {
          error("SYNTAX ERROR");
        }
        if(token == "THEN") {
          error("SYNTAX ERROR");
        }
        if(token == "END") {
          error("SYNTAX ERROR");
        }
        std::string res = scanner.nextToken();
        ex = readE(scanner);
        state.setValue(token, ex->eval(state));
        delete ex;
    } else {
        program.list();
    }
  } break;
  case 'P': {
    ex = readE(scanner);
    std::cout << ex->eval(state) << '\n';
    delete ex;
    break;
  }
  case 'I': {
    token = scanner.nextToken();
    std::cout << " ? ";
    int value;
    std::string res;
    TokenScanner Scanner2(res);
    bool flag = 1;
    while(flag)
    {
      flag = 0;
      getline(std::cin, res);
      for(int i = 0; i < res.size(); i++) {
        if(!(((res[i] >= '0') && (res[i] <= '9'))||((res[i] == '-')&&(i==0)))) {
          flag = 1;
          std::cout << "INVALID NUMBER" << '\n';
          std::cout << " ? "; 
          break;
        }
      }
      if(!flag) {
        Scanner2.setInput(res);
        Scanner2.ignoreWhitespace();
        Scanner2.scanNumbers();
        ex = readE(Scanner2);
      }
    }
    state.setValue(token, ex->eval(state));
    delete ex;
    break;
  }
  default:
    error("SYNTAX ERROR");
    break;
  }
  return;
}
