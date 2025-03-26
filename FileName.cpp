#include "BigDecimal.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>

using namespace std;

typedef long long ll;

long long insert_bit(ll mask, int k, int bit) {
	mask = mask & ~(1ll << k);
	return mask | ((long long)bit << k);
}

bool get_bit(ll mask, int k) {
	return mask & (1ll << k);
}

unsigned int reverse_mask(ll mask, int mask_length) {
	for (int i = 0; i < mask_length / 2; i++) {
		int other_bit = get_bit(mask, mask_length - i - 1), bit = get_bit(mask, i);
		mask = insert_bit(mask, mask_length - i - 1, bit);
		mask = insert_bit(mask, i, other_bit);
	}
	return mask;
}

int convert_integer_to_base2(vector<ll>& big_integer, string source_integer, int bits_for_integer, int block_size, int& exp_starts) {
	if (source_integer == "0") {
		big_integer.push_back(0);
		exp_starts = 1;
		return 0;
	}
	int added_bits = 0;
	while (added_bits < bits_for_integer && source_integer != "0") {
		if (added_bits % block_size == 0) {
			big_integer.push_back(0);
		}
		int bit = (source_integer.back() - '0') % 2;
		big_integer.back() = insert_bit(big_integer.back(), added_bits % block_size, bit);
		added_bits++;

		string division_result;
		if (bit)
			source_integer.back()--;
		char left_dig = '0';
		for (int i = 0; i < source_integer.length(); i++) {
			int current_divisor = (left_dig - '0') * 10 + (source_integer[i] - '0');
			if (current_divisor == 1) {
				division_result.push_back('0');
				left_dig = '1';
				continue;
			}
			division_result.push_back(current_divisor / 2 + '0');
			left_dig = '0' + current_divisor % 2;
		}
		if (division_result.length() > 1 && division_result[0] == '0')
			division_result = division_result.substr(1, division_result.length() - 1);
		source_integer = division_result;
	}
	exp_starts = added_bits;
	return source_integer != "0";
}

int convert_fraction_to_base2(vector<ll>& big_fraction, int& exp_starts, string source_fraction, int bits_for_fraction, int block_size) {
	if (source_fraction == "0") {
		big_fraction.push_back(0);
		exp_starts = 1;
		return 0;
	}
	int added_bits = 0;
	while (added_bits < bits_for_fraction && source_fraction != "0") {
		string addition_result;
		if (added_bits % block_size == 0) {
			big_fraction.push_back(0);
			//if (big_fraction.size() > 1)
				//big_fraction[big_fraction.size() - 2] = reverse_mask(big_fraction[big_fraction.size() - 2], block_size);
		}

		int extra_addition = 0;
		for (int i = source_fraction.size() - 1; i > -1; i--) {
			int sum_of_two_digits = extra_addition + 2 * (source_fraction[i] - '0');
			addition_result.push_back(sum_of_two_digits % 10 + '0');
			extra_addition = sum_of_two_digits / 10;
		}
		reverse(addition_result.begin(), addition_result.end());
		while (addition_result.size() > 1 && addition_result.back() == '0')
			addition_result.pop_back();

		big_fraction.back() = insert_bit(big_fraction.back(), block_size - added_bits % block_size - 1, extra_addition);
		added_bits++;

		source_fraction = addition_result;
		//cout << extra_addition << ' ' << addition_result << "\n";
	}
	//big_fraction.back() = reverse_mask(big_fraction.back(), added_bits % block_size);
	return source_fraction != "0";
}

bool BigDecimal::compare_absolute_values(const BigDecimal& other) const{ // 1 - если меньше, иначе 0
	if (other.exp_starts < exp_starts || exp_starts > other.exp_starts)
		return exp_starts < other.exp_starts;

	for (int i = big_integer.size() - 1; i > -1; i--){
		if (big_integer[i] < other.big_integer[i])
			return true;
		else if (big_integer[i] > other.big_integer[i])
			return false;
	}

	for (int i = 0; i < min(big_fraction.size(), other.big_fraction.size()); i--) {
		if (big_fraction[i] < other.big_fraction[i])
			return true;
		else if (big_fraction[i] > other.big_fraction[i])
			return false;
	}
	return big_fraction.size() < other.big_fraction.size();
}

BigDecimal::BigDecimal() {
	big_integer.push_back(0);
	big_fraction.push_back(0);
	sign = 0;
	exp_starts = 1;
	integer = 8, fraction = 8;
}

