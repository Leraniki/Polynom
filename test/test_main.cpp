#include <gtest.h>
#include <polynom.h>
int main(int argc, char **argv)
{
	std::string poly1Str, poly2Str;

	std::cout << "Enter the first polynomial: ";
	std::getline(std::cin, poly1Str);

	std::cout << "Enter the second polynomial: ";
	std::getline(std::cin, poly2Str);


	Polynomials poly1(poly1Str);
	Polynomials poly2(poly2Str);

	std::cout << "First polynomial: " << poly1 << std::endl;
	std::cout << "Second polynomial: " << poly2 << std::endl;

	Polynomials sum = poly1 + poly2;
	std::cout << "Sum: " << sum << std::endl;

	Polynomials diff = poly1 - poly2;
	std::cout << "Difference: " << diff << std::endl;

	Polynomials  mul = poly1 * poly2;
	std::cout << "Multiplication: " << mul << std::endl;

	Polynomials div_const = poly1 / 2;
	std::cout << "Division by const: " << div_const << std::endl;

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();

}
