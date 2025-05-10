#include "polynom.h" // Убедитесь, что имя вашего хедера правильное
#include <gtest.h> // Или #include <gtest.h> в зависимости от вашей установки
#include <map>

// --- Тесты, которые должны были проходить и раньше ---
// (Проверяем, что они все еще проходят)

TEST(PolynomialsTest, MonomialAddition) {
    Polynomials p1("2x^2y");
    Polynomials p2("3x^2y");
    Polynomials sum = p1 + p2;
    // Ожидаемый результат не меняется, т.к. один тип монома
    ASSERT_EQ(sum.toString(), "5x^2y");
}

TEST(PolynomialsTest, MonomialSubtraction) {
    Polynomials p1("5x^3y^2");
    Polynomials p2("2x^3y^2");
    Polynomials diff = p1 - p2;
    // Ожидаемый результат не меняется
    ASSERT_EQ(diff.toString(), "3x^3y^2");
}

TEST(PolynomialsTest, ComplexMonomial) {
    Polynomials p("x^3y^2z^9");
    // Ожидаемый результат не меняется
    ASSERT_EQ(p.toString(), "x^3y^2z^9");
}

TEST(PolynomialsTest, MonomialWithCoefficient) {
    Polynomials p("4x^3y^2z^9");
    // Ожидаемый результат не меняется
    ASSERT_EQ(p.toString(), "4x^3y^2z^9");
}

TEST(PolynomialsTest, AddingComplexMonomials) {
    Polynomials p1("2x^3y^2z^9");
    Polynomials p2("3x^3y^2z^9");
    Polynomials sum = p1 + p2;
    // Ожидаемый результат не меняется
    ASSERT_EQ(sum.toString(), "5x^3y^2z^9");
}

TEST(PolynomialsTest, SubtractingComplexMonomials) {
    Polynomials p1("5x^3y^2z^9");
    Polynomials p2("2x^3y^2z^9");
    Polynomials diff = p1 - p2;
    // Ожидаемый результат не меняется
    ASSERT_EQ(diff.toString(), "3x^3y^2z^9");
}

TEST(PolynomialsTest, InsertionWithExistingMonomial) {
    Polynomials p("2x^3y^2");
    p.insert({ {'x', 3}, {'y', 2} }, 5.0);
    // simplify должен объединить, результат не меняется
    ASSERT_EQ(p.toString(), "5x^3y^2");
}

TEST(PolynomialsTest, FindComplexMonomialCoefficient) {
    // Find не зависит от порядка
    Polynomials p("4x^3y^2z^9 + 2x - y"); // После simplify станет "2x - y + 4x^3y^2z^9" или похоже
    std::map<char, int> monomial = { {'x', 3}, {'y', 2}, {'z', 9} };
    ASSERT_DOUBLE_EQ(p.find(monomial), 4.0); // Используем DOUBLE_EQ для double
}


// --- Тесты, которые падали из-за порядка ---
// (Исправляем ожидаемые строки на канонический вид)

TEST(PolynomialsTest, MixedMonomialsAndOtherTerms_CanonicalOrder) {
    // Исходный: "2x^3y^2 + 3y - z + 5x^2"
    // Члены: {5, {x:2}}, {2, {x:3, y:2}}, {3, {y:1}}, {-1, {z:1}} (примерный отсортированный порядок)
    Polynomials p("2x^3y^2 + 3y - z + 5x^2");
    // Ожидаем строку, соответствующую лексикографической сортировке термов
    ASSERT_EQ(p.toString(), "5x^2 + 2x^3y^2 + 3y - z");
}

TEST(PolynomialsTest, AddingMonomialsWithDifferentOrder_CanonicalOrder) {
    // Исходный: "x^2y + xy^2"
    // Члены: {1, {x:1, y:2}}, {1, {x:2, y:1}} (отсортированный порядок)
    Polynomials p1("x^2y + xy^2");
    // Ожидаем строку в каноническом (отсортированном) порядке
    ASSERT_EQ(p1.toString(), "xy^2 + x^2y");
}

