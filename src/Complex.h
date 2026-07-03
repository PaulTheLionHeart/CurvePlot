// Complex.h: interface for the Complex class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include	<math.h>

#define		FALSE	0
#define		TRUE	1
#define		zerotol 1.e-50

class Complex
    {
    public:
	Complex(void)	{ }
	Complex(const double & real, const double & imaginary)
	    {
	    x = real;
	    y = imaginary;
	    }

	Complex(const Complex & Cmplx1)// Copy Constructor
	    {
	    x = Cmplx1.x;
	    y = Cmplx1.y;
	    }

	Complex(const double & value)
	    {
	    x = value;
	    y = 0;
	    }
		
	~Complex(void);

	Complex operator =(const Complex &);	// Assignment Operator
	Complex operator =(const double &);	// Assignment to a double Operator
	Complex operator+=(const Complex &);
	Complex operator+=(double&);
	Complex operator-=(const Complex &);
	Complex operator-=(double &);
	Complex operator*=(const Complex &);
	Complex operator*=(double &);
	bool	operator==(Complex &);
	Complex operator^(double &);
	Complex operator^(Complex &);
	Complex operator++(void);
	Complex operator +(const Complex &);	// Addition Operator
	Complex operator +(const double &);	// complex add by double Operator
	Complex operator -(const Complex &);	// Subtraction Operator
	Complex operator -(const double &);	// complex subtract by double Operator
	Complex operator -(void);		// unary minus
	Complex operator *(const Complex &);	// Multiplication Operator
	Complex operator *(const double &);	// complex multiply by double Operator
	Complex operator /(const Complex &);	// Division Operator
	Complex operator /(const double &);	// complex divide by double Operator
	inline friend Complex operator*(double real, Complex num)
		{return Complex(num.x*real, num.y*real);}
	inline friend Complex operator+(double real, Complex num)
		{return Complex(num.x+real, num.y);}
	inline friend Complex operator-(double real, Complex num)
		{return Complex(real-num.x, -num.y);}

	double		x, y;
    };

extern	Complex	CSin(Complex  &);		// sine of a complex number
extern	Complex	CCos(Complex  &);		// cosine
extern	Complex	CSqr(Complex  &);		// square
extern	Complex	CSqrt(Complex  &);		// square root
extern	Complex	CCube(Complex  &);		// cube
extern	double	CSumSqr(Complex  &);		// real squared + imaginary squared
extern	double	CFabs(Complex  &);		// abs
extern	Complex	CPolynomial(Complex  &, int);	// take a complex number to an integer power
extern	Complex	CInvert(Complex  &);		// invert
extern	Complex	CExp(Complex  &);		// exponent
extern	Complex	CLog(Complex  &);		// log
//extern	Complex	CComplexPower(Complex  &, Complex &);	// take a complex number to a complex power
// now replaced with operator z^x where z and x are both complex.


