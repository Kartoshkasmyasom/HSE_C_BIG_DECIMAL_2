// #include <iomanip>
//
// #include "Expression.h"
// #include "Expression.cpp"
// #include "Parser.h"
// #include <memory>
// #include <iostream>
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
// // runtime check
// bool test3() {
//     Parser<double> parser1("x * sin(x)"), parser2("x * sin(ln(cos(x)))");
//     map<string, double> variables1 = {{"x", 1.5}};
//     auto expr = parser1.parse();
//     cout<<expr->apply(variables1)<<endl;
//     return true;
// }
//
// bool test_differ() {
//     auto expr = make_shared<NumExpression<double>>(5.0);
//     auto derivative = expr->differentiate("x");
//
//     return (derivative->get_value() == 0);
// }
//
// bool test_differ_var() {
//     auto expr1 = make_shared<VarExpression<double>>("x");
//     auto derivative1 = expr1->differentiate("x");
//     auto derivative2 = expr1->differentiate("y");
//     return (derivative1->get_value() == 1 && derivative2->get_value() == 0);
// }
//
// int main() {
//     if (test1()) {
//         cout << "OK\n";
//     }else
//         cout << "FAIL\n";
//     if (test2())
//         cout << "OK\n";
//     else
//         cout << "FAIL\n";
//     test3();
//     if (test_differ())
//         cout << "OK\n";
//     else
//         cout<<"FAIL\n";
//     if (test_differ_var())
//         cout << "OK\n";
//     else
//         cout<<"FAIL\n";
//     return 0;
// }
