#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "Expression.h"
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <vector>
#include <complex>
#include <iostream>
#include <map>

using namespace std;
using Complex = std::complex<double>;

enum TokenType { START, FUNC, NUM, VAR, OP };

template<typename T>
class Parser {
    string input;
    int pos;
    std::vector<TokenType> tokens;
    std::vector<shared_ptr<Expression<T> > > exprs;

    void skip() {
        while (pos < input.size() && input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n') {
            pos++;
        }
    }

    bool is_number_minus() {
        return (tokens.back() == START || tokens.back() == OP);
    }

    shared_ptr<Expression<T> > parse_num_exp() {
        int start = pos;
        int dots = 0;
        while (pos < input.size() && ('0' <= input[pos] && input[pos] <= '9' || input[pos] == '.')) {
            if (input[pos] == '.') dots++;
            pos++;
        }
        if (dots > 1) throw runtime_error("Syntax error in expression");
        string number = input.substr(start, pos - start);
        return make_shared<NumExpression<T> >(stoll(number));
    }

    shared_ptr<Expression<T> > parse_var_exp() {
        string var;
        while (pos < input.size() && isalpha(input[pos])) {
            var.push_back(input[pos]);
            pos++;
        }
        if (var.length() != 1) throw runtime_error(
            "Syntax error in expression: variable can be expressed only in one letter");

        return make_shared<VarExpression<T> >(var);
    }

    shared_ptr<Expression<T> > parse_brackets() {
        int scope_end = pos, balance = 0;
        for (; scope_end < input.size(); scope_end++) {
            if (input[scope_end] == '(') {
                balance++;
            }
            else if (input[scope_end] == ')')
                balance--;
            if (balance == 0) {
                break;
            }
            if (balance < 0) throw runtime_error("Syntax error in expression: too much ')'");
        }
        if (balance > 0) throw runtime_error("Syntax error in expression: too much '('");
        if (scope_end - pos == 1) {
            throw runtime_error("Syntax error in expression: empty brackets");
        }
        auto scope_expr = input.substr(pos + 1, scope_end - pos - 1);
        Parser scope(scope_expr);
        auto expr = scope.parse();
        pos = scope_end + 1;
        return expr;
    }

    void add_func(char op, int len) {
        if (tokens.back() != START && tokens.back() != OP)
            throw runtime_error("Syntax error in expression");

        pos += len;
        if (input[pos] != '(')
            throw runtime_error("Syntax error in expression: after function missed '('");
        auto in_expr = parse_brackets();
        auto func = std::make_shared<MonoExpression<T>>(in_expr, op);
        if (tokens.back() == OP) {
            exprs.back()->set_right(func);
        }
        exprs.push_back(func);
        tokens.push_back(FUNC);
    }

    void add_binary(char op) {
        if (tokens.back() != NUM && tokens.back() != VAR && tokens.back() != FUNC)
            throw runtime_error("Syntax error in expression");
        auto left = exprs.back();
        exprs.pop_back();
        exprs.push_back(make_shared<BinaryExpression<T> >(left, nullptr, op));
        tokens.push_back(OP);
        pos++;
    }

    void parse_exp() {
        while (pos < input.size()) {
            skip();
            if (input[pos] == '(') {
                if (tokens.back() != START && tokens.back() != OP)
                    throw runtime_error("Syntax error in expression");
                auto scope_eval = parse_brackets();

                if (tokens.back() == OP) {
                    exprs.back()->set_right(scope_eval);
                }
                else
                    exprs.push_back(scope_eval);
                tokens.push_back(NUM);
            }
            else if (pos + 1 < input.size() && input.substr(pos, 2) == "ln") {
                add_func('l', 2);
            }
            else if (pos + 2 < input.size() && input.substr(pos, 3) == "exp") {
                add_func('e', 3);
            }
            else if (pos + 2 < input.size() && input.substr(pos, 3) == "cos") {
                add_func('c', 3);
            }
            else if (pos + 2 < input.size() && input.substr(pos, 3) == "sin") {
                add_func('s', 3);
            }
            else if (input[pos] == '^') {
                add_binary('^');
            }
            else if (input[pos] == '/') {
                add_binary('/');
            }
            else if (input[pos] == '*') {
                add_binary('*');
            }
            else if (input[pos] == '+') {
                add_binary('+');
            }
            else if (input[pos] == '-' && !is_number_minus()) {
                add_binary('-');
            }
            else if (input[pos] == '-') {
                exprs.push_back(make_shared<NumExpression<T> >(0));
                tokens.push_back(NUM);
                add_binary('-');
            }
            else if (isdigit(input[pos])) {
                if (tokens.back() != START && tokens.back() != OP)
                    throw runtime_error("Syntax error in expression");
                auto number = parse_num_exp();
                if (tokens.back() == OP) {
                    exprs.back()->set_right(number);
                }
                else
                    exprs.push_back(number);
                tokens.push_back(NUM);
            }
            else if (isalpha(input[pos])) {
                if (tokens.back() != START && tokens.back() != OP)
                    throw runtime_error("Syntax error in expression");
                auto var = parse_var_exp();
                if (tokens.back() == OP) {
                    exprs.back()->set_right(var);
                }
                exprs.push_back(var);
                tokens.push_back(VAR);
            }
            else {
                //throw runtime_error("Syntax error in expression: unknown symbol " + input[pos]);
                if (pos >= input.size())
                    return;
            }
        }
        if (tokens.back() != NUM && tokens.back() != VAR && tokens.back() != FUNC)
            throw runtime_error("Syntax error in expression");
        /*if (tokens.size() > 2)
            exprs.pop_back();*/
    }

    void apply_expr(int ind) {
        auto result = exprs[ind];
        if (ind > 0)
            exprs[ind - 1]->set_right(result);
        if (ind + 1 < exprs.size())
            exprs[ind + 1]->set_left(result);
        exprs.erase(exprs.begin() + ind);
    }

    bool find_power() {
        for (int i = exprs.size() - 1; i >= 0; i--) {
            if (exprs[i]->get_op()) {
                auto op = exprs[i]->get_op();

                if (op == '^') {
                    apply_expr(i);
                    return true;
                }
            }
        }
        return false;
    }

    bool find_multipliers() {
        for (int i = 0; i < exprs.size(); i++) {
            if (exprs[i]->get_op()) {
                auto op = exprs[i]->get_op();
                if (op == '/' || op == '*') {
                    apply_expr(i);
                    return true;
                }
            }
        }
        return false;
    }

    bool find_additions() {
        for (int i = exprs.size() - 1; i >= 0; i--) {
            if (exprs[i]->get_op()) {
                auto op = exprs[i]->get_op();
                if (op == '+' || op == '-') {
                    apply_expr(i);
                    return true;
                }
            }
        }
        return false;
    }

public:
    Parser(const string& input) : input(input), pos(0), tokens({ START }) {
    }

    shared_ptr<Expression<T>> parse() {
        parse_exp();


        while (exprs.size() > 1) {
            bool have_power = find_power();
            if (!have_power) {
                bool have_multi = find_multipliers();
                if (!have_multi) {
                    bool have_add = find_additions();
                    if (!have_add) throw runtime_error("Syntax error in expression");

                }
            }
        }
        return exprs.back();
    }
};
