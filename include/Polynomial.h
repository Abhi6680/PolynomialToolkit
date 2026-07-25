#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

class Polynomial {
private:
    std::map<int, double> terms;

    void cleanupZeros();
    static Polynomial parseSimple(const std::string& expr);

public:
    Polynomial() = default;
    Polynomial(const std::map<int, double>& terms) : terms(terms) { cleanupZeros(); }

    static Polynomial parse(const std::string& expr);
    Polynomial derivative() const;
    double evaluate(double x) const;
    int getDegree() const;

    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;
    bool operator==(const Polynomial& other) const;

    std::string toString() const;
};

#endif