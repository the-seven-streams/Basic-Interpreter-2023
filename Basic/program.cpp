/*
 * File: program.cpp
 * -----------------
 * This file is a sub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "Utils/tokenScanner.hpp"
#include "statement.hpp"
#include <string>
#include <assert.h>

Program::Program() = default;

Program::~Program() {
    clear();
    return;
}

void Program::clear() {
    lines.clear();
    for(auto it = parsedlines.begin(); it != parsedlines.end(); ++it) {
        delete it->second;
    }
    parsedlines.clear();
    return;
}

void Program::addSourceLine(int line_number, const std::string &line) {
    TokenScanner txt(line);
    txt.ignoreWhitespace();
    std::string  res = txt.nextToken();
    if(txt.hasMoreTokens()) {
        res = txt.nextToken();
        lines[line_number] = line;
        if(parsedlines.find(line_number) != parsedlines.end()) {
            delete parsedlines[line_number];
        }
        switch(res[0]) {
            case 'R':
            {
                setParsedStatement(line_number, new StatementRem);
                break;
            }
            case 'L':
            {
                setParsedStatement(line_number, new StatementLet(line));
                break;
            }
            case 'P':
            {
                setParsedStatement(line_number, new StatementPrint(line));
                break;
            }
            case 'I':
            {
                if(res[1] =='N') {
                    setParsedStatement(line_number, new StatementInput(line));
                } else {
                    setParsedStatement(line_number, new StatementIf(line));
                }
                break;
            }
            case 'E':
            {
                setParsedStatement(line_number, new StatementEnd);
                break;
            }
            case 'G':
            {
                setParsedStatement(line_number, new StatementGoto(line));
                break;
            }
            
        }
    } else {
        delete parsedlines[line_number];
        lines.erase(line_number);
        parsedlines.erase(line_number);
    }
    return;
}

void Program::removeSourceLine(int line_number) {
    lines.erase(line_number);
    return;
}

std::string Program::getSourceLine(int line_number) {
    return lines[line_number];
}

void Program::setParsedStatement(int line_number, Statement *stmt) {
    parsedlines[line_number] = stmt;
    return;

}

Statement *Program::getParsedStatement(int line_number) {
   return parsedlines[line_number];
}

int Program::getFirstLineNumber() {
    return lines.begin()->first;
}

int Program::getNextLineNumber(int line_number) {
    if(++lines.find(line_number) == lines.end()) return -1;
    return (++lines.find(line_number))->first;
}

void Program::run(EvalState &state) {
    now_line = getFirstLineNumber();
    while((now_line != -1 ) && (now_line <= lines.rbegin()->first) ) {
        int this_line = now_line;
        parsedlines[now_line]->execute(state, *this);
        if(this_line == now_line) {
            now_line = getNextLineNumber(now_line);
        }
    }
    return;
}
void Program::list() {
    for(auto it = lines.begin(); it != lines.end(); ++it) {
        std::cout << it->second << std::endl;
    }
    return;
}
//more func to add
//todo


