#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cstring>
#include <string>

#include "Lexer.h"
#include "Interpreter.h"
#include "Utils.h"

using namespace std;

const string version = "0.1.1";

void wrong_usage() { cerr << "usage: eui <fileName>" << endl; }

const char *b2c (vector<uint8_t> data) {
    return reinterpret_cast<const char*>(data.data());
}
int main(int argc, char *argv[]) {
    if(argc == 1) {
        cout << "EUI Lang v" << version << endl;
        cout << "usage: eui <filename>" << endl;
    } else  if(argc == 2) {
        if (!filesystem::exists(argv[1]) || filesystem::is_directory(argv[1])) {
            cerr << "error: invalid file " << argv[1] << endl;
            return 1;
        }
        ifstream input(argv[1], ios::binary);

        if(!input) {
            cerr << "error: unable to read file " << argv[1] << endl;
            return 1;
        }
        string text;
        string line;
        vector<string> lines;
        
        while(getline(input, line)) { text += line + '\n'; lines.push_back(line); }
        input.close();

        if(!endsWith(argv[1], ".eui")) {
            cerr << "error: .eui file extension expected"<< endl;
            return 1;
        }

        Lexer *lexer = new Lexer(text);
        Interpreter interpreter(lexer);
        try {
            interpreter.run();
        } catch(const exception &e) {
            cerr << argv[1] << ':' << interpreter.line << ": " << e.what() << endl;
            return 1;
        }
        delete lexer;
    } else {
        wrong_usage();
        return 1;
    }
    return 0;
}
