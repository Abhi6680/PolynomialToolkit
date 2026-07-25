#include <iostream>
#include "include/Polynomial.h"

int main() {
    Polynomial p1, p2;

    std::cout << "Enter degree then coefficients for P1:\n";
    std::cin >> p1;

    std::cout << "Enter degree then coefficients for P2:\n";
    std::cin >> p2;

    std::cout << "\nP1(x) = " << p1 << "\n";
    std::cout << "P2(x) = " << p2 << "\n\n";

    std::cout << "P1 + P2 = " << (p1 + p2) << "\n";
    std::cout << "P1 - P2 = " << (p1 - p2) << "\n";
    std::cout << "P1 * P2 = " << (p1 * p2) << "\n\n";

    std::cout << "Derivative of P1: " << p1.derivative() << "\n";
    std::cout << "Integral of P1: " << p1.indefiniteIntegral() << " + C\n\n";

    double a = 0.0, b = 2.0;
    std::cout << "Definite integral of P1 from " << a << " to " << b << ": " 
              << p1.definiteIntegral(a, b) << "\n";

    return 0;
}