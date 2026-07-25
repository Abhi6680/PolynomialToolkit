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
        std::cout << "Invalid input. Please enter a valid value.\n";
        clearInputBuffer();
    }
}

Polynomial inputPolynomialPrompt(const std::string& label) {
    std::cout << "Choose input method for " << label << ":\n";
    std::cout << "  1. String Expression (e.g. 3x^2 - 5x + 1)\n";
    std::cout << "  2. Raw Degree & Coefficients Array\n";
    int mode = getValidInput<int>("Select option (1-2): ");

    if (mode == 1) {
        std::cout << "Enter expression: ";
        std::string expr;
        std::cin >> std::ws;
        std::getline(std::cin, expr);
        return Polynomial::parse(expr);
    } else {
        Polynomial p;
        std::cout << "Enter degree followed by coefficients (c0 to cN):\n";
        std::cin >> p;
        return p;
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
    char choice = getValidInput<char>("Save result as new primary P1? (Y/N): ");
    if (choice == 'y' || choice == 'Y') {
        p1 = result;
        std::cout << "Updated primary P1(x) = " << p1 << "\n";
    }
}

int main() {
    Polynomial p1, p2;
    int choice = 0;

    std::cout << "========================================\n";
    std::cout << " Polynomial & Calculus Toolkit v2.0 (CAS)\n";
    std::cout << "========================================\n\n";

    p1 = inputPolynomialPrompt("Primary Polynomial P1");

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
        std::cout << "13. Compute GCD with P2(x)\n";
        std::cout << "14. Factorize P1(x)\n";
        std::cout << "15. Re-enter Primary Polynomial P1\n";
        std::cout << "16. Exit\n";

        choice = getValidInput<int>("Select an option (1-16): ");

        if (choice == 16) {
            std::cout << "Exiting toolkit. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                p2 = inputPolynomialPrompt("Polynomial P2 to add");
                Polynomial res = p1 + p2;
                std::cout << "Result: " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 2: {
                p2 = inputPolynomialPrompt("Polynomial P2 to subtract");
                Polynomial res = p1 - p2;
                std::cout << "Result: " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 3: {
                p2 = inputPolynomialPrompt("Polynomial P2 to multiply");
                Polynomial res = p1 * p2;
                std::cout << "Result: " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 4: {
                p2 = inputPolynomialPrompt("Divisor Polynomial P2");
                if (p2.isZero()) {
                    std::cout << "Error: Division by zero polynomial is undefined!\n";
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
                int order;
                while (true) {
                    order = getValidInput<int>("Enter derivative order (1, 2, 3...): ");
                    if (order > 0) break;
                    std::cout << "Derivative order must be greater than 0.\n";
                }
                Polynomial res = p1.nthDerivative(order);
                std::cout << order << getOrdinalSuffix(order) << " Derivative: " << res << "\n";
                askToSaveResult(p1, res);
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
                p2 = inputPolynomialPrompt("Inner polynomial G(x) for P1(G(x))");
                Polynomial res = p1.compose(p2);
                std::cout << "Composition P1(G(x)) = " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 13: {
                p2 = inputPolynomialPrompt("Second Polynomial P2 for GCD");
                Polynomial common = p1.gcd(p2);
                std::cout << "GCD(P1, P2) = " << common << "\n";
                break;
            }
            case 14: {
                std::cout << "Factorizing P1(x):\n";
                p1.printFactors();
                break;
            }
            case 15: {
                p1 = inputPolynomialPrompt("New Primary Polynomial P1");
                break;
            }
            default:
                std::cout << "Invalid option choice. Pick between 1 and 16.\n";
                break;
        }
    }

    return 0;
}