BigDecimal::BigDecimal(string source, int given_integer, int given_fraction) {
	integer = given_integer, fraction = given_fraction;
	if (source[0] == '-') {
		sign = 1;
	}

	int source_integer_length = 0;
	while (source_integer_length < source.size() && source[source_integer_length] != '.')
		source_integer_length++;

	convert_integer_to_base2(big_integer, source.substr(sign, source_integer_length - sign), integer, base, exp_starts);
	if (source.length() > source_integer_length)
		convert_fraction_to_base2(big_fraction, exp_starts, source.substr(source_integer_length + 1, source.size() - source_integer_length), fraction, base);
	else
		big_fraction.push_back(0);
	/*for (auto i : big_fraction) {
		for (int j = 0; j < base; j++) {
			cout << get_bit(i, j);
		}
		cout << ' ';
	}
	cout << endl;*/
}

BigDecimal& BigDecimal::operator=(const BigDecimal& other) {
	if (this != &other) {
		big_integer = other.big_integer;
		big_fraction = other.big_fraction;
		sign = other.sign;
		base = other.base;
		exp_starts = other.exp_starts;
		integer = other.integer;
		fraction = other.fraction;
	}
	return *this;
}

bool BigDecimal::operator==(const BigDecimal& other) const{
	if (other.sign == sign) {
		if (other.big_fraction.size() == big_fraction.size() && other.exp_starts == exp_starts) {
			for (int i = 0; i < big_fraction.size();i++) {
				if (big_fraction[i] != other.big_fraction[i])
					return false;
			}

			for (int i = 0; i < big_integer.size();i++) {
				if (big_integer[i] != other.big_integer[i])
					return false;
			}
			return true;
		}
	}
	return false;
}

bool BigDecimal::operator!=(const BigDecimal& other) const {
	return !(*this == other);
}

bool BigDecimal::operator<(const BigDecimal& other) const {
	if (*this == other) return false;
	if (sign && !other.sign)
		return true;
	if (!sign && other.sign)
		return false;
	bool res = compare_absolute_values(other);
	return (!res && sign) || (res && !sign);
}

bool BigDecimal::operator>(const BigDecimal& other) const {
	if (*this == other) return false;
	return !(*this < other);
}

bool BigDecimal::operator>=(const BigDecimal& other) const {
	return !(*this < other);
}

bool BigDecimal::operator<=(const BigDecimal& other) const {
	return !(*this > other);
}



const BigDecimal BigDecimal::operator+(const BigDecimal& b) const{
	if (!sign && b.sign) {
		BigDecimal b_copy;
		b_copy = b;
		b_copy.sign = 0;
		return *this - b_copy;
	}
	if (sign && !b.sign) {
		BigDecimal a_copy;
		a_copy = *this;
		a_copy.sign = 0;
		return b - a_copy;
	}

	BigDecimal sum("0", max(b.integer, integer), max(b.fraction, fraction));
	sum.sign = sign;

	int b_fraction_size = b.big_fraction.size();
	int a_fraction_size = big_fraction.size();
	int max_fraction_size = max(b_fraction_size, a_fraction_size);
	sum.big_fraction.resize(max_fraction_size);

	ll extra_addition = 0;
	ll base_module = (1ll << base);

	for (int i = max_fraction_size - 1; i >= 0; i--) {
		ll a_val = (i < a_fraction_size) ? big_fraction[i] : 0;
		ll b_val = (i < b_fraction_size) ? b.big_fraction[i] : 0;
		ll sum_val = a_val + b_val + extra_addition;
		sum.big_fraction[i] = sum_val % base_module;
		extra_addition = sum_val / base_module;
	}

	int b_integer_size = b.big_integer.size();
	int a_integer_size = big_integer.size();
	int max_integer_size = max(b_integer_size, a_integer_size);
	sum.big_integer.resize(max_integer_size);

	for (int i = 0; i < max_integer_size; i++) {
		ll a_val = (i < a_integer_size) ? big_integer[i] : 0;
		ll b_val = (i < b_integer_size) ? b.big_integer[i] : 0;
		ll sum_val = a_val + b_val + extra_addition;
		sum.big_integer[i] = sum_val % base_module;
		extra_addition = sum_val / base_module;
	}

	if (extra_addition)
		sum.big_integer.push_back(extra_addition);

	while (sum.big_fraction.back() == 0 && sum.big_fraction.size() > 1)
		sum.big_fraction.pop_back();

	while (sum.big_integer.back() == 0 && sum.big_integer.size() > 1)
		sum.big_integer.pop_back();

	int greatest_block_integer_length = 0;
	for (int i = base - 1; i >= 0; i--) {
		if (get_bit(sum.big_integer.back(), i)) {
			greatest_block_integer_length = i + 1;
			break;
		}
	}
	sum.exp_starts = base * (big_integer.size() - 1) + greatest_block_integer_length;
	sum.integer = max(sum.integer, exp_starts);

	if (sum.exp_starts == 0) sum.exp_starts = 1;

	if (sum.big_fraction.size() == 1 && sum.big_fraction[0] == 0 && sum.big_integer.size() == 1 && sum.big_integer[0] == 0)
		sum.sign = 0;

	return sum;
}

