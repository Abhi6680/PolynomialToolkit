#include <iostream>
#include "include/Polynomial.h"

int main() {
    Polynomial p1, p2;

    std::cout << "Enter degree and coeffs for P1:\n";
    std::cin >> p1;

    std::cout << "Enter degree and coeffs for P2:\n";
    std::cin >> p2;

    std::cout << "\nP1(x) = " << p1 << "\n";
    std::cout << "P2(x) = " << p2 << "\n\n";

    std::cout << "1st Derivative: " << p1.derivative() << "\n";
    std::cout << "2nd Derivative: " << p1.secondDerivative() << "\n";
    std::cout << "3rd Derivative: " << p1.nthDerivative(3) << "\n\n";

    double m, c;
    p1.tangentLine(1.0, m, c);
    std::cout << "Tangent at x=1: y = " << m << "x + " << c << "\n";

    p1.normalLine(1.0, m, c);
    std::cout << "Normal at x=1: y = " << m << "x + " << c << "\n\n";

    std::cout << "Root near x=2: " << p1.findRoot(2.0) << "\n\n";

    std::cout << "Critical Points [-5, 5]:\n";
    p1.findCriticalPoints(-5.0, 5.0);
    std::cout << "\n";

    Polynomial quotient, remainder;
    p1.divide(p2, quotient, remainder);
    std::cout << "P1 / P2 Quotient: " << quotient << "\n";
    std::cout << "P1 / P2 Remainder: " << remainder << "\n\n";

    std::cout << "Composition P1(P2(x)): " << p1.compose(p2) << "\n";

    return 0;
}