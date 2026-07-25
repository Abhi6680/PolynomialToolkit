#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>

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
    double evaluate(double x) const;

    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;

    Polynomial derivative() const;
    Polynomial indefiniteIntegral(double C = 0.0) const;
    double definiteIntegral(double a, double b) const;

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);
    friend std::istream& operator>>(std::istream& is, Polynomial& p);
};

#endif