/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"
#include "evalstate.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include <cassert>
#include <string>

/* Implementation of the Statement class */


Statement::Statement() = default;

Statement::~Statement() = default;

StatementLet::StatementLet(std::string txt) {
  TokenScanner res(txt);
  res.ignoreWhitespace();
  lvalue = res.nextToken();
  lvalue = res.nextToken();
  lvalue = res.nextToken();
  txt = res.nextToken();
  rvalue = readE(res);
  return;
}
void StatementLet::execute(EvalState &lva, Program &program) {
  lva.setValue(lvalue, rvalue->eval(lva));
  return;
}

StatementLet::~StatementLet() { delete rvalue; }

StatementPrint::StatementPrint(std::string txt) {
  TokenScanner res;
  res.ignoreWhitespace();
  res.scanNumbers();
  res.setInput(txt);
  std::string res1 = res.nextToken();
  res1 = res.nextToken();
  res1 = res.nextToken();
  res.setInput(res1);
  exp = readE(res); 
  return;
}
StatementPrint::~StatementPrint() { delete exp; }
void StatementPrint::execute(EvalState &lva, Program &program) {
    std::cout << exp->eval(lva) << '\n';
  return;
}
void StatementRem::execute(EvalState &lva, Program &program) { return; }

StatementInput::StatementInput(std::string txt) {
  TokenScanner res(txt);
  res.ignoreWhitespace();
  lvalue = res.nextToken();
  lvalue = res.nextToken();
  lvalue = res.nextToken();
  return;
}
void StatementInput::execute(EvalState &lva, Program &program) {
  std::cout << " ? ";
  std::string str;
  std::getline(std::cin, str);
  lva.setValue(lvalue, stringToInteger(str));
  return;
}

void StatementEnd::execute(EvalState &lva, Program &program) {
  program.now_line = -1;
  return;
}
StatementGoto::StatementGoto(std::string txt) {
  TokenScanner res(txt);
  res.ignoreWhitespace();
  res.nextToken();
  res.nextToken();
  line_number = stringToInteger(res.nextToken());
  return;
}
void StatementGoto::execute(EvalState &lva, Program &program) {
  program.now_line = line_number;
  if(program.lines.find(line_number) == program.lines.end()) {
    error("LINE NUMBER ERROR");
  }
  return;
}

StatementIf::StatementIf(std::string txt) {
  TokenScanner res(txt);
  res.ignoreWhitespace();
  res.scanNumbers();
  std::string txt1;
  txt1 = res.nextToken();
  txt1 =res.nextToken();
  condition1 = readE(res,1);
  op="";
  while(  op != "=" && op != "<" && op != ">") {
    op = res.nextToken();
  }
  condition2 = readE(res);
  res.nextToken();
  line_number = stringToInteger(res.nextToken());
  return;
}
void StatementIf::execute(EvalState &lva, Program &program) {
  if (op == "=") {
    if (condition1->eval(lva) == condition2->eval(lva)) {
      program.now_line = line_number;
    }
  } else if (op == "<") {
    if (condition1->eval(lva) < condition2->eval(lva)) {
      program.now_line = line_number;
    }
  } else if (op == ">") {
    if (condition1->eval(lva) > condition2->eval(lva)) {
      program.now_line = line_number;
    }
  }
  return;
}
StatementIf::~StatementIf() {
  delete condition1;
  delete condition2;
} 