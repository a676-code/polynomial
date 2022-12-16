#include <algorithm>
#include <cmath>
#include <cctype>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;
#include "Polynomial.h"

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