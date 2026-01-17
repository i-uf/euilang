#include "Interpreter.h"

#include "Utils.h"

using namespace std;

void Interpreter::run() {
    while (current().type != Token::End) {
        parseStatement();
        if(current().type == Token::NewLine) {
            yummy();
        } else if(current().type != Token::End) {
            error("syntax error: <line-separator> expected");
        }
    }
}

void Interpreter::parseStatement() {
    switch (current().type) {
        case Token::Var: case Token::Const:
            parseVariableDeclaration();
            break;
        case Token::If:
            parseIfStatement();
            break;
        case Token::Then:
            parseThenStatement();
            break;
        case Token::Else:
            parseElseStatement();
            break;
        case Token::While:
            parseWhileStatement();
            break;
        case Token::Continue:
            parseContinueStatement();
            break;
        case Token::Break:
            parseBreakStatement();
            break;
        case Token::Print:
            parsePrint();
            break;
        default:
            error("syntax error: unsupported statement: " + current().value);
    }
}

void Interpreter::parseVariableDeclaration() {
    bool variable = current().type == Token::Var;
    yummy();
    
    std::string name = parseIdentifier();
    Value val = parseExpression();

    if(find(consts.begin(), consts.end(), name) != consts.end())
        error("syntax error: cannot redefine variable: " + name + " is already declared as a const");
    vars[name] = val;
    if(!variable) consts.push_back(name);
}

void Interpreter::parsePrint() {
    yummy();
    Value val = parseExpression();
    cout << val.toString() << endl;
}

void Interpreter::parseIfStatement() {
    yummy();
    Value val = parseExpression();
    if(val.type != Value::Type::Boolean) error("syntax error: the value type must be Boolean");
    condition_enable = true;
    condition = val.b;
}

void Interpreter::parseThenStatement() {
    yummy();
    if(!condition_enable) error("syntax error: there was no 의?/의의? statememt before");
    if(condition) {parseStatement();}
    else while(current().type != Token::NewLine) yummy();
}

void Interpreter::parseElseStatement() {
    yummy();
    if(!condition_enable) error("syntax error: there was no 의?/의의? statememt before");
    if(!condition) parseStatement();
    else while(current().type != Token::NewLine) yummy();
}

void Interpreter::parseWhileStatement() {
    size_t copy = pc;
    yummy();
    Value val = parseExpression();
    if(val.type != Value::Type::Boolean) error("syntax error: the value type must be Boolean");
    condition_enable = true;
    condition = val.b;
    loopable = true;
    loop_pc = copy;
    loop_line = line;
}

void Interpreter::parseContinueStatement() {
    yummy();
    if(!loopable) error("syntax error: there was no 의의? statement before"); 
    pc = loop_pc;
    line = loop_line;
    parseStatement();
}

void Interpreter::parseBreakStatement() {
    yummy();
    if(!loopable) error("syntax error: there was no 의의? statement before"); 
    loopable = false;
    loop_pc = 0;
    loop_line = 0;
    condition = false;
}

Value Interpreter::parseExpression() {
    return parseOr();
}

Value Interpreter::parseOr() {
    return parse_biop(
        [this]{ return parseXor(); },
        [](Token t){ return t == Token::Or; }
    );
}
Value Interpreter::parseXor() {
    return parse_biop(
        [this]{ return parseAnd(); },
        [](Token t){ return t == Token::Xor; }
    );
}
Value Interpreter::parseAnd() {
    return parse_biop(
        [this]{ return parseComparison(); },
        [](Token t){ return t == Token::And; }
    );
}
Value Interpreter::parseComparison() {
    return parse_biop(
        [this]{ return parseTerm(); },
        [](Token t){ return t == Token::GreaterThan || t == Token::LessThan ||
            t == Token::GreaterOrEqualThan || t == Token::LessOrEqualThan ||
            t == Token::Is || t == Token::IsNot; }
    );
}
Value Interpreter::parseTerm() {
    return parse_biop(
        [this]{ return parseFactor(); },
        [](Token t){ return t == Token::Plus || t == Token::Minus; }
    );
}

Value Interpreter::parseFactor() {
    return parse_biop(
        [this]{ return parsePrimary(); },
        [](Token t){ return t == Token::Multiply ||
            t == Token::Divide || t == Token::Remainder; }
    );
}

Value Interpreter::parse_biop(
    function<Value()> nextParser,
    function<bool(Token)> ops
) {
    Value left = nextParser();

    TokenInfo curr;
    while (ops((curr = current()).type)) {
        yummy();
        Value right = nextParser();
        left = biop_ifbs(curr, left, right);
    }

    return left;
}


