#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>
#include <map>
#include <stdexcept>
#include <cmath>
#include <ios>
#include <iostream>
#include <ccomplex>
#include <bits/ios_base.h>

using Complex = std::complex<double>;

template <typename T>
class Expression {
public:
    virtual char get_op() { return 0; }
    virtual void set_expr(const std::shared_ptr<Expression<T>>& given_expr) {}
    virtual void set_right(const std::shared_ptr<Expression<T>>& given_right) {}
    virtual void set_left(const std::shared_ptr<Expression<T>>& given_left) {}
    virtual T apply(std::map<std::string, T>& params) = 0;
    virtual T get_value() = 0;
    virtual std::shared_ptr<Expression<T>> differentiate(const std::string& var) = 0;
    virtual std::string print() { return ""; }
};

template <typename T>
class NumExpression : public Expression<T> {
public:
    T value;
    NumExpression() = default;
    NumExpression(T source) {
        value = source;
    }
    ~NumExpression() = default;
    NumExpression(const NumExpression&) = default;
    NumExpression& operator=(const NumExpression&) = default;
    NumExpression(NumExpression&&) = default;
    NumExpression& operator=(NumExpression&&) = default;


    T get_value() override {
        return value;
    };

    std::shared_ptr<Expression<T>> differentiate(const std::string& var) override {
        return std::make_shared<NumExpression<T>>(0);
    }
    T apply(std::map<std::string, T>& params) override {
        //return make_shared<NumExpression<T>>(*this);
        return value;
    };

    std::string print() override {
        return std::to_string(value);
    }
};

template <typename T>
class VarExpression : public Expression<T> {
    std::string value;
public:
    VarExpression(const std::string& source) {
        value = source;
    }
    ~VarExpression() = default;
    VarExpression(const VarExpression&) = default;
    VarExpression& operator=(const VarExpression&) = default;
    VarExpression(VarExpression&&) = default;
    VarExpression& operator=(VarExpression&&) = default;

    T get_value() override {
        T val = 0;
        return val;
    };
    T apply(std::map<std::string, T>& params) override {
        auto it = params.find(value);
        if (it != params.end())
            return it->second;
        throw std::runtime_error("Variable " + value + " is not defined");
    };
    std::shared_ptr<Expression<T>> differentiate(const std::string& var) override {
        if (value == var) {
            return std::make_shared<NumExpression<T>>(1);
        }
        else {
            return std::make_shared<NumExpression<T>>(0);
        }
    }

    std::string print() override {
        return value;
    }
};

template <typename T>
class BinaryExpression : public Expression<T> {
    std::shared_ptr<Expression<T>> left;
    std::shared_ptr<Expression<T>> right;
    char op;
public:
    BinaryExpression(const std::shared_ptr<Expression<T>>& given_left, const std::shared_ptr<Expression<T>>& given_right, char given_op) {
        left = given_left, op = given_op, right = given_right;
    }
    ~BinaryExpression() = default;
    BinaryExpression(const BinaryExpression&) = default;
    BinaryExpression& operator=(const BinaryExpression&) = default;
    BinaryExpression(BinaryExpression&&) = default;
    BinaryExpression& operator=(BinaryExpression&&) = default;

    char get_op() override {
        return op;
    };
    void set_left(const std::shared_ptr<Expression<T>>& given_left) override {
        left = given_left;
    };
    void set_right(const std::shared_ptr<Expression<T>>& given_right) override {
        right = given_right;
    };
    T apply(std::map<std::string, T>& params) override {
        auto left_val = left->apply(params), right_val = right->apply(params);
        // Expression<T> result;
        if (op == '+')
            return left_val + right_val;
        else if (op == '-')
            return  left_val - right_val;
        else if (op == '*')
            return left_val * right_val;
        else if (op == '/') {
            if (right_val == 0) {
                throw std::runtime_error("Division by zero");
            }
            return left_val / right_val;
        }
        else if (op == '^') {
            return pow(left_val, right_val);
        }
    };

    std::shared_ptr<Expression<T>> differentiate(const std::string& var) override;

    T get_value() override {
        T val = 0;
        return val;
    }

    std::string print() override {
        return "(" + left->print() + " " + op + " " + right->print() + ")";
    }
};


