#include "../include/Polynomial.h"
#include <cmath>
#include <algorithm>

void Polynomial::cleanup() {
    delete[] coeffs;
    coeffs = nullptr;
}

Polynomial::Polynomial() : degree(0) {
    coeffs = new double[1]{0.0};
}

Polynomial::Polynomial(int deg) : degree(deg < 0 ? 0 : deg) {
    coeffs = new double[degree + 1]{0.0};
}

Polynomial::Polynomial(const double* arr, int deg) : degree(deg < 0 ? 0 : deg) {
    coeffs = new double[degree + 1];
    for (int i = 0; i <= degree; ++i) {
        coeffs[i] = arr[i];
    }
}

Polynomial::Polynomial(const Polynomial& other) : degree(other.degree) {
    coeffs = new double[degree + 1];
    for (int i = 0; i <= degree; ++i) {
        coeffs[i] = other.coeffs[i];
    }
}

Polynomial::~Polynomial() {
    cleanup();
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this != &other) {
        cleanup();
        degree = other.degree;
        coeffs = new double[degree + 1];
        for (int i = 0; i <= degree; ++i) {
            coeffs[i] = other.coeffs[i];
        }
    }
    return *this;
}

int Polynomial::getDegree() const {
    return degree;
}

double Polynomial::evaluate(double x) const {
    double result = coeffs[degree];
    for (int i = degree - 1; i >= 0; --i) {
        result = result * x + coeffs[i];
    }
    return result;
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
    int maxDeg = std::max(degree, other.degree);
    Polynomial result(maxDeg);

    for (int i = 0; i <= maxDeg; ++i) {
        double c1 = (i <= degree) ? coeffs[i] : 0.0;
        double c2 = (i <= other.degree) ? other.coeffs[i] : 0.0;
        result.coeffs[i] = c1 + c2;
    }
    return result;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
    int maxDeg = std::max(degree, other.degree);
    Polynomial result(maxDeg);

    for (int i = 0; i <= maxDeg; ++i) {
        double c1 = (i <= degree) ? coeffs[i] : 0.0;
        double c2 = (i <= other.degree) ? other.coeffs[i] : 0.0;
        result.coeffs[i] = c1 - c2;
    }
    return result;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
    int newDegree = degree + other.degree;
    Polynomial result(newDegree);

    for (int i = 0; i <= degree; ++i) {
        for (int j = 0; j <= other.degree; ++j) {
            result.coeffs[i + j] += coeffs[i] * other.coeffs[j];
        }
    }
    return result;
}

Polynomial Polynomial::derivative() const {
    if (degree == 0) return Polynomial();

    Polynomial res(degree - 1);
    for (int i = 1; i <= degree; ++i) {
        res.coeffs[i - 1] = coeffs[i] * i;
    }
    return res;
}

Polynomial Polynomial::indefiniteIntegral(double C) const {
    Polynomial res(degree + 1);
    res.coeffs[0] = C;
    for (int i = 0; i <= degree; ++i) {
        res.coeffs[i + 1] = coeffs[i] / (i + 1);
    }
    return res;
}

double Polynomial::definiteIntegral(double a, double b) const {
    Polynomial F = indefiniteIntegral();
    return F.evaluate(b) - F.evaluate(a);
}

std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
    bool first = true;
    for (int i = p.degree; i >= 0; --i) {
        if (p.coeffs[i] == 0.0 && p.degree > 0) continue;

        if (!first && p.coeffs[i] > 0) os << " + ";
        else if (p.coeffs[i] < 0) os << " - ";

        double absCoeff = std::abs(p.coeffs[i]);
        if (absCoeff != 1.0 || i == 0) os << absCoeff;

        if (i > 0) {
            os << "x";
            if (i > 1) os << "^" << i;
        }
        first = false;
    }
    if (first) os << "0";
    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& p) {
    int deg;
    is >> deg;

    p.cleanup();
    p.degree = (deg < 0) ? 0 : deg;
    p.coeffs = new double[p.degree + 1];

    for (int i = 0; i <= p.degree; ++i) {
        is >> p.coeffs[i];
    }
    return is;
}