TEST(PolynomialsTest, AdditionOfComplexMonomialsWithOtherTerms_CanonicalOrder) {
    Polynomials p1("2x^3y^2z^9 + 3x");      // Члены: {3, {x:1}}, {2, {x:3,y:2,z:9}}
    Polynomials p2("5x^3y^2z^9 - x + 2y"); // Члены: {-1, {x:1}}, {5, {x:3,y:2,z:9}}, {2, {y:1}}
    Polynomials sum = p1 + p2;
    // После simplify: {2, {x:1}}, {7, {x:3,y:2,z:9}}, {2, {y:1}}
    // toString должен выдать их в этом порядке
    ASSERT_EQ(sum.toString(), "2x + 7x^3y^2z^9 + 2y");
}

TEST(PolynomialsTest, ParseComplexMonomialWithAndWithoutCoefficients_CanonicalOrder) {
    // p1("x^2yz^3 + 5x^4y^2 + xy");
    // Члены: {1, {x:1,y:1}}, {1, {x:2,y:1,z:3}}, {5, {x:4,y:2}}
    Polynomials p1("x^2yz^3 + 5x^4y^2 + xy");
    ASSERT_EQ(p1.toString(), "xy + x^2yz^3 + 5x^4y^2");

    // p2("3x^2yz^3 + 5x^4y^2 + 2xy");
    // Члены: {2, {x:1,y:1}}, {3, {x:2,y:1,z:3}}, {5, {x:4,y:2}}
    Polynomials p2("3x^2yz^3 + 5x^4y^2 + 2xy");
    ASSERT_EQ(p2.toString(), "2xy + 3x^2yz^3 + 5x^4y^2");

    // p3("-x^2yz^3 - 5x^4y^2 - xy");
    // Члены: {-1, {x:1,y:1}}, {-1, {x:2,y:1,z:3}}, {-5, {x:4,y:2}}
    Polynomials p3("-x^2yz^3 - 5x^4y^2 - xy");
    ASSERT_EQ(p3.toString(), "-xy - x^2yz^3 - 5x^4y^2");
}

TEST(PolynomialsTest, RemovalOfComplexMonomial_CanonicalOrder) {
    // Исходный: "4x^3y^2z^9 + 2x - y"
    // После simplify: "2x - y + 4x^3y^2z^9" (вероятно)
    Polynomials p("4x^3y^2z^9 + 2x - y");
    p.remove({ {'x', 3}, {'y', 2}, {'z', 9} }); // Удаляем {4, {x:3,y:2,z:9}}
    // Остаются: {2, {x:1}}, {-1, {y:1}}
    // simplify (вызывается в remove или нет? Если нет, порядок может быть как в p)
    // Если simplify вызывается в remove (что логично), то порядок будет "2x - y"
    ASSERT_EQ(p.toString(), "2x - y");
}


// --- Дополнительные тесты для надежности ---

TEST(PolynomialsTest, ConstantTerms) {
    Polynomials p1("5");
    Polynomials p2("-3");
    Polynomials sum = p1 + p2;
    ASSERT_EQ(sum.toString(), "2");
    Polynomials p3("x + 5 + 2x - 3");
    // Ожидаемый канонический вид: константа + переменные по порядку
    ASSERT_EQ(p3.toString(), "2 + 3x");
}

TEST(PolynomialsTest, CancellationToZero) {
    Polynomials p1("x^2y - x^2y");
    ASSERT_EQ(p1.toString(), "0");
    Polynomials p2("3x + 5y - x - 5y - 2x");
    ASSERT_EQ(p2.toString(), "0");
}

TEST(PolynomialsTest, NegativeFirstTermCanonical) {
    Polynomials p1("y - 3x^2");
    // Ожидаемый канонический порядок: -3x^2 + y
    ASSERT_EQ(p1.toString(), "-3x^2 + y");
}

TEST(PolynomialsTest, FloatingPointCoefficients) {
    Polynomials p1("1.5x^2 + 2.5y - 0.5x^2");
    // Ожидается: 1.0x^2 + 2.5y -> x^2 + 2.5y
    ASSERT_EQ(p1.toString(), "x^2 + 2.5y");
    Polynomials p2("3.1xy - 1.1xy + z");
    // Ожидается: 2.0xy + z -> 2xy + z
    ASSERT_EQ(p2.toString(), "2xy + z");
}

