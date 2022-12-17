#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <algorithm>
#include <cmath>
#include <cctype>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
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
        Polynomial(string);
        ~Polynomial();

        void appendTerm(double, int);
        int getNumTerms();
        double getCoeff(int);
        double getExpo(int);
        bool isEmpty();
        void printPolynomial();
        vector<string> split(string, const char);
};

Polynomial::Polynomial()
{
    leading = NULL;
    trailing = NULL;
}

/// @brief creates a polynomial of the form given in s
/// @param s 
Polynomial::Polynomial(string s)
{
    // removing spaces
	s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

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
    bool negTerm = false;
    int pluspos = s.find("+");
    int minuspos= s.find("-");
    if (minuspos == 0)
    {
        minuspos = s.find("-", 1);
        negTerm = true;
    }
    cout << "\nm: " << minuspos << endl;

    if (pluspos == -1 && minuspos == -1)
        terms.push_back(s);
    // only plusses
    else if (pluspos != -1 && minuspos == -1)
    {
        terms.push_back(s.substr(0, pluspos));
        int pluspos2 = s.find("+", pluspos + 1);
        while (pluspos2 != -1)
        {
            terms.push_back(s.substr(pluspos + 1, pluspos2 - pluspos - 1));
            pluspos = pluspos2;
            pluspos2 = s.find("+", pluspos2 + 1);
        }
        terms.push_back(s.substr(pluspos + 1));
    }
    // only minuses
    else if (pluspos == -1 && minuspos != -1)
    {
        terms.push_back(s.substr(0, minuspos));
        int minuspos2 = s.find("-", minuspos + 1);
        while (minuspos2 != string::npos)
        {
            terms.push_back(s.substr(minuspos + 1, minuspos2 - minuspos - 1));
            minuspos = minuspos2;
            minuspos2 = s.find("-", minuspos2 + 1);
        }
        terms.push_back("-" + s.substr(minuspos + 1));
    }
    else // plusses and minuses
    {
        cout << "HAMBURGER\n";

        int signpos = -1;
        int signpos2 = -1;
        if (pluspos < minuspos)
        {
            signpos = pluspos;
            signpos2 = minuspos;
        }
        else 
        {
            signpos = minuspos;
            signpos2 = pluspos;
        }
        terms.push_back(s.substr(0, signpos));
        while (signpos2 != string::npos)
        {
            if (negTerm)
            {
                cout << "SOMETHING";
                terms.push_back("-" + s.substr(signpos + 1, signpos2 - signpos - 1));
                negTerm = false;
            }
            else
            {
                terms.push_back(s.substr(signpos + 1, signpos2 - signpos - 1));
            }
            signpos = signpos2;
            pluspos = s.find("+", signpos2 + 1);
            minuspos = s.find("-", signpos2 + 1);
            if (pluspos < minuspos)
                signpos2 = pluspos;
            else
            {
                negTerm = true;
                signpos2 = minuspos;
            }
        }
        if (negTerm)
        {
            terms.push_back("-" + s.substr(signpos + 1));
            negTerm = false;
        }
        else
            terms.push_back(s.substr(signpos + 1));
    }

    // building the polynomial
    leading = NULL;
    trailing = NULL;

    cout << "\n";

    int xpos = -1;
    int caretpos = -1;
    double coeff = -1;
    int expo = -1;
    for (auto t : terms)
    {
        cout << "t: " << t << endl;

        if (t[0] == '-')
            negTerm = true;
        xpos = t.find("x");
        caretpos = t.find("^");

        if (xpos == 0 || caretpos <= 0)
            coeff = 1;
        else
            coeff = stod(t.substr(0, xpos));

        if (xpos >= 0 && caretpos == -1)
            expo = 1;
        else if (xpos == -1 && caretpos == -1)
            expo = 0;
        else
            expo = int(stod(t.substr(caretpos + 1)));

        if (negTerm)
        {
            this->appendTerm(-1 * coeff, expo);
        }
        else
        {
            cout << "DOG";
            this->appendTerm(coeff, expo);
        }
    }
    
    this->printPolynomial();
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
            if (abs(curTerm->coeff) != 1 || curTerm->expo == 0)
                cout << curTerm->coeff;
            if (curTerm->expo >= 1)
                cout << "x";
            if (curTerm->expo > 1)
                cout << "^" << curTerm->expo;
            if (curTerm != trailing && curTerm->coeff >= 0)
                cout << " + ";
            else if (curTerm != trailing && curTerm->coeff < 0)
                cout << " - ";
            curTerm = curTerm->next;
        }
    }
}

/// @brief splits a string into substrings based on the location of the character c in str
/// @param str 
/// @param c 
/// @return 
vector<string> Polynomial::split(string str, const char c)
{
    string substring = "";
	vector<string> subStrings;
    for (auto x : str)
    {
        if (x == c)
        {
            subStrings.push_back(substring);
            substring = "";
        }
        else
            substring = substring + x;
    }
    subStrings.push_back(substring);
	return subStrings;
}

#endif