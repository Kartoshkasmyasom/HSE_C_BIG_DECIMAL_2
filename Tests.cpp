//cpp_unit
#include "BigDecimal.h"
#include <iostream>

bool test_sum() {
	BigDecimal a("13"), b("4");
	BigDecimal res = a + b;
	BigDecimal expected("17");
	return res == expected;
}

bool test_fraction_sum() {
	BigDecimal a("0.5"), b("0.5");
	BigDecimal res = a + b;
	BigDecimal expected("1");
	return res == expected;
}

bool test_extra_addition_sum() {
	BigDecimal a("12.5", 8, 16), b("4.6", 8, 16);
	BigDecimal res = a + b;
	BigDecimal expected("17.1", 8, 16);
	return res == expected;
}

bool test_neative_sum() {
	BigDecimal a("-12.5", 8, 16), b("-4.6", 8, 16);
	BigDecimal res = a + b;
	BigDecimal expected("-17.1", 8, 16);

	BigDecimal a1("17.75"), b1("-15.5");
	BigDecimal res1 = a1 + b1;
	BigDecimal expected1("2.25");
	return (res == expected) && (res1 == expected1);
}

bool test_big_sum() {
	BigDecimal a("42343565615335436564574676575632455326577525.4242423", 200, 100), b("79824798759487598795879821749283749287.191923842424144", 200, 100);
	BigDecimal res = a + b;
	BigDecimal expected("42343645440134196052173472455454204610326812.616166142424144", 200, 100);
	return res == expected;
}

bool test_minus() {
	BigDecimal a1("1.25", 200, 100), b1("0.5", 200, 100);
	BigDecimal res1 = a1 - b1;
	BigDecimal expected1("0.75", 200, 100);
	//std::cout << (res1 == expected1) << std::endl;

	BigDecimal a2("0.25", 200, 100), b2("0.5", 200, 100);
	BigDecimal res2 = a2 - b2;
	BigDecimal expected2("-0.25", 200, 100);

	BigDecimal a3("15.68", 100, 200), b3("15.68", 100, 200);
	BigDecimal res3 = a3 - b3;
	BigDecimal expected3("0", 100, 200);

	BigDecimal a4("1"), b4("-56.8");
	BigDecimal res4 = a4 - b4;
	BigDecimal expected4("57.8");

	return (res1 == expected1) && (res2 == expected2) && (res3 == expected3) && (res4 == expected4);
}

bool test_equal() {
	BigDecimal a1("17.75"), b1("17.7500");
	BigDecimal a2("16.75"), b2("-16.75");
	BigDecimal a3("-15.24"), b3("-15.241", 8, 32);
	BigDecimal a4("543.5", 100), b4("543.5", 100, 100);
	return (a1 == b1) && !(a2 == b2) && !(a3 == b3) && (a4 == b4);
}

bool test_comparing_absolute_values() {
	BigDecimal a1("14.92"), b1("-15");
	BigDecimal a2("16", 8, 16), b2("16.00001", 8, 32);
	BigDecimal a3("17.75"), b3("17.7500");
	return a1.compare_absolute_values(b1) && a2.compare_absolute_values(b2) && !a3.compare_absolute_values(b3);

}

bool test_bool_operators() {
	BigDecimal a1("14.930000"), b1("14.93");
	BigDecimal a2("-5.5"), b2("5.5");
	BigDecimal a3("-0.15"), b3("-0.2");
	BigDecimal a4("0.15"), b4("0.2");
	BigDecimal a5("0.6"), b5("-12");
	return (a1 == b1) && (a2 != b2) && (a3 >= b3) && (a3 != b3) && !(a3 < b3) && (a4 < b4) && (a4 <= b4) && (a1 <= b1) && (a1 >= b1) && (a5  > b5);
}

bool test_integer_multiplication() {
	BigDecimal a1("6"), b1("5"), c1("30");
	BigDecimal a2("0"), b2("14"), c2("0");
	BigDecimal a3("-4"), b3("15"), c3("-60");
	BigDecimal a4("-4"), b4("-15"), c4("60");
	return (a1 * b1 == c1) && (a2 * b2 == c2) && (a3 * b3 == c3) && (a4 * b4 == c4);
}

bool test_integer_fraction_multiplication() {
	BigDecimal a1("0.25"), b1("2"), c1("0.5");
	BigDecimal a2("0.125"), b2("16"), c2("2");
	return (a1 * b1 == c1) && (a2 * b2 == c2);
}

