#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>
#include <map>
#include <stdexcept>
#include <cmath>


template <typename T>
class Expression {
public:
	virtual char get_op() {return 0;}
	virtual void set_expr(const std::shared_ptr<Expression<T>> &given_expr) {}
	virtual void set_right(const std::shared_ptr<Expression<T>>& given_right) {}
	virtual void set_left(const std::shared_ptr<Expression<T>>& given_left) {}
	virtual T apply(std::map<std::string, T> &params) = 0;
	virtual T get_value() = 0;
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
	T apply(std::map<std::string, T> &params) override {
		//return make_shared<NumExpression<T>>(*this);
		return value;
	};
};

template <typename T>
class VarExpression : public Expression<T>{
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

	};
	T apply(std::map<std::string, T> &params) override {
		auto it = params.find(value);
		if (it != params.end())
			return it->second;
		throw std::runtime_error("Variable " + value + " is not defined");
	};
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
	T apply(std::map<std::string, T> &params) override {
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
		} else if (op == '^') {
			return pow(left_val, right_val);
		}
	};

	T get_value() override {

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
	T apply(std::map<std::string, T> &params) override {
		auto val = expr->apply(params);
		//NumExpression<T> result;
		if (op == 's')
			return sin(val);
		else if (op == 'c')
			return cos(val);
		else if (op == 'l')
			return log(val);
		else if (op == 'e')
			return exp(val);
	}
	void set_expr(const std::shared_ptr<Expression<T>> &given_expr) override {
		expr = given_expr;
	};

	T get_value() override {
	}
};
#endif

