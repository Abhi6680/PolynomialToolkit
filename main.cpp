#include <iostream>
#include <limits>
#include <cmath>
#include <string>
#include "include/Polynomial.h"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template <typename T>
T getValidInput(const std::string& prompt) {
    T val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            return val;
        }
        std::cout << "Invalid input. Please try again.\n";
        clearInputBuffer();
    }
}

std::string getOrdinalSuffix(int n) {
    if (n % 100 >= 11 && n % 100 <= 13) return "th";
    switch (n % 10) {
        case 1:  return "st";
        case 2:  return "nd";
        case 3:  return "rd";
        default: return "th";
    }
}

void askToSaveResult(Polynomial& p1, const Polynomial& result) {
    char choice;
    std::cout << "Save result as new primary P1? (Y/N): ";
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        p1 = result;
        std::cout << "Updated primary P1(x) = " << p1 << "\n";
    }
}

int main() {
    Polynomial p1, p2;
    int choice = 0;

    std::cout << "========================================\n";
    std::cout << " Polynomial & Calculus Toolkit v1.0\n";
    std::cout << "========================================\n\n";

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
        std::cout << "5. Derivative (1st, 2nd, Nth)\n";
        std::cout << "6. Integration (Indefinite & Definite)\n";
        std::cout << "7. Evaluate at x\n";
        std::cout << "8. Tangent Line at x\n";
        std::cout << "9. Normal Line at x\n";
        std::cout << "10. Find Root (Newton-Raphson)\n";
        std::cout << "11. Find Critical Points\n";
        std::cout << "12. Composition P1(P2(x))\n";
        std::cout << "13. Re-enter Primary Polynomial P1\n";
        std::cout << "14. Exit\n";

        choice = getValidInput<int>("Select an option (1-14): ");

        if (choice == 14) {
            std::cout << "Exiting toolkit. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                std::cout << "Enter Polynomial P2 to add:\n";
                std::cin >> p2;
                Polynomial res = p1 + p2;
                std::cout << "Result: " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 2: {
                std::cout << "Enter Polynomial P2 to subtract:\n";
                std::cin >> p2;
                Polynomial res = p1 - p2;
                std::cout << "Result: " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 3: {
                std::cout << "Enter Polynomial P2 to multiply:\n";
                std::cin >> p2;
                Polynomial res = p1 * p2;
                std::cout << "Result: " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 4: {
                std::cout << "Enter Divisor Polynomial P2:\n";
                std::cin >> p2;
                if (p2.isZero()) {
                    std::cout << "Error: Division by the zero polynomial is undefined!\n";
                } else {
                    Polynomial quotient, remainder;
                    p1.divide(p2, quotient, remainder);
                    std::cout << "Quotient:  " << quotient << "\n";
                    std::cout << "Remainder: " << remainder << "\n";
                    askToSaveResult(p1, quotient);
                }
                break;
            }
            case 5: {
                int order = getValidInput<int>("Enter derivative order (1, 2, 3...): ");
                if (order > 0) {
                    Polynomial res = p1.nthDerivative(order);
                    std::cout << order << getOrdinalSuffix(order) << " Derivative: " << res << "\n";
                    askToSaveResult(p1, res);
                } else {
                    std::cout << "Derivative order must be greater than 0.\n";
                }
                break;
            }
            case 6: {
                double c = getValidInput<double>("Enter constant of integration C: ");
                std::cout << "Indefinite Integral: " << p1.indefiniteIntegral(c) << "\n";

                double a = getValidInput<double>("Enter lower bound (a) for Definite Integral: ");
                double b = getValidInput<double>("Enter upper bound (b) for Definite Integral: ");
                std::cout << "Definite Integral [" << a << ", " << b << "]: " << p1.definiteIntegral(a, b) << "\n";
                break;
            }
            case 7: {
                double x = getValidInput<double>("Enter value x to evaluate P1(x): ");
                std::cout << "P1(" << x << ") = " << p1.evaluate(x) << "\n";
                break;
            }
            case 8: {
                double x0 = getValidInput<double>("Enter point x0 for Tangent Line: ");
                double m, c;
                p1.tangentLine(x0, m, c);
                std::cout << "Tangent Line at x = " << x0 << ": y = " << m << "x " << (c >= 0 ? "+ " : "- ") << std::abs(c) << "\n";
                break;
            }
            case 9: {
                double x0 = getValidInput<double>("Enter point x0 for Normal Line: ");
                double m, c;
                p1.normalLine(x0, m, c);
                if (m >= 1e8) {
                    std::cout << "Normal Line at x = " << x0 << " is Vertical: x = " << x0 << "\n";
                } else {
                    std::cout << "Normal Line at x = " << x0 << ": y = " << m << "x " << (c >= 0 ? "+ " : "- ") << std::abs(c) << "\n";
                }
                break;
            }
            case 10: {
                double guess = getValidInput<double>("Enter initial guess for Newton-Raphson root finding: ");
                double root = p1.findRoot(guess);
                std::cout << "Root approximated near x = " << guess << ": x = " << root << "\n";
                std::cout << "Verification P1(" << root << ") = " << p1.evaluate(root) << "\n";
                break;
            }
            case 11: {
                double start = getValidInput<double>("Enter search range start: ");
                double end = getValidInput<double>("Enter search range end: ");
                std::cout << "Critical Points in [" << start << ", " << end << "]:\n";
                p1.findCriticalPoints(start, end);
                break;
            }
            case 12: {
                std::cout << "Enter inner polynomial G(x) for P1(G(x)):\n";
                std::cin >> p2;
                Polynomial res = p1.compose(p2);
                std::cout << "Composition P1(G(x)) = " << res << "\n";
                askToSaveResult(p1, res);
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