#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Polynomial
{
    private:
        struct Term
        {
            int coeff;
            int expo;
            Term* next;
        };
        Term* leading;
        Term* trailing;

    public:
        Polynomial();
        Polynomial(string);
        ~Polynomial();

        void appendTerm(double, int);
        int evaluateAt(int x);
        int getCoeff(int);
        int getExpo(int);
        int getNumTerms();
        bool isEmpty();
        void printPolynomial();
        string toString();
        vector<string> split(string, const char);
};

/// @brief default constructor
Polynomial::Polynomial()
{
    leading = NULL;
    trailing = NULL;
}

Polynomial::Polynomial(string s)
{
    // removing spaces
	s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

    // cout << "s: " << s << endl;

    // input validation
	for (int i = 0; i < s.length(); i++)
	{
		if (isalpha(s[i]) && s[i] != 'x')
		{
			cout << "ERROR: input string must be of the form ax^n + bx^{n-1} + ... + yx + z" << endl;
			delete this;
			return;
		}
	}

    // getting each term
    vector<string> terms;
    bool firstTermNegative = false;
    bool negTerm = false;
    int plusPos = s.find("+");
    int minusPos= s.find("-");

    // no signs, one term
    if (plusPos == -1 && minusPos == -1)
        terms.push_back(s);
    // only plusses
    else if (plusPos != -1 && minusPos == -1)
    {
        // adding first term
        terms.push_back(s.substr(0, plusPos));
        int plusPos2 = s.find("+", plusPos + 1);
        // adding middle terms
        while (plusPos2 != -1)
        {
            terms.push_back(s.substr(plusPos + 1, plusPos2 - plusPos - 1));
            plusPos = plusPos2;
            plusPos2 = s.find("+", plusPos2 + 1);
        }
        // adding last term
        terms.push_back(s.substr(plusPos + 1));
    }
    // only minuses
    else if (plusPos == -1 && minusPos != -1)
    {
        // adding first term
        terms.push_back(s.substr(0, minusPos));
        int minusPos2 = s.find("-", minusPos + 1);
        // adding middle terms
        while (minusPos2 != string::npos)
        {
            terms.push_back("-" + s.substr(minusPos + 1, minusPos2 - minusPos - 1));
            minusPos = minusPos2;
            minusPos2 = s.find("-", minusPos2 + 1);
        }
        // adding last term
        terms.push_back("-" + s.substr(minusPos + 1));
    }
    else // plusses and minuses
    {
        bool negTerm = false;
        int signPos = -2;
        int signPos2 = -2;

        int plusPos = s.find("+");
        int minusPos = s.find("-");

        signPos = 0;
        if (plusPos < minusPos)
            signPos2 = plusPos;
        else
            signPos2 = minusPos;
        
        // terms.push_back(s.substr(signPos, signPos2));
        while(signPos != -1)
        {
            
            cout << "here: "  << s.substr(signPos, signPos2 - signPos - 1) << endl;
            terms.push_back(s.substr(signPos, signPos2 - signPos - 1));

            // moving forward
            signPos = signPos2;
            plusPos = s.find("+", signPos2 + 1);
            minusPos = s.find("-", signPos2 + 1);
            if (plusPos < minusPos)
                signPos2 = plusPos;
            else
                signPos2 = minusPos;
        }
    }

    // building the polynomial
    leading = NULL;
    trailing = NULL;

    cout << "\n";

    int xpos = -1;
    int caretpos = -1;
    double coeff = -1;
    int expo = -1;
    for (int i = 0; i < terms.size(); i++)
    {
        // cout << "TERM: "  << terms.at(i) << endl;

        if (terms.at(i)[0] == '-')
            negTerm = true;
        
        // parsing
        xpos = terms.at(i).find("x");
        caretpos = terms.at(i).find("^");

        if (xpos == 0 || caretpos <= 0)
            coeff = 1;
        else
            coeff = stod(terms.at(i).substr(0, xpos));

        if (xpos >= 0 && caretpos == -1)
            expo = 1;
        else if (xpos == -1 && caretpos == -1)
            expo = 0;
        else
            expo = int(stod(terms.at(i).substr(caretpos + 1)));

        if (negTerm)
        {
            if (firstTermNegative && i == 0)
                coeff = coeff * -1;
                
            this->appendTerm(-1 * coeff, expo);
            negTerm = false;
        }
        else
            this->appendTerm(coeff, expo);       
    }
    this->printPolynomial();
    cout << endl;
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

int Polynomial::evaluateAt(int x)
{
    int result = -std::numeric_limits<int>::max();
    Term* curTerm;

    if (!leading)
        cout << "Polynomial empty";
    else
    {
        result = 0;
        curTerm = leading;
        while (curTerm)
        {
            result += curTerm->coeff * pow(x, curTerm->expo);
            curTerm = curTerm->next;
        }
    }
    return result;
}

int Polynomial::getCoeff(int pos)
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
    return -std::numeric_limits<int>::max();
}

int Polynomial::getExpo(int pos)
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
    return -std::numeric_limits<int>::max();
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
        cout << "Polynomial empty";
    else
    {
        curTerm = leading;
        while(curTerm)
        {
            // cout << "COEFF: " << curTerm->coeff << endl;

            if (abs(curTerm->coeff) != 1 || curTerm->expo == 0)
            {
                if (curTerm->coeff < 0 && curTerm != leading)
                    cout << curTerm->coeff * -1;
                else
                    cout << curTerm->coeff;
            }
            if (curTerm->expo >= 1)
                cout << "x";
            if (curTerm->expo > 1)
                cout << "^" << curTerm->expo;
            if (curTerm != trailing && curTerm->next->coeff >= 0)
                cout << " + ";
            else if (curTerm != trailing && curTerm->next->coeff < 0)
                cout << " - ";
            curTerm = curTerm->next;
        }
    }
}

/// @brief convert Polynomial object to string
/// @return the polynomial as a string
string Polynomial::toString()
{
    string result = "";
    Term* curTerm;
    if (!leading)
        cout << "Polynomial empty";
    else
    {
        curTerm = leading;
        while (curTerm)
        {
            if (abs(curTerm->coeff) != 1 || curTerm->expo == 0)
            {
                if (curTerm->coeff < 0 && curTerm != leading)
                    result.append(to_string(curTerm->coeff * -1));
                else
                    result.append(to_string(curTerm->coeff));
            }
            if (curTerm->expo >= 1)
                result.append("x");
            if (curTerm->expo > 1)
                result.append("^" + to_string(curTerm->expo));
            if (curTerm != trailing && curTerm->next->coeff >= 0)
                result.append(" + ");
            else if (curTerm != trailing && curTerm->next->coeff < 0)
                result.append(" - ");
            curTerm = curTerm->next;
        }
    }
    return result;
}

#endif