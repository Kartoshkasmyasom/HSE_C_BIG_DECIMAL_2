#ifndef BIG_DECIMAL_H
#define BIG_DECIMAL_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

class BigDecimal {
private:
	std::vector<long long> big_integer, big_fraction;
	bool sign = 0;
	int base = 32, exp_starts, integer, fraction;
public:

	BigDecimal();

	BigDecimal(std::string source, int given_integer = 8, int given_fraction = 8);
	
	BigDecimal(double source);
	
//	BigDecimal(long source);

	BigDecimal(BigDecimal& other) = default;
	
	BigDecimal(BigDecimal&& other) = default;

	BigDecimal& operator=(const BigDecimal& other);

	BigDecimal& operator=(BigDecimal&& other) = default;

	bool compare_absolute_values(const BigDecimal& other) const;

	bool operator==(const BigDecimal& other) const;

	const BigDecimal operator+(const BigDecimal& b) const;
	const BigDecimal operator-(const BigDecimal& b) const;

	virtual ~BigDecimal();
};

#endif // BIG_DECIMAL_H