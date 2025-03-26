// #include <iomanip>
//
// #include "Expression.h"
// #include "Expression.cpp"
// #include "Parser.h"
// #include <memory>
// using namespace std;
//
// bool test1() {
//     Parser<double> parser1("2 + 3 * 4");
//     auto expr1 = parser1.parse();
//
//     Parser<double> parser2("2 * (3 + 4)");
//     auto expr2 = parser2.parse();
//
//     std::map<std::string, double> params;
//     Parser<double> parser3("2 ^ 3 + 4 * 5");
//     auto expr3 = parser3.parse();
//     Parser<double> parser4("2 + 3 * 4 ^ 2");
//     auto expr4 = parser4.parse();
//
//
//     return (expr1->apply(params) == 14) &&(expr2->apply(params) == 14) && (expr3->apply(params) == 28) && (expr4->apply(params) == 50);
// }
//
// bool test2() {
//     Parser<double> parser1("-10 + 20");
//     auto expr1 = parser1.parse();
//     std::map<std::string, double> params;
//
//     Parser<double> parser2("10 - (-20)");
//     auto expr2 = parser2.parse();
//
//     Parser<double> parser3("10 / 20");
//     auto expr3 = parser3.parse();
//     Parser<double> parser4("x * (-y)");
//     auto expr4 = parser4.parse();
//     map<string, double> variables = {{"x", 5}, {"y", 3}};
//
//     Parser<double> parser("sin(-x)");
//     auto expr = parser.parse();
//     map<string, double> variables1 = {{"x", 1}};
//     return (expr1->apply(params) == 10)&& (expr2->apply(params) == 30) && (expr3->apply(params) == 0.5) && (expr4->apply(variables) == -15) && (expr->apply(variables1) == sin(-1));
//
// }
//
//
//
// // int main() {
// //     if (test1()) {
// //         std::cout << "OK\n";
// //     }else
// //         std::cout << "FAIL\n";
// //     if (test2())
// //         std::cout << "OK\n";
// //     else
// //         std::cout << "FAIL\n";
// //     return 0;
// // }
