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

Polynomial Polynomial::secondDerivative() const {
    return derivative().derivative();
}

Polynomial Polynomial::nthDerivative(int n) const {
    Polynomial res = *this;
    for (int i = 0; i < n; ++i) {
        res = res.derivative();
        if (res.degree == 0 && res.coeffs[0] == 0.0) break;
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

void Polynomial::tangentLine(double x0, double& m, double& c) const {
    m = derivative().evaluate(x0);
    double y0 = evaluate(x0);
    c = y0 - m * x0;
}

void Polynomial::normalLine(double x0, double& m, double& c) const {
    double derivVal = derivative().evaluate(x0);
    if (std::abs(derivVal) < 1e-9) {
        m = 1e9;
    } else {
        m = -1.0 / derivVal;
    }
    double y0 = evaluate(x0);
    c = y0 - m * x0;
}

double Polynomial::findRoot(double initialGuess, int maxIter, double tol) const {
    double x = initialGuess;
    Polynomial d = derivative();

    for (int i = 0; i < maxIter; ++i) {
        double fx = evaluate(x);
        double dfx = d.evaluate(x);

        if (std::abs(dfx) < 1e-12) break;

        double nextX = x - (fx / dfx);
        if (std::abs(nextX - x) < tol) return nextX;

        x = nextX;
    }
    return x;
}

void Polynomial::findCriticalPoints(double searchStart, double searchEnd, double step) const {
    Polynomial d = derivative();
    Polynomial d2 = secondDerivative();

    for (double x = searchStart; x <= searchEnd; x += step) {
        double f_prime_x = d.evaluate(x);
        double f_prime_next = d.evaluate(x + step);

        if (f_prime_x * f_prime_next <= 0) {
            double critPt = d.findRoot(x + step / 2.0);
            double secondDerivVal = d2.evaluate(critPt);

            std::cout << "Critical Point at x = " << critPt << ", P(x) = " << evaluate(critPt) << " -> ";
            if (secondDerivVal > 1e-7) {
                std::cout << "Local Minimum\n";
            } else if (secondDerivVal < -1e-7) {
                std::cout << "Local Maximum\n";
            } else {
                std::cout << "Inflexion / Saddle Point\n";
            }
        }
    }
}

Polynomial Polynomial::compose(const Polynomial& g) const {
    Polynomial result;
    Polynomial currentPower(0);
    currentPower.coeffs[0] = 1.0;

    for (int i = 0; i <= degree; ++i) {
        Polynomial term = currentPower;
        for (int k = 0; k <= term.degree; ++k) {
            term.coeffs[k] *= coeffs[i];
        }
        result = result + term;
        currentPower = currentPower * g;
    }
    return result;
}

void Polynomial::divide(const Polynomial& divisor, Polynomial& quotient, Polynomial& remainder) const {
    if (divisor.degree == 0 && divisor.coeffs[0] == 0.0) {
        return;
    }

    if (degree < divisor.degree) {
        quotient = Polynomial();
        remainder = *this;
        return;
    }

    remainder = *this;
    quotient = Polynomial(degree - divisor.degree);

    while (remainder.degree >= divisor.degree && !(remainder.degree == 0 && remainder.coeffs[0] == 0.0)) {
        int degDiff = remainder.degree - divisor.degree;
        double scale = remainder.coeffs[remainder.degree] / divisor.coeffs[divisor.degree];

        quotient.coeffs[degDiff] = scale;

        Polynomial temp(degDiff);
        temp.coeffs[degDiff] = scale;
        Polynomial sub = temp * divisor;

        remainder = remainder - sub;

        int newDeg = 0;
        for (int i = remainder.degree; i >= 0; --i) {
            if (std::abs(remainder.coeffs[i]) > 1e-9) {
                newDeg = i;
                break;
            }
        }
        remainder.degree = newDeg;
    }
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