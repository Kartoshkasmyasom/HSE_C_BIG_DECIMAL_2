#include "Parser.h"
#include "tests.cpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << "--command [expression] [var=value ...]" << endl;
        return 1;
    }

    string command = argv[1];
    string input = argv[2];
    //cout<<input<<endl;

    Parser<double> parser(input);
    if (command == "--eval") {
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
            for (auto i : variables) {
                cout<<i.first<<"="<<i.second<<endl;
            }
            auto expr = parser.parse();
            cout << "Result: " << expr->apply(variables) << endl;
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
        }
    } else if (command == "--diff") {
        if (argc < 5 || string(argv[3]) != "--by") {
            cerr << "Usage: " << argv[0] << " --diff [expression] --by [var] [var=value ...]" << endl;
            return 1;
        }
        string var = argv[4];
        try {
            // map<string, double> variables;
            // for (int i = 5; i < argc; ++i) {
            //     string arg = argv[i];
            //     int pos = arg.find('=');
            //     if (pos != -1) {
            //         string var_name = arg.substr(0, pos);
            //         double val = stod(arg.substr(pos + 1));
            //         variables[var_name] = val;
            //     }
            // }

            auto expr = parser.parse();
            auto derivative = expr->differentiate(var);
            cout<<derivative->print()<<endl;
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
        }
    } else {
        cerr << "Unknown command: " << command << endl;
        return 1;
    }


    return 0;
}
