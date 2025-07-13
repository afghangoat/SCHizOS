#include "math.h"
//TODO rename this to math.h
#define TAYLOR_TERMS 10
#define e 2.71828183
#define NAN 0./0.
#define ERR_CORRECTION 7
#define M_PI 3.1415926536
#define LN2 0.69314718056

uint32_t align(uint32_t number, uint32_t align_to){
	if(align_to==0){
		return number;
	}
	uint32_t remainder=number%align_to;
	if(remainder>0){
		return (number+align_to-remainder);
	}
	return number;
}

uint32_t min(uint32_t num1,uint32_t num2){
	if(num1<num2){
		return num1;
	}
	return num2;
}

uint32_t max(uint32_t num1,uint32_t num2){
	if(num1<num2){
		return num2;
	}
	return num1;
}
/*double pow(double x, double y){
	double n=1.0;
	for(int i=0;i<y;i++){
		n*=x;
	}
	return n;
}*/
double pow(double x, int y)
{
    double temp;
    if (y == 0){
		return 1;
	}
    temp = pow (x, y / 2);
    if ((y % 2) == 0) {
        return temp * temp;
    } else {
        if (y > 0){
            return x * temp * temp;
        }else{
            return (temp * temp) / x;
		}
	}
}
//Returns x raised to the power of y.

// Function to calculate factorial 
unsigned long long factorial(int n) { 
    unsigned long long result = 1; 
    for (int i = 1; i <= n; i++) { 
        result *= i; 
    } 
    return result; 
} 

double acos(double x) {
    if (x < -1.0 || x > 1.0) {
        return NAN; // Domain of acos is [-1, 1]
    }
    return M_PI / 2 - asin(x);
}
//Returns the arc cosine of x in radians.

double asin(double x) {
    if (x < -1.0 || x > 1.0) {
        // Domain of asin is [-1, 1]
        return NAN; // Not a number
    }

    double y = x; // Initial guess
    for (int i = 0; i < ERR_CORRECTION; i++) { // Iterate for better approximation
        y -= (sin(y) - x) / cos(y); // Newton's method formula
    }
    return y;
}

double atan(double x) {
    if (x > 1.0) {
        return M_PI / 2 - atan(1 / x);
    } else if (x < -1.0) {
        return -M_PI / 2 - atan(1 / x);
    }

    double atan_value = 0.0;
    for (int n = 0; n < TAYLOR_TERMS; n++) {
        double term = (n % 2 == 0 ? 1 : -1) * pow(x, 2 * n + 1) / (2 * n + 1);
        atan_value += term;
    }
    return atan_value;
}
//Returns the arc tangent of x in radians.

double atan2(double y, double x) {
    if (x > 0) {
        return atan(y / x); // First and fourth quadrants
    } else if (x < 0 && y >= 0) {
        return atan(y / x) + M_PI; // Second quadrant
    } else if (x < 0 && y < 0) {
        return atan(y / x) - M_PI; // Third quadrant
    } else if (x == 0 && y > 0) {
		return M_PI / 2; // Positive y-axis
    } else if (x == 0 && y < 0) {
        return -M_PI / 2; // Negative y-axis
    } else {
        return 0; // Origin (0, 0)
    }
}
//Returns the arc tangent in radians of y/x based on the signs of both values to determine the correct quadrant.

double cos(double x) {
    double cosine = 0.0;

    for (int n = 0; n < TAYLOR_TERMS; n++) {
        // Calculate the current term
        double term = (n % 2 == 0 ? 1 : -1) * (pow(x, 2 * n) / factorial(2 * n));
        cosine += term;
    }

    return cosine;
}
//Returns the cosine of a radian angle x.

double cosh(double x){
	return (pow(e,x)+pow(e,-x))/2;
}
//Returns the hyperbolic cosine of x.

double sin(double x){
	double sine = 0.0; 
 
    for (int n = 0; n < TAYLOR_TERMS; n++) { 
        // Calculate the current term 
        double term = (n % 2 == 0 ? 1 : -1) * (pow(x, 2 * n + 1) / factorial(2 * n + 1)); 
        sine += term; 
    } 
     
    return sine; 
}
//Returns the sine of a radian angle x.