const BigDecimal BigDecimal::operator-(const BigDecimal& b) const {
	if (!sign && b.sign) {
		BigDecimal b_copy;
		b_copy = b;
		b_copy.sign = 0;
		return *this + b_copy;
	}
	else if (sign && !b.sign) {
		BigDecimal b_copy;
		b_copy = b;
		b_copy.sign = 1;
		return *this + b_copy;
	}

	BigDecimal difference("0", max(b.integer, integer), max(b.fraction, fraction));
	
	BigDecimal larger_number, smaller_number;
	if (compare_absolute_values(b)) {
		larger_number = b;
		smaller_number = *this;
		difference.sign = 1;
	}
	else {
		larger_number = *this;
		smaller_number = b;
		difference.sign = 0;
	}

	int larger_fraction_size = larger_number.big_fraction.size();
	int smaller_fraction_size = smaller_number.big_fraction.size();
	int max_fraction_size = max(larger_fraction_size, smaller_fraction_size);
	difference.big_fraction.resize(max_fraction_size);

	ll extra_subtraction = 0;
	ll base_module = (1ll << base);

	for (int i = max_fraction_size - 1; i >= 0; i--) {
		ll a_val = (i < larger_fraction_size) ? larger_number.big_fraction[i] : 0;
		ll b_val = (i < smaller_fraction_size) ? smaller_number.big_fraction[i] : 0;
		a_val -= extra_subtraction;
		if (a_val < b_val) {
			a_val += base_module;
			extra_subtraction = 1;
		}
		ll sum_val = a_val - b_val;
		difference.big_fraction[i] = sum_val;
	}

	int larger_integer_size = larger_number.big_integer.size();
	int smaller_integer_size = smaller_number.big_integer.size();
	int max_integer_size = max(larger_integer_size, smaller_integer_size);
	difference.big_integer.resize(max_integer_size);

	for (int i = 0; i < max_integer_size; i++) {
		ll a_val = (i < larger_integer_size) ? larger_number.big_integer[i] : 0;
		ll b_val = (i < smaller_integer_size) ? smaller_number.big_integer[i] : 0;
		a_val -= extra_subtraction;
		if (a_val < b_val) {
			a_val += base_module;
			extra_subtraction = 1;
		}
		ll sum_val = a_val - b_val;
		difference.big_integer[i] = sum_val;
	}

	while (difference.big_fraction.back() == 0 && difference.big_fraction.size() > 1)
		difference.big_fraction.pop_back();

	while (difference.big_integer.back() == 0 && difference.big_integer.size() > 1)
		difference.big_integer.pop_back();

	int greatest_block_integer_length = 0;
	for (int i = base - 1; i >= 0; i--) {
		if (get_bit(difference.big_integer.back(), i)) {
			greatest_block_integer_length = i + 1;
			break;
		}
	}
	difference.exp_starts = base * (big_integer.size() - 1) + greatest_block_integer_length;

	if (difference.exp_starts == 0) difference.exp_starts = 1;
	difference.integer = max(difference.integer, difference.exp_starts);

	return difference;
}

void BigDecimal::operator+=(const BigDecimal& b) {
	*this = *this + b;
}

void BigDecimal::operator-=(const BigDecimal& b) {
	*this = *this - b;
}

bool is_zero(vector<long long>& number_part) {
	return (number_part.size() == 1 && number_part[0] == 0);
}

