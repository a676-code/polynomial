#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
using namespace std;

class Polynomial
{
    private:
        struct Term
        {
            double coeff;
            int expo;
            Term* next;
        };
        Term* leading;
        Term* trailing;

    public:
        Polynomial();
        ~Polynomial();

        void appendTerm(double, int);
        int getNumTerms();
        double getCoeff(int);
        double getExpo(int);
        bool isEmpty();
        void printPolynomial();
};

Polynomial::Polynomial()
{
    leading = NULL;
    trailing = NULL;
}

Polynomial::~Polynomial()
{
    Term* curTerm = leading;
    while (curTerm)
    {
        delete curTerm;
        curTerm = curTerm->next;
    }
}

#endif