TEST(PolynomialsTest, ZeroPolynomial) {
    Polynomials p1("0");
    ASSERT_EQ(p1.toString(), "0");
    Polynomials p2; // Конструктор по умолчанию
    ASSERT_EQ(p2.toString(), "0");
    Polynomials p3("x");
    Polynomials p4("-x");
    Polynomials sum = p3 + p4;
    ASSERT_EQ(sum.toString(), "0");
}

TEST(PolynomialsTest, MultiplicationByZero) {
    Polynomials p1("3x^2 - y");
    Polynomials p0; // Нулевой полином
    Polynomials p0_str("0");

    EXPECT_EQ((p1 * p0).toString(), "0");
    EXPECT_EQ((p0 * p1).toString(), "0");
    EXPECT_EQ((p1 * p0_str).toString(), "0");
    EXPECT_EQ((p0_str * p1).toString(), "0");
    EXPECT_EQ((p0 * p0).toString(), "0");
}

TEST(PolynomialsTest, MultiplicationByIdentity) {
    Polynomials p1("3x^2 - y + 5");
    Polynomials pId("1");
    // Ожидаем канонический вид p1
    // Константа идет первой, потом x^2, потом y
    ASSERT_EQ((p1 * pId).toString(), "5 + 3x^2 - y");
    ASSERT_EQ((pId * p1).toString(), "5 + 3x^2 - y");
}

TEST(PolynomialsTest, MultiplicationMonomialByMonomial) {
    Polynomials m1("2x^2y");
    Polynomials m2("3xz^3");
    // (2x^2y) * (3xz^3) = 6x^(2+1)y^(1)z^3 = 6x^3yz^3
    ASSERT_EQ((m1 * m2).toString(), "6x^3yz^3");

    Polynomials m3("-5a");
    Polynomials m4("4a^2b");
    // (-5a) * (4a^2b) = -20a^3b
    ASSERT_EQ((m3 * m4).toString(), "-20a^3b");
}

TEST(PolynomialsTest, MultiplicationMonomialByPolynomial) {
    Polynomials m1("2x");
    Polynomials p1("3y^2 - 5z + 1");
    // 2x * (3y^2 - 5z + 1) = 6xy^2 - 10xz + 2x
    // Канонический вид: 2x + 6xy^2 - 10xz (сортируем по первой переменной)
    ASSERT_EQ((m1 * p1).toString(), "2x + 6xy^2 - 10xz");
    // Проверим и наоборот (коммутативность)
    ASSERT_EQ((p1 * m1).toString(), "2x + 6xy^2 - 10xz");
}

TEST(PolynomialsTest, MultiplicationPolynomialByPolynomial) {
    Polynomials p1("x + 2");
    Polynomials p2("y - 3");
    // (x + 2)(y - 3) = xy - 3x + 2y - 6
    // Канонический вид: -6 - 3x + xy + 2y
    ASSERT_EQ((p1 * p2).toString(), "-6 - 3x + xy + 2y");

    Polynomials p3("a - b");
    Polynomials p4("a + b");
    // (a - b)(a + b) = a^2 + ab - ab - b^2 = a^2 - b^2
    // Канонический вид: a^2 - b^2
    ASSERT_EQ((p3 * p4).toString(), "a^2 - b^2");
}

TEST(PolynomialsTest, MultiplicationWithFloatingPoint) {
    Polynomials p1("1.5x + 2");
    Polynomials p2("2x - 1");
    // (1.5x + 2)(2x - 1) = 3x^2 - 1.5x + 4x - 2 = 3x^2 + 2.5x - 2
    // Канонический вид: -2 + 2.5x + 3x^2
    ASSERT_EQ((p1 * p2).toString(), "-2 + 2.5x + 3x^2");
}

