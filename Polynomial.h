#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
using namespace std;

class Polynomial
{
    struct Term
    {
        double coeff;
        int expo;
        Term* next;
    };
    Term* leading;
    Term* trailing;

    Polynomial();
    ~Polynomial();
};