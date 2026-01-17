#include "Lexer.h"

#include <string>
#include <iostream>
#include "Token.h"

using namespace std; 

TokenInfo Lexer::next_token() {
    bool ln = false;
    while (pos < input.size() && (isspace(input[pos]))) {
        if(input[pos] == '\n') { ln = true; line++;}
        pos++;
    }
    if(ln) return {Token::NewLine, to_string(line)};
    if (pos == input.size()) return {Token::End, ""};
    char current = input[pos];

    if (!isdigit(current)) {
        string identifier;
        while (pos < input.size() && !isspace(input[pos])) {
            identifier += input[pos++];
        }
        
        if (identifier == "의")   return {Token::Is,        "의"  };
        if (identifier == "의!!") return {Token::Const,     "의!!"};
        if (identifier == "의!")  return {Token::Var,       "의!" };
        if (identifier == "의:")  return {Token::Then,      "의:" };
        if (identifier == "의?")  return {Token::If,        "의?" };

        if (identifier == "늬")   return {Token::IsNot,     "늬"  };
        if (identifier == "늬:")  return {Token::Else,      "늬:" };
        
        if (identifier == "의의?")return {Token::While,     "의의?"};
        if (identifier == "계의") return {Token::Continue,  "계의" };
        if (identifier == "종의") return {Token::Break,     "종의" };

        if (identifier == "덧의") return {Token::Plus,      "덧의" };
        if (identifier == "차의") return {Token::Minus,     "차의" };
        if (identifier == "곱의") return {Token::Multiply,  "곱의" };
        if (identifier == "분의") return {Token::Divide,    "분의" };
        if (identifier == "남의") return {Token::Remainder, "남의" };

        if (identifier == "합의") return {Token::Or,        "합의" };
        if (identifier == "교의") return {Token::And,       "교의" };
        if (identifier == "늬의") return {Token::Xor,       "늬의" };
        if (identifier == "부의") return {Token::Not,       "부의" };
        if (identifier == "반의") return {Token::Negate,    "반의" };

        if (identifier == "대의") return {Token::GreaterThan,"대의" };
        if (identifier == "소의") return {Token::LessThan,   "소의" };
        if (identifier == "대의의") return {Token::GreaterOrEqualThan,"대의의" };
        if (identifier == "소의의") return {Token::LessOrEqualThan,   "소의의" };

        if (identifier == "표의") return {Token::Print,     "표의" };

        if (identifier == "읭")   return {Token::Boolean,   "읭"  };
        if (identifier == "닁")   return {Token::Boolean,   "닁"  };
        
        return {Token::Identifier, identifier};
    } else {
        string num;
        while (pos < input.size() && isdigit(input[pos])) {
            num += input[pos++];
        }
        if(pos + 1 < input.size() && input[pos] == '.' && isdigit(input[pos+1])) {
            num += input[pos++];
            while (pos < input.size() && isdigit(input[pos])) {
                num += input[pos++];
            }
            return {Token::Float, num};
        }
        return {Token::Integer, num};
    }
    pos++;

    return {Token::Invalid, string(1, current)};
}