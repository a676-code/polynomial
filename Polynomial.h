#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <algorithm>
#include <cmath>
#include <cctype>
#include <iostream>
#include <vector>
#include <sstream>
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
	vector<string> terms = split(s, '+');

    // splitting over ^'s, components contains (cx, e)
	vector<vector<string> > components;
	for (auto t : terms)
		components.push_back(split(t, '^'));

    // saving in coeffs and exponents
	vector<double> coeffs;
	vector<int> exponents;
	for (int i = 0; i < terms.size(); i++)
	{
		vector<string> pair = split(components[i][0], 'x');
		if (pair[0] == "")
			coeffs.push_back(1.0);
		else
			coeffs.push_back(stod(pair[0]));

		if (components[i].size() > 1)
			exponents.push_back(stoi(components[i][1]));
		else
			exponents.push_back(1);
	}

    // building the polynomial
    leading = NULL;
    trailing = NULL;

    for (int i; i < terms.size(); i++)
        this->appendTerm(coeffs.at(i), exponents.at(i));
    
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
            if (abs(curTerm->coeff) != 1 || curTerm->expo == 1)
                cout << curTerm->coeff;
            if (curTerm->expo > 1)
                cout << "x^" << curTerm->expo;
            if (curTerm != trailing)
                cout << " + ";
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