template <typename T>
class MonoExpression : public Expression<T> {
    std::shared_ptr<Expression<T>> expr;
    char op;
public:
    MonoExpression(const std::shared_ptr<Expression<T>>& given_expr, char given_op) {
        expr = given_expr, op = given_op;
    }
    ~MonoExpression() = default;
    MonoExpression(const MonoExpression&) = default;
    MonoExpression& operator=(const MonoExpression&) = default;
    MonoExpression(MonoExpression&&) = default;
    MonoExpression& operator=(MonoExpression&&) = default;

    char get_op() override {
        return op;
    };
    T apply(std::map<std::string, T>& params) override {
        auto val = expr->apply(params);
        //NumExpression<T> result;
        if (op == 's')
            return sin(val);
        else if (op == 'c')
            return cos(val);
        else if (op == 'l') {
            if (val <= 0)
                throw std::runtime_error("Error: ln takes only positive values\n");
            return log(val);
        }
        else if (op == 'e')
            return exp(val);
    }

    std::shared_ptr<Expression<T>> differentiate(const std::string& var) override {
        auto expr_diff = expr->differentiate(var);

        switch (op) {
        case 's':
            return std::make_shared<BinaryExpression<T>>(
                expr_diff,
                std::make_shared<MonoExpression<T>>(expr, 'c'),
                '*'
            );
        case 'c':
            return std::make_shared<BinaryExpression<T>>(
                std::make_shared<NumExpression<T>>(-1),
                std::make_shared<BinaryExpression<T>>(
                    expr_diff,
                    std::make_shared<MonoExpression<T>>(expr, 's'),
                    '*'
                ),
                '*'
            );
        case 'l':
            return std::make_shared<BinaryExpression<T>>(expr_diff, expr, '/');
        case 'e':
            return std::make_shared<BinaryExpression<T>>(
                expr_diff,
                std::make_shared<MonoExpression<T>>(expr, 'e'),
                '*'
            );
        default:
            throw std::runtime_error("Unknown operator");
        }
    }

    void set_expr(const std::shared_ptr<Expression<T>>& given_expr) override {
        expr = given_expr;
    };

    T get_value() override {
        T val = 0;
        return val;
    }

    std::string print() override {
        auto str = expr->print();
        if (str[0] != '(')
            str = '(' + str + ')';
        switch (op) {
        case 's':
            return "sin" + str;
        case 'c':
            return "cos" + str;
        case 'l':
            return "ln" + str;
        case 'e':
            return "exp" + str;
        }
    }
};

template <typename T>
std::shared_ptr<Expression<T>> BinaryExpression<T>::differentiate(const std::string& var) {
    auto left_diff = left->differentiate(var);
    auto right_diff = right->differentiate(var);

    switch (op) {
    case '+':
        return std::make_shared<BinaryExpression<T>>(left_diff, right_diff, '+');
    case '-':
        return std::make_shared<BinaryExpression<T>>(left_diff, right_diff, '-');
    case '*':
        return std::make_shared<BinaryExpression<T>>(
            std::make_shared<BinaryExpression<T>>(left_diff, right, '*'),
            std::make_shared<BinaryExpression<T>>(left, right_diff, '*'),
            '+'
        );
    case '/': {
        auto numerator = std::make_shared<BinaryExpression<T>>(
            std::make_shared<BinaryExpression<T>>(left_diff, right, '*'),
            std::make_shared<BinaryExpression<T>>(left, right_diff, '*'),
            '-'
        );
        auto denominator = std::make_shared<BinaryExpression<T>>(right, std::make_shared<NumExpression<T>>(2), '^');
        return std::make_shared<BinaryExpression<T>>(numerator, denominator, '/');
    }
    case '^': {
        auto log_term = std::make_shared<MonoExpression<T>>(left, 'l');
        auto term1 = std::make_shared<BinaryExpression<T>>(right_diff, log_term, '*');
        auto term2 = std::make_shared<BinaryExpression<T>>(
            std::make_shared<BinaryExpression<T>>(right, left_diff, '*'),
            left,
            '/'
        );
        auto sum = std::make_shared<BinaryExpression<T>>(term1, term2, '+');
        auto copy = std::make_shared<BinaryExpression<T>>(left, right, op);
        return std::make_shared<BinaryExpression<T>>(copy, sum, '*');
    }
    default:
        throw std::runtime_error("Unknown operator");
    }
}
#endif

