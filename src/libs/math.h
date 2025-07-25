#pragma once
#include <stdint.h>

uint32_t align(uint32_t number, uint32_t align_to);

uint32_t min(uint32_t num1,uint32_t num2);
uint32_t max(uint32_t num1,uint32_t num2);

unsigned long long factorial(int n);

double acos(double x);
//Returns the arc cosine of x in radians.

double asin(double x);
//Returns the arc sine of x in radians.

double atan(double x);
//Returns the arc tangent of x in radians.

double atan2(double y, double x);
//Returns the arc tangent in radians of y/x based on the signs of both values to determine the correct quadrant.

double cos(double x);
//Returns the cosine of a radian angle x.

double cosh(double x);
//Returns the hyperbolic cosine of x.

double sin(double x);
//Returns the sine of a radian angle x.

double sinh(double x);
//Returns the hyperbolic sine of x.

double tan(double x);
//Returns the tangent of a given angle(x).

double tanh(double x);
//Returns the hyperbolic tangent of x.

double exp(double x);
//Returns the value of e raised to the xth power.

double frexp(double x, int *exponent);
//The returned value is the mantissa and the integer pointed to by exponent is the exponent. The resultant value is x = mantissa * 2 ^ exponent.

double ldexp(double x, int exponent);
//Returns x multiplied by 2 raised to the power of exponent.

double log(double x);
//Returns the natural logarithm (base-e logarithm) of x.

double log10(double x);
//Returns the common logarithm (base-10 logarithm) of x.

double modf(double x, double *integer);
//The returned value is the fraction component (part after the decimal), and sets integer to the integer component.

double pow(double x, int y);
//Returns x raised to the power of y.

double sqrt(double x);
//Returns the square root of x.

double ceil(double x);
//Returns the smallest integer value greater than or equal to x.

double fabs(double x);
int abs(int x);
//Returns the absolute value of x.

double floor(double x);
//Returns the largest integer value less than or equal to x.

double fmod(double x, double y);
//Returns the remainder of x divided by y.

double round(double x);
//Returns the nearest integer value of x(rounded off values).
