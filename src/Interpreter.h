#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <functional>

#include "Lexer.h"
#include "Token.h"
#include "Values.h"

class Interpreter {
    public:

    explicit Interpreter(Lexer *lexer) : lexer(lexer) {
        while (true) {
            TokenInfo token = lexer->next_token();
            tokens.push_back(token);
            if (token.type == Token::End) break;
        }
    }

    Lexer *lexer;

    size_t line = 1;

    void run();

    private:

    std::vector<TokenInfo> tokens;

    size_t pc = 0;

    bool condition_enable = false;
    bool condition = false;
    
    bool loopable = false;
    size_t loop_pc = 0;
    size_t loop_line = 0;

    std::vector<std::string> consts;
    std::map<std::string, Value> vars;

    void parseStatement();
    
    void parsePrint();

    void parseVariableDeclaration();
    void parseFunctionDeclaration();

    void parseIfStatement();
    void parseThenStatement();
    void parseElseStatement();
    void parseWhileStatement();
    void parseContinueStatement();
    void parseBreakStatement();

    Value parseExpression();
    Value parseOr();
    Value parseXor();
    Value parseAnd();
    Value parseComparison();
    Value parseTerm();
    Value parseFactor();
    Value parsePrimary();
    Value parse_biop(
        std::function<Value()> nextParser,
        std::function<bool(Token)> ops
    );

    Value biop_ifbs(TokenInfo expr, Value a, Value b);
    Value unop_ifbs(TokenInfo expr, Value a);

    std::string parseIdentifier();

    void eat(Token token, std::string message);
    void yummy();

    TokenInfo current() { return tokens[pc]; }
};

#endif