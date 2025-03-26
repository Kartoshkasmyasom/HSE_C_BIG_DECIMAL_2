#ifndef BIG_DECIMAL_H
#define BIG_DECIMAL_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>


class BigDecimal {
private:
	std::vector<long long> big_integer, big_fraction;
	bool sign = 0; // 1 - negative, 0 - positive
	int base = 32, exp_starts, integer, fraction;
public:

	BigDecimal();

	BigDecimal(std::string source, int given_integer = 8, int given_fraction = 8);
	
//	BigDecimal(long source);

	BigDecimal(BigDecimal& other) = default;
	
	BigDecimal(BigDecimal&& other) = default;

	BigDecimal& operator=(const BigDecimal& other);

	BigDecimal& operator=(BigDecimal&& other) = default;

	bool compare_absolute_values(const BigDecimal& other) const;

	bool operator==(const BigDecimal& other) const;
	bool operator!=(const BigDecimal& other) const;
	bool operator<(const BigDecimal& other) const;
	bool operator>(const BigDecimal& other) const;
	bool operator<=(const BigDecimal& other) const;
	bool operator>=(const BigDecimal& other) const;

	const BigDecimal operator*(const BigDecimal& b) const;
	const BigDecimal operator/(const BigDecimal& b) const;
	void operator *=(const BigDecimal& b);
	void operator /=(const BigDecimal& b);
	

	const BigDecimal operator+(const BigDecimal& b) const;
	const BigDecimal operator-(const BigDecimal& b) const;
	void operator+=(const BigDecimal& b);
	void operator-=(const BigDecimal& b);


	virtual ~BigDecimal();
};

BigDecimal operator""_longnum(long double number);
#endif // BIG_DECIMAL_H
