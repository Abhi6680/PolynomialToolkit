#include <iostream>
#include <limits>
#include "include/Polynomial.h"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Polynomial p1, p2;
    int choice = 0;

    std::cout << "===========================================\n";
    std::cout << "      POLYNOMIAL & CALCULUS TOOLKIT        \n";
    std::cout << "===========================================\n\n";

    std::cout << "Enter primary Polynomial P1:\n";
    std::cin >> p1;

    while (true) {
        std::cout << "\n-------------------------------------------\n";
        std::cout << "Current P1(x) = " << p1 << "\n";
        std::cout << "-------------------------------------------\n";
        std::cout << "1. Add another polynomial (P1 + P2)\n";
        std::cout << "2. Subtract another polynomial (P1 - P2)\n";
        std::cout << "3. Multiply by another polynomial (P1 * P2)\n";
        std::cout << "4. Divide by another polynomial (P1 / P2)\n";
        std::cout << "5. Derivative (1st, 2nd, or Nth)\n";
        std::cout << "6. Integration (Indefinite & Definite)\n";
        std::cout << "7. Evaluate at x\n";
        std::cout << "8. Tangent Line at x\n";
        std::cout << "9. Normal Line at x\n";
        std::cout << "10. Find Root (Newton-Raphson)\n";
        std::cout << "11. Find Critical Points\n";
        std::cout << "12. Composition P1(P2(x))\n";
        std::cout << "13. Re-enter Primary Polynomial P1\n";
        std::cout << "14. Exit\n";
        std::cout << "Select an option (1-14): ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid selection. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }

        if (choice == 14) {
            std::cout << "Exiting toolkit. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                std::cout << "Enter Polynomial P2 to add:\n";
                std::cin >> p2;
                std::cout << "Result: " << (p1 + p2) << "\n";
                break;
            }
            case 2: {
                std::cout << "Enter Polynomial P2 to subtract:\n";
                std::cin >> p2;
                std::cout << "Result: " << (p1 - p2) << "\n";
                break;
            }
            case 3: {
                std::cout << "Enter Polynomial P2 to multiply:\n";
                std::cin >> p2;
                std::cout << "Result: " << (p1 * p2) << "\n";
                break;
            }
            case 4: {
                std::cout << "Enter Divisor Polynomial P2:\n";
                std::cin >> p2;
                if (p2.getDegree() == 0 && p2.evaluate(0) == 0.0) {
                    std::cout << "Error: Division by the zero polynomial is undefined!\n";
                } else {
                    Polynomial quotient, remainder;
                    p1.divide(p2, quotient, remainder);
                    std::cout << "Quotient:  " << quotient << "\n";
                    std::cout << "Remainder: " << remainder << "\n";
                }
                break;
            }
            case 5: {
                int order;
                std::cout << "Enter derivative order (1 for 1st, 2 for 2nd, etc.): ";
                if (std::cin >> order && order > 0) {
                    std::cout << order << "th Derivative: " << p1.nthDerivative(order) << "\n";
                } else {
                    std::cout << "Invalid derivative order.\n";
                    clearInputBuffer();
                }
                break;
            }
            case 6: {
                double a, b, c;
                std::cout << "Enter constant of integration C: ";
                std::cin >> c;
                std::cout << "Indefinite Integral: " << p1.indefiniteIntegral(c) << "\n";
                std::cout << "Enter lower bound (a) and upper bound (b) for Definite Integral: ";
                if (std::cin >> a >> b) {
                    std::cout << "Definite Integral [" << a << ", " << b << "]: " << p1.definiteIntegral(a, b) << "\n";
                } else {
                    std::cout << "Invalid bounds.\n";
                    clearInputBuffer();
                }
                break;
            }
            case 7: {
                double x;
                std::cout << "Enter value x to evaluate P1(x): ";
                if (std::cin >> x) {
                    std::cout << "P1(" << x << ") = " << p1.evaluate(x) << "\n";
                } else {
                    std::cout << "Invalid input for x.\n";
                    clearInputBuffer();
                }
                break;
            }
            case 8: {
                double x0, m, c;
                std::cout << "Enter point x0 for Tangent Line: ";
                if (std::cin >> x0) {
                    p1.tangentLine(x0, m, c);
                    std::cout << "Tangent Line at x = " << x0 << ": y = " << m << "x " << (c >= 0 ? "+ " : "- ") << std::abs(c) << "\n";
                } else {
                    std::cout << "Invalid point x0.\n";
                    clearInputBuffer();
                }
                break;
            }
            case 9: {
                double x0, m, c;
                std::cout << "Enter point x0 for Normal Line: ";
                if (std::cin >> x0) {
                    p1.normalLine(x0, m, c);
                    if (m >= 1e8) {
                        std::cout << "Normal Line at x = " << x0 << " is Vertical: x = " << x0 << "\n";
                    } else {
                        std::cout << "Normal Line at x = " << x0 << ": y = " << m << "x " << (c >= 0 ? "+ " : "- ") << std::abs(c) << "\n";
                    }
                } else {
                    std::cout << "Invalid point x0.\n";
                    clearInputBuffer();
                }
                break;
            }
            case 10: {
                double guess;
                std::cout << "Enter initial guess for Newton-Raphson root finding: ";
                if (std::cin >> guess) {
                    double root = p1.findRoot(guess);
                    std::cout << "Root approximated near x = " << guess << ": x = " << root << "\n";
                    std::cout << "Verification P1(" << root << ") = " << p1.evaluate(root) << "\n";
                } else {
                    std::cout << "Invalid guess.\n";
                    clearInputBuffer();
                }
                break;
            }
            case 11: {
                double start, end;
                std::cout << "Enter search range [start end] for critical points: ";
                if (std::cin >> start >> end) {
                    std::cout << "Critical Points in [" << start << ", " << end << "]:\n";
                    p1.findCriticalPoints(start, end);
                } else {
                    std::cout << "Invalid range.\n";
                    clearInputBuffer();
                }
                break;
            }
            case 12: {
                std::cout << "Enter inner polynomial G(x) for P1(G(x)):\n";
                std::cin >> p2;
                std::cout << "Composition P1(G(x)) = " << p1.compose(p2) << "\n";
                break;
            }
            case 13: {
                std::cout << "Enter new primary Polynomial P1:\n";
                std::cin >> p1;
                break;
            }
            default:
                std::cout << "Invalid option choice. Pick between 1 and 14.\n";
                break;
        }
    }

    return 0;
}