TEST(PolynomialsTest, MultiplicationResultingInZero) {
    Polynomials p1("x + y");
    Polynomials p2("0");
    ASSERT_EQ((p1 * p2).toString(), "0");

    // Не самый очевидный, но возможный случай при сложных умножениях
    Polynomials p3("x - 1");
    Polynomials p4("x + 1"); // x^2 - 1
    Polynomials p5("1");
    Polynomials p6("-x^2");
    Polynomials intermediate = p3 * p4; // x^2 - 1
    Polynomials final_mul = intermediate + p5 + p6; // (x^2 - 1) + 1 - x^2 = 0
    // Проверим напрямую через сложение/вычитание
    Polynomials p7("x^2 - 1 + 1 - x^2"); // Конструктор вызовет simplify
    ASSERT_EQ(p7.toString(), "0");
}

TEST(PolynomialsTest, MultiplicationCommutativity) {
    Polynomials p1("x^2 - 3y + z");
    Polynomials p2("2x + y^3");
    // Нам не нужно знать точный результат, просто проверяем, что p1*p2 == p2*p1
    // Так как simplify приводит к канонической форме, строки должны совпасть
    ASSERT_EQ((p1 * p2).toString(), (p2 * p1).toString());
}


// =========================================
// Тесты для operator/ (Деление на константу)
// =========================================

TEST(PolynomialsTest, DivisionByZeroThrows) {
    Polynomials p1("3x^2 - y");
    // Проверяем точный ноль
    ASSERT_THROW(p1 / 0.0, std::domain_error);
    // Проверяем близкое к нулю значение
    ASSERT_THROW(p1 / 1e-15, std::domain_error);
}

TEST(PolynomialsTest, DivisionOfZero) {
    Polynomials p0;
    Polynomials p0_str("0");
    ASSERT_EQ((p0 / 5.0).toString(), "0");
    ASSERT_EQ((p0_str / -2.5).toString(), "0");
}

TEST(PolynomialsTest, DivisionByIdentity) {
    Polynomials p1("3x^2 - y + 5");
    // Канонический вид: 5 + 3x^2 - y
    ASSERT_EQ((p1 / 1.0).toString(), "5 + 3x^2 - y");
}

TEST(PolynomialsTest, DivisionByNegativeOne) {
    Polynomials p1("3x^2 - y + 5");
    // Ожидаем: -5 - 3x^2 + y (смена знаков и канонический порядок)
    ASSERT_EQ((p1 / -1.0).toString(), "-5 - 3x^2 + y");
}

TEST(PolynomialsTest, DivisionSimpleIntegerResult) {
    Polynomials p1("10x^4 - 6xy + 8");
    // Делим на 2: 5x^4 - 3xy + 4
    // Канонический вид: 4 - 3xy + 5x^4
    ASSERT_EQ((p1 / 2.0).toString(), "4 - 3xy + 5x^4");

    Polynomials p2("-9a^2 + 12b");
    // Делим на -3: 3a^2 - 4b
    // Канонический вид: 3a^2 - 4b
    ASSERT_EQ((p2 / -3.0).toString(), "3a^2 - 4b");
}

TEST(PolynomialsTest, DivisionFloatingPointResult) {
    Polynomials p1("5x^2 - 2y + 1");
    // Делим на 2: 2.5x^2 - y + 0.5
    // Канонический вид: 0.5 + 2.5x^2 - y
    ASSERT_EQ((p1 / 2.0).toString(), "0.5 + 2.5x^2 - y");

    Polynomials p2("3a + 4b");
    // Делим на -4: -0.75a - b
    // Канонический вид: -0.75a - b
    ASSERT_EQ((p2 / -4.0).toString(), "-0.75a - b");
}

TEST(PolynomialsTest, DivisionResultingInZeroTerms) {
    // Используем маленькие коэффициенты, которые simplify должен удалить после деления
    // Зависит от epsilon в simplify
    const double epsilon = 1e-9;
    Polynomials p1(std::to_string(epsilon / 2.0) + "x^2 - " + std::to_string(epsilon * 5.0) + "y");
    // После деления на 10.0 коэффициенты станут еще меньше epsilon
    ASSERT_EQ((p1 / 10.0).toString(), "0");

    Polynomials p2("5x");
    // Делим на большую константу
    ASSERT_EQ((p2 / 1e12).toString(), "0"); // Коэффициент будет 5e-12, что меньше epsilon=1e-9
}