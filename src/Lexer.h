#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>
#include "Token.h"

class Lexer {
    public: Lexer(const std::string &input) : input(input) { }
    const std::string &input; int pos = 0, line = 1;
    TokenInfo next_token();
};

#endif