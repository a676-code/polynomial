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
    int pluspos = s.find("+");
    int minuspos= s.find("-");
    if (minuspos == 0)
    {
        firstTermNegative = true;
        minuspos = s.find("-", 1);
    }

    // no signs, one term
    if (pluspos == -1 && minuspos == -1)
        terms.push_back(s);
    // only plusses
    else if (pluspos != -1 && minuspos == -1)
    {
        // adding first term
        terms.push_back(s.substr(0, pluspos));
        int pluspos2 = s.find("+", pluspos + 1);
        // adding middle terms
        while (pluspos2 != -1)
        {
            terms.push_back(s.substr(pluspos + 1, pluspos2 - pluspos - 1));
            pluspos = pluspos2;
            pluspos2 = s.find("+", pluspos2 + 1);
        }
        // adding last term
        terms.push_back(s.substr(pluspos + 1));
    }
    // only minuses
    else if (pluspos == -1 && minuspos != -1)
    {
        // adding first term
        terms.push_back(s.substr(0, minuspos));
        int minuspos2 = s.find("-", minuspos + 1);
        // adding middle terms
        while (minuspos2 != string::npos)
        {
            terms.push_back("-" + s.substr(minuspos + 1, minuspos2 - minuspos - 1));
            minuspos = minuspos2;
            minuspos2 = s.find("-", minuspos2 + 1);
        }
        // adding last term
        terms.push_back("-" + s.substr(minuspos + 1));
    }
    else // plusses and minuses
    {
        int signpos = -1;
        int signpos2 = -1;
        // first sign +
        if (pluspos < minuspos)
        {
            signpos = pluspos;
            signpos2 = minuspos;
        }
        else // first sign -
        {
            signpos = minuspos;
            signpos2 = pluspos;
        }
        // adding first term
        terms.push_back(s.substr(0, signpos));
        // adding middle terms
        while (signpos2 != -1)
        {
            if (negTerm)
            {
                // cout << "\t1 neg: " << "-" + s.substr(signpos + 1, signpos2 - signpos - 1);
                terms.push_back("-" + s.substr(signpos + 1, signpos2 - signpos - 1));
                negTerm = false;
            }
            else
            {
                // cout << "\t1 pos: " << s.substr(signpos, signpos2 - signpos) << endl;
                terms.push_back(s.substr(signpos, signpos2 - signpos));
            }
            signpos = signpos2;
            pluspos = s.find("+", signpos2 + 1);
            minuspos = s.find("-", signpos2 + 1);
            // + before -
            if (pluspos < minuspos)
            {
                if (pluspos != -1)
                    signpos2 = pluspos;
            }
            else // - before +
            {
                if (minuspos != -1)
                negTerm = true;
                signpos2 = minuspos;
            }
        }
        // adding last term
        if (negTerm)
        {
            // cout << "s again: " << s << endl;
            // cout << "\t2 neg: " << "-" + s.substr(signpos) << endl;
            terms.push_back("-" + s.substr(signpos + 1));
            negTerm = false;
        }
        else
        {
            // cout << "\t2 pos" << s.substr(signpos + 1) << endl;
            terms.push_back(s.substr(signpos + 1));
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