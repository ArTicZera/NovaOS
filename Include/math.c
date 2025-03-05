/*
    Coded by ArTic/JhoPro

    I'll not say much about this because it doesn't evolves OS 
    development. These functions was made just for programs. 
    mostly like graphics programs.
*/

#include "stdint.h"

#include "math.h"

static DWORD randSeed = 42;

int rand(void)
{
    randSeed = randSeed * 1103515245 + 12345;

    return (randSeed / 65536) % RANDMAX;
}

double sin(double x)
{
    double sum = 0.0;

    double a  = 1.0;
    double b  = 1.0;
    double expx = x;

    for (int i = 1; i <= SUMLIM; i++)
    {
        sum += (a / b) * expx;
        
        expx *= x * x;

        a *= -1.0;
        b *= 4 * i * i + 2 * i;
    }

    return sum;
}

double cos(double x)
{
    double sum = 0.0;

    double a  = 1.0;
    double b  = 1.0;
    double expx = 1;

    for (int i = 1; i <= SUMLIM; i++)
    {
        sum += (a / b) * expx;
        
        expx *= x * x;

        a *= -1.0;
        b *= 4 * i * i - 2 * i;
    }

    return sum;
}

double sqrt(double x) 
{
    const int MAX_ITERATIONS = 30;
    const double INITIAL_APPROXIMATION = 1.0;

    double approximation = INITIAL_APPROXIMATION;

    for (int i = 0; i < MAX_ITERATIONS; ++i) 
    {
        approximation = 0.5 * (approximation + x / approximation);
    }

    return approximation;
}

double exp2(double x) 
{
    double result = 1.0;
    double power_of_2 = 2.0;

    if (x == 0.0) 
    {
        return 1.0;  // 2^0 = 1
    }
    else if (x > 0.0) 
    {
        while (x >= 1.0) 
        {
            result *= power_of_2;
            x -= 1.0;
        }
    }
    else 
    {  // x < 0
        while (x <= -1.0) 
        {
            result /= power_of_2;
            x += 1.0;
        }
    }

    double frac = 1.0;
    double term = 1.0;
    double i = 1.0;

    while (i <= 32.0) 
    {
        term *= (x / i);
        frac += term;
        i += 1.0;
    }

    return result * frac;
}

double fabs(double x)
{
    return (x < 0) ? -x : x;
}

int sign(int x)
{
    if (x < 0)
    {
        return -1;
    }

    return 1;
}

int floor(double x) 
{
    if (x >= 0) 
    {
        return (int)x;
    }
    else 
    {
        int int_part = (int)x;
        return (x == int_part) ? int_part : int_part - 1;
    }
}

double atan(double x) 
{
    const int maxIter = 50;
    const double epsilon = 1e-10;

    if (x > 1 || x < -1) 
    {
        double pi = 3.141592653589793;

        if (x > 0) 
        {
            return pi / 2 - atan(1 / x);
        } 
        else 
        {
            return -pi / 2 - atan(1 / x);
        }
    }

    double result = 0.0;
    double term = x;
    int n = 1;

    while (abs(term) > epsilon && n <= maxIter) 
    {
        result += term;
        term *= -x * x * (2 * n - 1) / (2 * n + 1);
        n++;
    }

    return result;
}

double atan2(double y, double x) 
{
    if (x == 0) 
    {
        if (y > 0) return PI / 2;
        if (y < 0) return -PI / 2;
        return 0;
    }

    double atanResult = atan(y / x);

    if (x > 0) 
    {
        return atanResult;
    } 
    else 
    {
        if (y >= 0) 
        {
            return atanResult + PI;
        }
        else 
        {
            return atanResult - PI;
        }
    }
}

double pow(double base, int exp) 
{
    if (exp == 0) 
    {
        return 1.0;
    }
    
    if (exp < 0) 
    {
        return 1.0 / pow(base, -exp);
    }

    double result = 1.0;

    while (exp > 0) 
    {
        if (exp % 2 == 1) result *= base;
        base *= base;
        exp /= 2;
    }
    
    return result;
}
