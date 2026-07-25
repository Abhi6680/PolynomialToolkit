#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <cmath>
#include <string>

class Polynomial {
private:
    double* coeffs;
    int degree;

    void cleanup();

public:
    Polynomial();
    Polynomial(int deg);
    Polynomial(const double* arr, int deg);
    Polynomial(const Polynomial& other);
    ~Polynomial();

    Polynomial& operator=(const Polynomial& other);

    int getDegree() const;
    bool isZero() const;
    double evaluate(double x) const;

    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;

    Polynomial derivative() const;
    Polynomial secondDerivative() const;
    Polynomial nthDerivative(int n) const;

    Polynomial indefiniteIntegral(double C = 0.0) const;
    double definiteIntegral(double a, double b) const;

    void tangentLine(double x0, double& m, double& c) const;
    void normalLine(double x0, double& m, double& c) const;

    double findRoot(double initialGuess, int maxIter = 1000, double tol = 1e-7) const;
    void findCriticalPoints(double searchStart, double searchEnd, double step = 0.1) const;

    Polynomial compose(const Polynomial& g) const;
    void divide(const Polynomial& divisor, Polynomial& quotient, Polynomial& remainder) const;

    // Advanced CAS Extensions
    Polynomial gcd(const Polynomial& other) const;
    void printFactors() const;
    static Polynomial parse(const std::string& expr);

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);
    friend std::istream& operator>>(std::istream& is, Polynomial& p);
};

#endif