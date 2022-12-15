#include <iostream>
#include "Polynomial.h"
using namespace std;

void Polynomial::appendTerm(double c, int e)
{
    Term* curTerm;

    Term* newTerm = new Term;
    newTerm->coeff = c;
    newTerm->expo = e;
    newTerm->next = NULL;

    if (!leading)
    {
        leading = newTerm;
        trailing = newTerm;
    }
    else
    {
        trailing->next = newTerm;
        trailing = newTerm;
    }
}

int Polynomial::getNumTerms()
{
    int counter = 0;
    Term* curTerm;

    curTerm = leading;
    while (curTerm != trailing)
    {
        counter++;
        curTerm = curTerm->next;
        if (curTerm == trailing)
            counter++;
    }
    return counter;
}

double Polynomial::getCoeff(int pos)
{
    Term* curTerm;
    if (!leading)
        return -std::numeric_limits<int>::max();
    else
    {
        if (pos == 0)
            return leading->coeff;
        
        curTerm = leading;
        int curPos = 0;
        while(curTerm != NULL && pos >= curPos)
        {
            if (pos == curPos)
                return curTerm->coeff;
            curPos++;
            curTerm = curTerm->next;
        }
    }
}

double Polynomial::getExpo(int pos)
{
    Term* curTerm;
    if (!leading)
        return -std::numeric_limits<int>::max();
    else
    {
        if (pos == 0)
            return leading->expo;
        
        curTerm = leading;
        int curPos = 0;
        while(curTerm != NULL && pos >= curPos)
        {
            if (pos == curPos)
                return curTerm->expo;
            curPos++;
            curTerm = curTerm->next;
        }
    }
}

bool Polynomial::isEmpty()
{
    if (!leading)
        return true;
    else
        return false;
}

void Polynomial::printPolynomial()
{
    Term* curTerm;

    if (!leading)
    {
        cout << "Polynomial empty";
    }
    else
    {
        curTerm = leading;
        while(curTerm)
        {
            cout << curTerm->coeff << "^" << curTerm->expo;
            if (curTerm != trailing)
                cout << " + ";
            curTerm = curTerm->next;
        }
    }
}