bool test_fraction_fraction_multiplication() {
	BigDecimal a1("0.5"), b1("0.5"), c1("0.25");
	BigDecimal a2("0.5"), b2("14.0025"), c2("7.00125");
	BigDecimal a3("-1", 8, 32), b3("15.64", 8, 32), c3("-15.64", 8, 32);
	BigDecimal a4("-1", 8, 32), b4("-15.64", 8, 32), c4("15.64", 8, 32);
	return (a1 * b1 == c1) && (a2 * b2 == c2) && (a3 * b3 == c3) && (a4 * b4 == c4);
}

bool test_division() {
	BigDecimal a1("4"), b1("2"), c1("2");
	return (a1 / b1 == c1);
}


int main(int argc, char **argv) {
	if (test_sum())
		std::cout << "test1: OK\n";
	else
		std::cout << "test1: FAIL\n";
	if (test_fraction_sum())
		std::cout << "test2: OK\n";
	else
		std::cout << "test2: FAIL\n";
	if (test_extra_addition_sum())
		std::cout << "test3: OK\n";
	else
		std::cout << "test3: FAIL\n";
	if(test_neative_sum())
		std::cout << "test4: OK\n";
	else
		std::cout << "test4: FAIL\n";
	if(test_big_sum())
		std::cout << "test5: OK\n";
	else
		std::cout << "test5: FAIL\n";
	if (test_minus())
		std::cout << "test6: OK\n";
	else
		std::cout << "test6: FAIL\n";
	if (test_comparing_absolute_values())
		std::cout << "test7: OK\n";
	else
		std::cout << "test7: FAIL\n";
	if(test_equal())
		std::cout << "test8: OK\n";
	else
		std::cout << "test8: FAIL\n";
	if (test_bool_operators()) 
		std::cout << "test9: OK\n";
	else
		std::cout << "test9: FAIL\n";
	if(test_integer_multiplication())
		std::cout << "test10: OK\n";
	else
		std::cout << "test10: FAIL\n";
	if (test_integer_fraction_multiplication())
		std::cout << "test11: OK\n";
	else
		std::cout << "test11: FAIL\n";
	if (test_fraction_fraction_multiplication())
		std::cout << "test12: OK\n";
	else
		std::cout << "test12: FAIL\n";
	if (test_division())
		std::cout << "test13: OK\n";
	else
		std::cout << "test13: FAIL\n";
}

//int main() {
//	//freopen("input.txt", "r", stdin);
//	ios::sync_with_stdio(0);
//	cin.tie(nullptr);
//	// test of convertation of integer
//	/*BigDecimal number1("12");
//	BigDecimal number2("19342813113834066795298815.15", 100);
//	BigDecimal number3;
//	BigDecimal number4("1242352415624543523.5", 100);
//	BigDecimal number5("0.6");*/
//	//BigDecimal strange_negative_integer("-0.0", 100, 100);
//
//	// test of convertation of fraction
//	/*BigDecimal number6("0.560");
//	BigDecimal number7("15.625", 100);
//	BigDecimal number8("17.7", 100, 100);*/
//	//BigDecimal number9("0.0009765625", 100, 100);
//	//BigDecimal number10("0.3", 100, 100);
//	//BigDecimal negative("-15.125", 100, 100);
//
//	//test of addition
//
//	/*BigDecimal sum_works_int_a("13"), sum_works_int_b("4");
//	BigDecimal sum_works_int_c = sum_works_int_a + sum_works_int_b;*/
//
//	/*BigDecimal sum_works_frac_a("0.5"), sum_works_frac_b("0.5");
//	BigDecimal sum_works_frac_c = sum_works_frac_a + sum_works_frac_b;*/
//
//	/*BigDecimal sum_mixed_a("12.5", 8, 16), sum_mixed_b("4.6", 8, 16);
//	BigDecimal sum_works_frac_c = sum_mixed_a + sum_mixed_b;*/
//
//	/*BigDecimal sum_negative_a("-12.5", 8, 16), sum_negative_b("-4.6", 8, 16);
//	BigDecimal sum_works_frac_c = sum_negative_a + sum_negative_b;*/
//
//	/*BigDecimal sum_big_mixed_a("42343565615335436564574676575632455326577525.4242423", 200, 100);
//	BigDecimal sum_big_mixed_b("79824798759487598795879821749283749287.191923842424144", 200, 100);
//	BigDecimal sum_big_mixed_c = sum_big_mixed_a + sum_big_mixed_b;*/
//
//}
