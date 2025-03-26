#include "Parser.h"
#include "tests.cpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << "--command [expression] [var=value ...]" << endl;
        return 1;
    }

    string command = argv[1];
    string input = argv[2];

    Parser<double> parser(input);
    if (command == "--eval") {
        if (argc <= 2) {
            cerr << "Usage: " << argv[0] << "--command [expression] [var=value ...]" << endl;
            return 1;
        }
        try {
            map<string, double> variables;
            for (int i = 3; i < argc; ++i) {
                string arg = argv[i];
                int pos = arg.find('=');
                if (pos != -1) {
                    string var = arg.substr(0, pos);
                    double val = stod(arg.substr(pos + 1));
                    variables[var] = val;
                }
            }
            auto expr = parser.parse();
            cout << "Result: " << expr->apply(variables) << endl;
            // auto derivative = expr->differentiate("x");
            // cout << "Derivative with respect to x: " << derivative->evaluate(variables) << endl;
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }



    return 0;
}
