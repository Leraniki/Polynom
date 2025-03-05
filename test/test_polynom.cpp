#include "polynom.h"

#include <gtest.h>


TEST(PolynomialsTest, MonomialAddition) {
    Polynomials p1("2x^2y");
    Polynomials p2("3x^2y");
    Polynomials sum = p1 + p2;
    ASSERT_EQ(sum.toString(), "5x^2y");
}

TEST(PolynomialsTest, MonomialSubtraction) {
    Polynomials p1("5x^3y^2");
    Polynomials p2("2x^3y^2");
    Polynomials diff = p1 - p2;
    ASSERT_EQ(diff.toString(), "3x^3y^2");
}

TEST(PolynomialsTest, ComplexMonomial) {
    Polynomials p("x^3y^2z^9");
    ASSERT_EQ(p.toString(), "x^3y^2z^9");
}

TEST(PolynomialsTest, MonomialWithCoefficient) {
    Polynomials p("4x^3y^2z^9");
    ASSERT_EQ(p.toString(), "4x^3y^2z^9");
}

TEST(PolynomialsTest, MixedMonomialsAndOtherTerms) {
    Polynomials p("2x^3y^2 + 3y - z + 5x^2");
    ASSERT_EQ(p.toString(), "2x^3y^2 + 3y - z + 5x^2");
}

TEST(PolynomialsTest, AddingMonomialsWithDifferentOrder) {
    Polynomials p1("x^2y + xy^2");
    ASSERT_EQ(p1.toString(), "x^2y + xy^2");
}

TEST(PolynomialsTest, AddingComplexMonomials) {
    Polynomials p1("2x^3y^2z^9");
    Polynomials p2("3x^3y^2z^9");
    Polynomials sum = p1 + p2;
    ASSERT_EQ(sum.toString(), "5x^3y^2z^9");
}

TEST(PolynomialsTest, SubtractingComplexMonomials) {
    Polynomials p1("5x^3y^2z^9");
    Polynomials p2("2x^3y^2z^9");
    Polynomials diff = p1 - p2;
    ASSERT_EQ(diff.toString(), "3x^3y^2z^9");
}

TEST(PolynomialsTest, InsertionWithExistingMonomial) {
    Polynomials p("2x^3y^2");
    p.insert({ {'x', 3}, {'y', 2} }, 5.0);
    ASSERT_EQ(p.toString(), "5x^3y^2");
}

TEST(PolynomialsTest, RemovalOfComplexMonomial) {
    Polynomials p("4x^3y^2z^9 + 2x - y");
    p.remove({ {'x', 3}, {'y', 2}, {'z', 9} });
    ASSERT_EQ(p.toString(), "2x - y");
}

TEST(PolynomialsTest, FindComplexMonomialCoefficient) {
    Polynomials p("4x^3y^2z^9 + 2x - y");
    std::map<char, int> monomial = { {'x', 3}, {'y', 2}, {'z', 9} };
    ASSERT_EQ(p.find(monomial), 4.0);
}

TEST(PolynomialsTest, AdditionOfComplexMonomialsWithOtherTerms) {
    Polynomials p1("2x^3y^2z^9 + 3x");
    Polynomials p2("5x^3y^2z^9 - x + 2y");
    Polynomials sum = p1 + p2;
    ASSERT_EQ(sum.toString(), "7x^3y^2z^9 + 2x + 2y");
}

TEST(PolynomialsTest, ParseComplexMonomialWithAndWithoutCoefficients) {
    Polynomials p1("x^2yz^3 + 5x^4y^2 + xy"); //With complex mono and without coeff
    ASSERT_EQ(p1.toString(), "x^2yz^3 + 5x^4y^2 + xy");

    Polynomials p2("3x^2yz^3 + 5x^4y^2 + 2xy"); //With complex mono and coeff
    ASSERT_EQ(p2.toString(), "3x^2yz^3 + 5x^4y^2 + 2xy");

    Polynomials p3("-x^2yz^3 - 5x^4y^2 - xy"); //Negative term
    ASSERT_EQ(p3.toString(), "-x^2yz^3 - 5x^4y^2 - xy");

}