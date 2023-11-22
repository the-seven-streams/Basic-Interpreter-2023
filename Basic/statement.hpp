/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "evalstate.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include <sstream>
#include <string>

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {
public:
  /*
   * Constructor: Statement
   * ----------------------
   * The base class constructor is empty.  Each subclass must provide
   * its own constructor.
   */

  Statement();

  /*
   * Destructor: ~Statement
   * Usage: delete stmt;
   * -------------------
   * The destructor deallocates the storage for this expression.
   * It must be declared virtual to ensure that the correct subclass
   * destructor is called when deleting a statement.
   */

  virtual ~Statement();

  /*
   * Method: execute
   * Usage: stmt->execute(state);
   * ----------------------------
   * This method executes a BASIC statement.  Each of the subclasses
   * defines its own execute method that implements the necessary
   * operations.  As was true for the expression evaluator, this
   * method takes an EvalState object for looking up variables or
   * controlling the operation of the interpreter.
   */

  virtual void execute(EvalState &state, Program &program) = 0;
};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */
class StatementRem : public Statement {
public:
  void execute(EvalState &, Program &);
};
class StatementLet : public Statement {
private:
  std::string lvalue;
  Expression *rvalue;

public:
  ~StatementLet();
  StatementLet(std::string);
  void execute(EvalState &, Program &);
};
class StatementPrint : public Statement {
private:
  Expression *exp;
public:
  StatementPrint(std::string);
  ~StatementPrint();
  void execute(EvalState &, Program &);
};
class StatementInput : public Statement {
public:
  StatementInput(std::string);
  void execute(EvalState &, Program &);

private:
  std::string lvalue;
};
class StatementEnd : public Statement {
public:
  void execute(EvalState &, Program &);
};
class StatementGoto : public Statement {
private:
  int line_number;

public:
  StatementGoto(std::string);
  void execute(EvalState &, Program &);
};

class StatementIf : public Statement {
private:
  Expression *condition1, *condition2;
  std::string op;
  int line_number;

public:
  ~StatementIf();
  StatementIf(std::string);
  void execute(EvalState &, Program &);
};
#endif
