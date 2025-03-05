#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cmath> 

class Polynomials {
private:
    
    struct Term {
        std::map<char, int> variables; 
        double coefficient; 
    };

    std::vector<Term> terms; 

    enum class State {
        START,
        COEFFICIENT,
        VARIABLE,
        EXPONENT
    };

    Term parseTerm(const std::string& termStr) {
        Term term;
        term.coefficient = 1.0; 

        State state = State::START;
        std::string currentNumber;
        char currentVariable = '\0';  

        for (char c : termStr) {
            switch (state) {
            case State::START:
                if (isdigit(c) || c == '-' || c == '+') {
                    state = State::COEFFICIENT;
                    currentNumber += c;
                }
                else if (isalpha(c)) {
                    state = State::VARIABLE;
                    currentVariable = c;
                    term.variables[c] = 1; 
                }
                else if (c == '.') {
                    state = State::COEFFICIENT;
                    currentNumber += c;
                }
                else if (isspace(c)) {
                    
                }
                else {
                    throw std::invalid_argument("Invalid character in term: " + std::string(1, c));
                }
                break;

            case State::COEFFICIENT:
                if (isdigit(c) || c == '.') {
                    currentNumber += c;
                }
                else if (isalpha(c)) {
                    try {
                        term.coefficient = std::stod(currentNumber);
                    }
                    catch (const std::invalid_argument& e) {
                        throw std::invalid_argument("Invalid coefficient: " + currentNumber);
                    }
                    currentNumber.clear();

                    state = State::VARIABLE;
                    currentVariable = c;
                    term.variables[c] = 1;
                }
                else {
                    try {
                        term.coefficient = std::stod(currentNumber);
                    }
                    catch (const std::invalid_argument& e) {
                        throw std::invalid_argument("Invalid coefficient: " + currentNumber);
                    }
                    currentNumber.clear();
                    throw std::invalid_argument("Invalid character after coefficient: " + std::string(1, c));
                }
                break;

            case State::VARIABLE:
                if (c == '^') {
                    state = State::EXPONENT;
                }
                else if (isalpha(c)) {
                    currentVariable = c;
                    term.variables[c] = 1;
                }
                else if (isspace(c)) {
                    state = State::START;
                }
                else {
                    throw std::invalid_argument("Invalid character after variable: " + std::string(1, c));
                }
                break;

            case State::EXPONENT:
                if (isdigit(c)) {
                    currentNumber += c;
                    try {
                        int exponent = std::stoi(currentNumber);
                        if (exponent > 9) {
                            throw std::invalid_argument("Exponent exceeds maximum value of 9");
                        }
                        term.variables[currentVariable] = exponent;
                    }
                    catch (const std::invalid_argument& e) {
                        throw std::invalid_argument("Invalid exponent: " + currentNumber);
                    }
                    currentNumber.clear();
                    state = State::START; 
                }
                else {
                    throw std::invalid_argument("Invalid exponent character: " + std::string(1, c));
                }
                break;
            }
        }

        if (state == State::COEFFICIENT) {
            try {
                term.coefficient = std::stod(currentNumber);
            }
            catch (const std::invalid_argument& e) {
                throw std::invalid_argument("Invalid coefficient: " + currentNumber);
            }
        }

        return term;
    }

    void parsePolynomial(const std::string& polynomialStr) {
        terms.clear();
        std::string currentTerm;
        bool positive = true; 

        for (size_t i = 0; i < polynomialStr.length(); ++i) {
            char c = polynomialStr[i];

            if (c == '+') {
                if (!currentTerm.empty()) {
                    Term term = parseTerm(currentTerm);
                    term.coefficient = positive ? term.coefficient : -term.coefficient; 
                    terms.push_back(term);
                    currentTerm.clear();
                }
                positive = true;
            }
            else if (c == '-') {
                if (!currentTerm.empty()) {
                    Term term = parseTerm(currentTerm);
                    term.coefficient = positive ? term.coefficient : -term.coefficient; 
                    terms.push_back(term);
                    currentTerm.clear();
                }
                positive = false;
            }
            else if (!isspace(c)) { 
                currentTerm += c;
            }
        }

        if (!currentTerm.empty()) {
            Term term = parseTerm(currentTerm);
            term.coefficient = positive ? term.coefficient : -term.coefficient; 
            terms.push_back(term);
        }

        simplify(); 
    }


    bool areTermsEquivalent(const Term& term1, const Term& term2) const {
        return term1.variables == term2.variables;
    }

    void simplify() {
        std::vector<Term> simplifiedTerms;
        for (const auto& term : terms) {
            bool combined = false;
            for (auto& simplifiedTerm : simplifiedTerms) {
                if (areTermsEquivalent(term, simplifiedTerm)) {
                    simplifiedTerm.coefficient += term.coefficient;
                    combined = true;
                    break;
                }
            }
            if (!combined) {
                simplifiedTerms.push_back(term);
            }
        }
        terms = simplifiedTerms;

        
        terms.erase(std::remove_if(terms.begin(), terms.end(), [](const Term& term) { return term.coefficient == 0.0; }), terms.end());
    }


public:
    Polynomials() {}

    Polynomials(const std::string& polynomialStr) {
        parsePolynomial(polynomialStr);
    }

    Polynomials operator+(const Polynomials& other) const {
        Polynomials result = *this; 
        for (const auto& term : other.terms) {
            result.terms.push_back(term); 
        }
        result.simplify(); 
        return result;
    }

    Polynomials operator-(const Polynomials& other) const {
        Polynomials result = *this;
        for (const auto& term : other.terms) {
            Term negatedTerm = term;
            negatedTerm.coefficient = -negatedTerm.coefficient;
            result.terms.push_back(negatedTerm);
        }
        result.simplify();
        return result;
    }


   
    double find(const std::map<char, int>& variables) const {
        for (const auto& term : terms) {
            if (term.variables == variables) {
                return term.coefficient;
            }
        }
        return 0.0; 
    }

    
    void insert(const std::map<char, int>& variables, double coefficient) {
        for (auto& term : terms) {
            if (term.variables == variables) {
                term.coefficient = coefficient;
                simplify();
                return;
            }
        }
        Term newTerm;
        newTerm.variables = variables;
        newTerm.coefficient = coefficient;
        terms.push_back(newTerm);
        simplify(); 
    }

    
    void remove(const std::map<char, int>& variables) {
        terms.erase(std::remove_if(terms.begin(), terms.end(),
            [&](const Term& term) { return term.variables == variables; }),
            terms.end());
    }

    std::string toString() const {
        if (terms.empty()) {
            return "0";
        }

        std::string result;
        bool firstTerm = true;

        for (const auto& term : terms) {
            if (term.coefficient == 0.0) continue; 
            if (!firstTerm) {
                result += (term.coefficient > 0.0 ? " + " : " - "); 
            }
            else {
                firstTerm = false;
                if (term.coefficient < 0.0) {
                    result += "-"; 
                }
            }

            double absCoefficient = std::abs(term.coefficient);

            if (absCoefficient != 1.0 || term.variables.empty()) {
                result += std::to_string(absCoefficient);
                if (result.find('.') != std::string::npos) {
                    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
                    if (result.back() == '.') {
                        result.pop_back();
                    }
                }
            }

            for (const auto& element : term.variables) {
                char variable = element.first;
                int exponent = element.second;
                result += variable;
                if (exponent > 1) {
                    result += "^" + std::to_string(exponent);
                }
            }
        }

        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Polynomials& poly) {
        os << poly.toString();
        return os;
    }
};