const BigDecimal BigDecimal::operator*(const BigDecimal& b) const {
	BigDecimal integer_multiplication("0", max(exp_starts + b.exp_starts, max(integer, b.integer)), max(fraction, b.fraction));
	BigDecimal b_copy;
	b_copy = b;
	b_copy.big_fraction = { 0 };
	for (int i = 0; i < exp_starts; i++) {
		if (get_bit(big_integer[i / base], i % base))
			integer_multiplication += b_copy;
		b_copy += b_copy;
		b_copy.exp_starts++;
		b_copy.integer = max(b_copy.integer, b_copy.exp_starts);
	}
	BigDecimal integer_fraction_multiplication("0", max(exp_starts + b.exp_starts, max(integer, b.integer)), max(fraction, b.fraction));
	BigDecimal b_fraction_copy;
	b_fraction_copy = b;
	b_fraction_copy.big_integer = { 0 };
	b_fraction_copy.exp_starts = 1;

	for (int i = 0; i < exp_starts; i++) {
		if (get_bit(big_integer[i / base], i % base))
			integer_fraction_multiplication += b_fraction_copy;
		b_fraction_copy += b_fraction_copy;
		if (b_fraction_copy.big_integer.size() == 1 && b_fraction_copy.big_integer[0] < 2)
			b_fraction_copy.exp_starts = 1;
		else
			b_fraction_copy.exp_starts++;
		b_fraction_copy.integer = max(b_fraction_copy.integer, b_fraction_copy.exp_starts);
	}

	BigDecimal fraction_integer_multiplication("0", max(exp_starts + b.exp_starts, max(integer, b.integer)), max(fraction, b.fraction));
	BigDecimal a_fraction_copy;
	a_fraction_copy = *this;
	a_fraction_copy.big_integer = { 0 };
	a_fraction_copy.exp_starts = 1;

	for (int i = 0; i < b.exp_starts; i++) {
		if (get_bit(b.big_integer[i / b.base], i % b.base))
			fraction_integer_multiplication += a_fraction_copy;
		a_fraction_copy += a_fraction_copy;
		if (a_fraction_copy.big_integer.size() == 1 && a_fraction_copy.big_integer[0] < 2)
			a_fraction_copy.exp_starts = 1;
		else
			a_fraction_copy.exp_starts++;
		a_fraction_copy.integer = max(a_fraction_copy.integer, a_fraction_copy.exp_starts);
	}

	int new_precision = 1;
	int max_precision = max(fraction, b.fraction);
	while (new_precision < max_precision)
		new_precision *= base;
	BigDecimal fraction_multiplication("0", 8, new_precision);
	BigDecimal b_fraction_copy2;
	b_fraction_copy2 = b;
	b_fraction_copy2.big_integer = { 0 };
	b_fraction_copy2.exp_starts = 1;
	while (b_fraction_copy2.big_fraction.size() < new_precision / base)
		b_fraction_copy2.big_fraction.push_back(0);

	for (int i = 0; i < big_fraction.size(); i++) {
		for (int j = base - 1; j >= 0; j--) {
			int lost_bit = 0;
			for (int u = 0; u < new_precision / base; u++) {
				insert_bit(b_fraction_copy2.big_fraction[u], base, lost_bit);
				lost_bit = get_bit(b_fraction_copy2.big_fraction[u], 0);
				b_fraction_copy2.big_fraction[u] >>= 1;
			}
			if (get_bit(big_fraction[i], j))
				fraction_multiplication += b_fraction_copy2;
			
		}
	}

	BigDecimal result = integer_multiplication + integer_fraction_multiplication + fraction_integer_multiplication + fraction_multiplication;

	if (sign != b.sign && (!is_zero(result.big_fraction) || !is_zero(result.big_integer))) {
		result.sign = 1;
	}
	else {
		result.sign = 0;
	}

	return result;
}

const BigDecimal BigDecimal::operator/(const BigDecimal& b) const {
	if (b == BigDecimal("0")) {
		throw std::invalid_argument("Division by zero");
	}
	BigDecimal quotient("0", max(integer, b.integer), max(fraction, b.fraction));
	BigDecimal dividend, divisor;
	dividend = *this; divisor = b;
	quotient.sign = (sign != b.sign);
	dividend.sign = 0;
	divisor.sign = 0;
	BigDecimal temp_dividend = dividend;
	BigDecimal temp_divisor = divisor;
	BigDecimal result("0", dividend.integer, dividend.fraction);

	int max_integer_size = max(dividend.big_integer.size(), divisor.big_integer.size());
	BigDecimal current_dividend("0", max_integer_size, dividend.fraction);
	BigDecimal remainder("0", max_integer_size, dividend.fraction);
	for (int i = 0; i < dividend.big_integer.size(); i++) {
		remainder.big_integer.push_back(dividend.big_integer[i]);
		int current_quotient_digit = 0;
		while (remainder.compare_absolute_values(divisor) >= 0) {
			remainder -= divisor;
			current_quotient_digit++;
		}
		quotient.big_integer.push_back(current_quotient_digit);
	}

	BigDecimal current_fraction("0", dividend.integer, dividend.fraction);
	current_fraction.exp_starts = dividend.exp_starts;

	for (int i = 0; i < dividend.fraction; i++) {
		current_fraction.big_integer.push_back(0);
		int current_quotient_digit = 0;

		while (current_fraction.compare_absolute_values(divisor) >= 0) {
			current_fraction -= divisor;
			current_quotient_digit++;
		}
		quotient.big_fraction.push_back(current_quotient_digit);
	}
	return quotient;
}

void BigDecimal::operator*=(const BigDecimal& b) {
	*this = *this * b;
}


BigDecimal operator""_longnum(long double number) {
	ostringstream oss;
	oss << setprecision(std::numeric_limits<long double>::digits10) << number;
	std::string numStr = oss.str();
	return BigDecimal(numStr);
}


BigDecimal::~BigDecimal() {
}


