#include "Polynomial.h"
#include <sstream>
#include <cmath>
#include <cctype>
#include <algorithm>

void Polynomial::cleanupZeros() {
    auto it = terms.begin();
    while (it != terms.end()) {
        if (std::abs(it->second) < 1e-9) {
            it = terms.erase(it);
        } else {
            ++it;
        }
    }
}

int Polynomial::getDegree() const {
    if (terms.empty()) return 0;
    return terms.rbegin()->first;
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
    std::map<int, double> result = this->terms;
    for (const auto& pair : other.terms) {
        result[pair.first] += pair.second;
    }
    return Polynomial(result);
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
    std::map<int, double> result;
    for (const auto& pair1 : this->terms) {
        for (const auto& pair2 : other.terms) {
            result[pair1.first + pair2.first] += pair1.second * pair2.second;
        }
    }
    return Polynomial(result);
}

bool Polynomial::operator==(const Polynomial& other) const {
    return this->terms == other.terms;
}

Polynomial Polynomial::derivative() const {
    std::map<int, double> derivTerms;
    for (const auto& pair : terms) {
        int deg = pair.first;
        double coeff = pair.second;
        if (deg > 0) {
            derivTerms[deg - 1] = coeff * deg;
        }
    }
    return Polynomial(derivTerms);
}

double Polynomial::evaluate(double x) const {
    double result = 0.0;
    for (const auto& pair : terms) {
        int deg = pair.first;
        double coeff = pair.second;
        result += coeff * std::pow(x, deg);
    }
    return result;
}

Polynomial Polynomial::parseSimple(const std::string& expr) {
    std::map<int, double> termsMap;
    std::string s = expr;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

    if (s.empty()) return Polynomial();

    size_t i = 0;
    while (i < s.length()) {
        int sign = 1;
        if (s[i] == '+') { i++; }
        else if (s[i] == '-') { sign = -1; i++; }

        double coeff = 1.0;
        bool hasCoeff = false;
        size_t start = i;

        while (i < s.length() && (std::isdigit(s[i]) || s[i] == '.')) {
            i++;
            hasCoeff = true;
        }

        if (hasCoeff) {
            coeff = std::stod(s.substr(start, i - start));
        }
        coeff *= sign;

        int deg = 0;
        if (i < s.length() && s[i] == 'x') {
            i++;
            deg = 1;
            if (i < s.length() && s[i] == '^') {
                i++;
                size_t pStart = i;
                if (i < s.length() && (s[i] == '-' || s[i] == '+')) i++;
                while (i < s.length() && std::isdigit(s[i])) i++;
                deg = std::stoi(s.substr(pStart, i - pStart));
            }
        }
        termsMap[deg] += coeff;
    }
    return Polynomial(termsMap);
}

Polynomial Polynomial::parse(const std::string& expr) {
    std::string s = expr;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

    if (s.empty()) return Polynomial();

    if (s.front() == '(') {
        size_t closePos = s.find_last_of(')');
        if (closePos != std::string::npos) {
            std::string subExpr = s.substr(1, closePos - 1);
            Polynomial base = parse(subExpr);

            int power = 1;
            if (closePos + 1 < s.length() && s[closePos + 1] == '^') {
                power = std::stoi(s.substr(closePos + 2));
            }

            Polynomial result({{0, 1.0}});
            for (int p = 0; p < power; ++p) {
                result = result * base;
            }

            size_t endPowerPos = closePos + 1 + (power > 1 ? std::to_string(power).length() + 1 : 0);
            if (endPowerPos < s.length()) {
                std::string remainder = s.substr(endPowerPos);
                result = result + parse(remainder);
            }
            return result;
        }
    }

    return parseSimple(s);
}

std::string Polynomial::toString() const {
    if (terms.empty()) return "0";
    std::stringstream ss;
    bool first = true;
    for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
        int deg = it->first;
        double coeff = it->second;

        if (!first && coeff > 0) ss << " + ";
        else if (coeff < 0) ss << (first ? "-" : " - ");

        double absCoeff = std::abs(coeff);
        if (deg == 0 || std::abs(absCoeff - 1.0) > 1e-9) {
            ss << absCoeff;
        }

        if (deg > 0) ss << "x";
        if (deg > 1) ss << "^" << deg;

        first = false;
    }
    return ss.str();
}