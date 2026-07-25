#include "../include/Polynomial.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <map>
#include <stdexcept>
#include <stack>
#include <vector>

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

bool Polynomial::isZero() const {
    for (int i = 0; i <= degree; ++i) {
        if (std::abs(coeffs[i]) > 1e-9) return false;
    }
    return true;
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
        if (res.isZero()) break;
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
                std::cout << "Inflexion Point\n";
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
    if (divisor.isZero()) {
        return;
    }

    if (degree < divisor.degree) {
        quotient = Polynomial();
        remainder = *this;
        return;
    }

    remainder = *this;
    quotient = Polynomial(degree - divisor.degree);

    while (remainder.degree >= divisor.degree && !remainder.isZero()) {
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

Polynomial Polynomial::gcd(const Polynomial& other) const {
    Polynomial a = *this;
    Polynomial b = other;

    while (!b.isZero()) {
        Polynomial q, r;
        a.divide(b, q, r);
        a = b;
        b = r;
    }

    if (!a.isZero()) {
        double leadingCoeff = a.coeffs[a.degree];
        for (int i = 0; i <= a.degree; ++i) {
            a.coeffs[i] /= leadingCoeff;
        }
    }
    return a;
}

void Polynomial::printFactors() const {
    if (isZero()) {
        std::cout << "0\n";
        return;
    }

    Polynomial temp = *this;
    double leadingCoeff = temp.coeffs[temp.degree];
    std::cout << "P(x) = " << leadingCoeff;

    for (double x = -50.0; x <= 50.0; x += 0.5) {
        if (temp.degree <= 0) break;

        double root = temp.findRoot(x);
        if (std::abs(temp.evaluate(root)) < 1e-4) {
            root = std::round(root * 1000.0) / 1000.0;
            std::cout << "(x " << (root >= 0 ? "- " : "+ ") << std::abs(root) << ")";

            double factorCoeffs[2] = {-root, 1.0};
            Polynomial factor(factorCoeffs, 1);

            Polynomial q, r;
            temp.divide(factor, q, r);
            temp = q;
            x = -50.0;
        }
    }

    if (temp.getDegree() > 0 && !temp.isZero()) {
        std::cout << " * (" << temp << ")";
    }
    std::cout << "\n";
}

enum TokenType { NUMBER, VAR, OP_ADD, OP_SUB, OP_MUL, OP_POW, LPAREN, RPAREN };

struct Token {
    TokenType type;
    double numVal = 0.0;
};

Polynomial Polynomial::parse(const std::string& expr) {
    if (expr.empty()) throw std::invalid_argument("Parser Error: Empty expression string.");

    std::vector<Token> tokens;
    int n = expr.length();
    int i = 0;

    while (i < n) {
        char c = expr[i];
        if (std::isspace(c)) { i++; continue; }

        if (std::isdigit(c) || c == '.') {
            int start = i;
            while (i < n && (std::isdigit(expr[i]) || expr[i] == '.')) i++;
            try {
                double val = std::stod(expr.substr(start, i - start));
                tokens.push_back({NUMBER, val});
            } catch (...) {
                throw std::invalid_argument("Parser Error: Invalid numeric constant near position " + std::to_string(start));
            }
        } else if (c == 'x' || c == 'X') {
            if (!tokens.empty() && (tokens.back().type == NUMBER || tokens.back().type == RPAREN || tokens.back().type == VAR)) {
                tokens.push_back({OP_MUL, 0});
            }
            tokens.push_back({VAR, 0});
            i++;
        } else if (c == '+') { tokens.push_back({OP_ADD, 0}); i++; }
        else if (c == '-') {
            if (tokens.empty() || tokens.back().type == LPAREN || tokens.back().type == OP_ADD || 
                tokens.back().type == OP_SUB || tokens.back().type == OP_MUL || tokens.back().type == OP_POW) {
                tokens.push_back({NUMBER, 0.0});
            }
            tokens.push_back({OP_SUB, 0}); 
            i++; 
        }
        else if (c == '*') { tokens.push_back({OP_MUL, 0}); i++; }
        else if (c == '^') { tokens.push_back({OP_POW, 0}); i++; }
        else if (c == '(') {
            if (!tokens.empty() && (tokens.back().type == NUMBER || tokens.back().type == VAR || tokens.back().type == RPAREN)) {
                tokens.push_back({OP_MUL, 0});
            }
            tokens.push_back({LPAREN, 0}); 
            i++; 
        }
        else if (c == ')') { tokens.push_back({RPAREN, 0}); i++; }
        else {
            throw std::invalid_argument(std::string("Parser Error: Unexpected character '") + c + "' at index " + std::to_string(i));
        }
    }

    int parenDepth = 0;
    for (const auto& tok : tokens) {
        if (tok.type == LPAREN) parenDepth++;
        if (tok.type == RPAREN) parenDepth--;
        if (parenDepth < 0) throw std::invalid_argument("Parser Error: Unmatched closing parenthesis ')'");
    }
    if (parenDepth != 0) throw std::invalid_argument("Parser Error: Unmatched opening parenthesis '('");

    std::vector<Token> rpn;
    std::stack<Token> opStack;

    auto precedence = [](TokenType type) {
        if (type == OP_POW) return 3;
        if (type == OP_MUL) return 2;
        if (type == OP_ADD || type == OP_SUB) return 1;
        return 0;
    };

    for (const auto& tok : tokens) {
        if (tok.type == NUMBER || tok.type == VAR) {
            rpn.push_back(tok);
        } else if (tok.type == LPAREN) {
            opStack.push(tok);
        } else if (tok.type == RPAREN) {
            while (!opStack.empty() && opStack.top().type != LPAREN) {
                rpn.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        } else {
            while (!opStack.empty() && precedence(opStack.top().type) >= precedence(tok.type) && tok.type != OP_POW) {
                rpn.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(tok);
        }
    }
    while (!opStack.empty()) {
        rpn.push_back(opStack.top());
        opStack.pop();
    }

    std::stack<Polynomial> polyStack;

    for (const auto& tok : rpn) {
        if (tok.type == NUMBER) {
            double c[1] = {tok.numVal};
            polyStack.push(Polynomial(c, 0));
        } else if (tok.type == VAR) {
            double c[2] = {0.0, 1.0};
            polyStack.push(Polynomial(c, 1));
        } else {
            if (polyStack.size() < 2) throw std::invalid_argument("Parser Error: Malformed operator expression.");
            Polynomial b = polyStack.top(); polyStack.pop();
            Polynomial a = polyStack.top(); polyStack.pop();

            if (tok.type == OP_ADD) polyStack.push(a + b);
            else if (tok.type == OP_SUB) polyStack.push(a - b);
            else if (tok.type == OP_MUL) polyStack.push(a * b);
            else if (tok.type == OP_POW) {
                int exp = static_cast<int>(b.evaluate(0));
                Polynomial powRes;
                double unit[1] = {1.0};
                powRes = Polynomial(unit, 0);
                for (int p = 0; p < exp; p++) powRes = powRes * a;
                polyStack.push(powRes);
            }
        }
    }

    if (polyStack.empty()) throw std::invalid_argument("Parser Error: Evaluation failed.");
    return polyStack.top();
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