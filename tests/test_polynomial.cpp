#include <gtest/gtest.h>
#include "Polynomial.h"

TEST(PolynomialTest, ZeroPolynomial) {
    Polynomial p = Polynomial::parse("0");
    EXPECT_EQ(p.getDegree(), 0);
    EXPECT_EQ(p.evaluate(10.0), 0.0);
    EXPECT_EQ(p.derivative().toString(), "0");
}

TEST(PolynomialTest, ParenthesesExpansion) {
    Polynomial p = Polynomial::parse("(x + 1)^2");
    EXPECT_EQ(p.getDegree(), 2);
    EXPECT_EQ(p.evaluate(2.0), 9.0);

    Polynomial deriv = p.derivative();
    EXPECT_EQ(deriv.evaluate(2.0), 6.0);
}

TEST(PolynomialTest, HighDegreePolynomial) {
    Polynomial p = Polynomial::parse("x^100 - 5x^50 + 10");
    EXPECT_EQ(p.getDegree(), 100);
    EXPECT_EQ(p.evaluate(1.0), 6.0);

    Polynomial deriv = p.derivative();
    EXPECT_EQ(deriv.getDegree(), 99);
}

TEST(PolynomialTest, PolynomialMultiplication) {
    Polynomial p1 = Polynomial::parse("x + 2");
    Polynomial p2 = Polynomial::parse("x - 2");
    Polynomial result = p1 * p2;

    EXPECT_EQ(result.toString(), "x^2 - 4");
    EXPECT_EQ(result.evaluate(3.0), 5.0);
}

TEST(PolynomialTest, ConstantDerivative) {
    Polynomial p = Polynomial::parse("42");
    EXPECT_EQ(p.derivative().toString(), "0");
}