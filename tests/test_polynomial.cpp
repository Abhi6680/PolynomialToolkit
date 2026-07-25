#include <gtest/gtest.h>
#include "../include/Polynomial.h"
#include <stdexcept>

TEST(PolynomialParserTest, SimpleExpressionParsing) {
    Polynomial p = Polynomial::parse("3x^2 - 5x + 1");
    EXPECT_EQ(p.getDegree(), 2);
    EXPECT_DOUBLE_EQ(p.evaluate(2), 3);
}

TEST(PolynomialParserTest, ParenthesesAndExpansion) {
    Polynomial p1 = Polynomial::parse("(x + 1)^2");
    EXPECT_EQ(p1.getDegree(), 2);
    EXPECT_DOUBLE_EQ(p1.evaluate(0), 1.0);
    EXPECT_DOUBLE_EQ(p1.evaluate(2), 9.0);

    Polynomial p2 = Polynomial::parse("(2x - 3)(x + 2)");
    EXPECT_EQ(p2.getDegree(), 2);
    EXPECT_DOUBLE_EQ(p2.evaluate(3), 15.0);
}

TEST(PolynomialParserTest, DiagnosticsAndErrorHandling) {
    EXPECT_THROW(Polynomial::parse("(x + 1))"), std::invalid_argument);
    EXPECT_THROW(Polynomial::parse("((x + 1)"), std::invalid_argument);
    EXPECT_THROW(Polynomial::parse("3x^2 @ 5"), std::invalid_argument);
}

TEST(PolynomialCalculusTest, DerivativesAndIntegrals) {
    Polynomial p = Polynomial::parse("x^3 - 3x");

    Polynomial dev1 = p.derivative();
    EXPECT_DOUBLE_EQ(dev1.evaluate(2), 9.0);

    Polynomial dev2 = p.secondDerivative();
    EXPECT_DOUBLE_EQ(dev2.evaluate(3), 18.0);

    Polynomial pQuad = Polynomial::parse("x^2");
    EXPECT_NEAR(pQuad.definiteIntegral(0, 3), 9.0, 1e-5);
}

TEST(PolynomialCalculusTest, TangentAndNormalLines) {
    Polynomial p = Polynomial::parse("x^2");
    double m, c;
    p.tangentLine(1.0, m, c);

    EXPECT_DOUBLE_EQ(m, 2.0);
    EXPECT_DOUBLE_EQ(c, -1.0);
}