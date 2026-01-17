#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class Token {
    Invalid, NewLine,
    Integer, Float, Boolean, String, Identifier,
    Assign, Plus, Minus, Multiply, Divide, Remainder, Negate, Not, And, Or, Xor,
    Comma, Colon, GreaterThan, LessThan, GreaterOrEqualThan, LessOrEqualThan,
    Var, Const, Fun,
    If, Then, Else, While, Continue, Break,
    Is, IsNot,
    Print,
    End
};
struct TokenInfo {
    Token type;
    std::string value;
};

#endif