Value Interpreter::parsePrimary() {
    TokenInfo t = current();
    yummy();
    switch (t.type) {
        case Token::Not: case Token::Negate: {
            Value v = parsePrimary();
            return unop_ifbs(t, v);
        }
        case Token::Integer: {
            return Value(std::stoi(t.value));
        }
        case Token::Float: {
            return Value(std::stof(t.value));
        }
        case Token::Boolean: {
            return Value(t.value == "읭");
        }
        case Token::String: {
            return Value(t.value);
        }
        case Token::Identifier: {
            if (vars.find(t.value) != vars.end())
                return vars[t.value];
            throw std::runtime_error("unknown variable: " + t.value);
        }
        default:
            throw std::runtime_error("syntax error: needs a value");
    }
}

std::string Interpreter::parseIdentifier() {
    string identifierName = current().value;
    eat(Token::Identifier, "syntax error: <identifier> expected");
    return identifierName;
}

void Interpreter::eat(Token token, string message) {
    if(current().type != token) error(message);
    if(current().type == Token::NewLine) line=stoi(current().value);
    pc++;
}

void Interpreter::yummy() {
    if(current().type == Token::NewLine) line=stoi(current().value);
    pc++;
}

Value Interpreter::unop_ifbs(TokenInfo expr, Value a) {
    switch (expr.type) {
        case Token::Not:
            if(a.type != Value::Type::Boolean)
                error("unsupported operation: cannot solve '" + expr.value + " <" + a.typeToString() + ">'");
            return Value(!a.b);
        case Token::Negate:
            switch(a.type) {
                case Value::Type::Boolean: case Value::Type::String:
                    error("unsupported operation: cannot solve '" + expr.value + " <" + a.typeToString() + ">'");
                case Value::Type::Int:
                    return Value(-a.i);
                case Value::Type::Float:
                    return Value(-a.f);
            }
        default:
            error("unsupported operation: cannot solve '" + expr.value + " <" + a.typeToString() + ">'");
    }
}

Value Interpreter::biop_ifbs(TokenInfo expr, Value a, Value b) {
    bool same = a.type == b.type;
    bool str = a.type == Value::Type::String || b.type == Value::Type::String;
    bool num = (a.type == Value::Type::Int || a.type == Value::Type::Float)
        && (b.type == Value::Type::Int || b.type == Value::Type::Float);
    
    if(same && a.type == Value::Type::Int) {
        switch (expr.type) {
            case Token::Plus: return Value(a.i + b.i);
            case Token::Minus: return Value(a.i - b.i);
            case Token::Multiply: return Value(a.i * b.i);
            case Token::Divide: return Value(a.i / b.i);
            case Token::Remainder: return Value(a.i % b.i);
            case Token::GreaterThan: return Value(a.i > b.i);
            case Token::GreaterOrEqualThan: return Value(a.i >= b.i);
            case Token::LessThan: return Value(a.i < b.i);
            case Token::LessOrEqualThan: return Value(a.i <= b.i);
            case Token::And: return Value(a.i | b.i);
            case Token::Or: return Value(a.i & b.i);
            case Token::Xor: return Value(a.i ^ b.i);
            case Token::Is: return Value(a.i == b.i);
            case Token::IsNot: return Value(a.i != b.i);
        }
    }
    
    if(num) {
        float af = (a.type == Value::Type::Float ? a.f : a.i);
        float bf = (b.type == Value::Type::Float ? b.f : b.i);
        switch (expr.type) {
            case Token::Plus: return Value(af + bf);
            case Token::Minus: return Value(af - bf);
            case Token::Multiply: return Value(af * bf);
            case Token::Divide: return Value(af / bf);
            case Token::GreaterThan: return Value(af > bf);
            case Token::GreaterOrEqualThan: return Value(af >= bf);
            case Token::LessThan: return Value(af < bf);
            case Token::LessOrEqualThan: return Value(af <= bf);
            case Token::Is: return Value(af == bf);
            case Token::IsNot: return Value(af != bf);
        }
    }

    if(same && a.type == Value::Type::Boolean) {
        switch (expr.type) {
            case Token::And: return Value(a.b && b.b);
            case Token::Or: return Value(a.b || b.b);
            case Token::Xor: return Value(a.b ^ b.b);
            case Token::Is: return Value(a.b == b.b);
            case Token::IsNot: return Value(a.b != b.b);
        }
    }

    if(same && a.type == Value::Type::String) {
        switch (expr.type) {
            case Token::Plus: return Value(a.s + b.s);
            case Token::Is: return Value(a.s == b.s);
            case Token::IsNot: return Value(a.s != b.s);
        }
    }

    error("unsupported operation: cannot solve '<" + a.typeToString() + "> " + expr.value + " <" + b.typeToString() + ">'");
}