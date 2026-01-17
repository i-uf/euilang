#ifndef VALUES_H
#define VALUES_H

#include <string>

struct Value {
    enum class Type {
        Int, Float,
        Boolean, String,
        None
    } type;

    int i;
    float f;
    bool b;
    std::string s;

    Value(int v) : type(Type::Int), i(v) {}
    Value(float v) : type(Type::Float), f(v) {}
    Value(bool v) : type(Type::Boolean), b(v) {}
    Value(const std::string& v) : type(Type::String), s(v) {}
    Value() : type(Type::None) {}

    std::string toString() {
        switch (type) {
            case Type::Int: return std::to_string(i);
            case Type::Float: return std::to_string(f);
            case Type::Boolean: return b ? "true" : "false";
            case Type::String: return s;
        }
        return "null";
    }

    std::string typeToString() {
        switch (type) {
            case Type::Int: return "int";
            case Type::Float: return "float";
            case Type::Boolean: return "boolean";
            case Type::String: return "string";
        }
        return "null";
    }
};

#endif