double sinh(double x){
	return (pow(e,x)-pow(e,-x))/2;
}
//Returns the hyperbolic sine of x.

double tan(double x){
	double val=sin(x)/cos(x);
	return val;
}
//Returns the tangent of a given angle(x).

double tanh(double x){
	return sinh(x)/cosh(x);
}
//Returns the hyperbolic tangent of x.

double exp(double x){
	return pow(e,x);
}
//Returns the value of e raised to the xth power.

double frexp(double x, int *exponent) {
    if (x == 0.0) {
        *exponent = 0;
        return 0.0; // x is zero, mantissa is 0
    }

    *exponent = 0;

    // Get the absolute value of x (mantissa will later have the same sign as x)
    double abs_x = fabs(x);

    // Scale abs_x to the range [0.5, 1) by adjusting the exponent
    while (abs_x >= 1.0) {
        abs_x /= 2.0; // Scale down
        (*exponent)++; // Increment exponent
    }
    while (abs_x < 0.5) {
        abs_x *= 2.0; // Scale up
        (*exponent)--; // Decrement exponent
    }

    // Return the mantissa with the original sign of x
    return x < 0 ? -abs_x : abs_x;
}

double ldexp(double x, int exponent){
	return x*pow(2,exponent);
}
//Returns x multiplied by 2 raised to the power of exponent.

double log(double x){ //ln
	// ln(1 - y) = sum of -(y ^ k) / k from k = 1 to n but only if 0 < y < 1
    // taylor expansion formula of ln(x): https://furthermathematicst.blogspot.com/2011/06/71-taylor-polynomial-part-1.html
    int exponent = 0; // map the value x between 0 and 1
    while (x > 1.0) {x /= 2.0; exponent++;}
    while (x < 0.0) {x *= 2.0; exponent--;}
    int k, n = 1000; // iteration
    double y = 1 - x; // ln(1 - y) = ln(x) >> ln(y) = ln(1 - x)
    double initial_y = y;
    double result = 0;
    for (k = 1; k <= n; k++) { // sum of -(y ^ k) / k from k = 1 to n
        result += y / k;
        y *= initial_y; // y ^ k
    }
    return -1 * result + exponent * LN2;
}
//Returns the natural logarithm (base-e logarithm) of x.

double log10(double x) {
    const double LN_10 = 2.302585092994046; // Natural log of 10
    return log(x) / LN_10; // Calculate log10(x) using the natural logarithm
}
//Returns the common logarithm (base-10 logarithm) of x.

double modf(double x, double *integer) {
    if (x == 0.0) { //edge case
        *integer = 0.0;
        return 0.0;
    }

    // Get the integer part of x (positive or negative)
    *integer = (x > 0.0) ? floor(x) : ceil(x);

    // Calculate the fractional part
    return x - *integer;
}
//The returned value is the fraction component (part after the decimal), and sets integer to the integer component.

double sqrt(double number) { //stupid quake 3 sqrt, very fast but not accurate
   long i;
   double x2, y;
   const double threehalfs = 1.5F;

   x2 = number * 0.5F;
   y  = number;
   i  = * ( long * ) &y;                     // floating point bit level hacking [sic]
   i  = 0x5f3759df - ( i >> 1 );             // Newton's approximation
   y  = * ( double * ) &i;
   y  = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
   y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration
   y  = y * ( threehalfs - ( x2 * y * y ) ); // 3rd iteration

   return 1/y;
}
//Returns the square root of x.

double ceil(double x){
	int val=((int)x)+1;
	return (double)val;
}
//Returns the smallest integer value greater than or equal to x.

double fabs(double x){
	if(x<0.0){
		return -x;
	}
	return x;
}

int abs(int x){
	if(x<0){
		return -x;
	}
	return x;
}

double floor(double x){
	int val=(int)x;
	return (double)val;
}
//Returns the largest integer value less than or equal to x.

double fmod(double x, double y){
	double rem=x;
	while(rem>y){
		rem-=y;
	}
	return rem;
}
//Returns the remainder of x divided by y.

double round(double x){
	int val=(int)x;
	double frac=fmod(x,1);
	if(frac>0.5){
		return val+1;
	}
	return val;
}
//Returns the nearest integer value of x(rounded off values).