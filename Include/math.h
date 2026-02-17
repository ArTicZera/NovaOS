#ifndef MATH_H
#define MATH_H

typedef struct
{
    float x;
    float y;
    float z;
} VERTEX;

#define PI 3.14159265358979
#define RANDMAX 32767
#define SUMLIM 50

#define abs(a) (((a) > 0) ? a : -(a))

int rand(void);
int sign(int x);
int floor(double x);
double sin(double x);
double cos(double x);
double sqrt(double x);
double exp2(double x);
double fabs(double x);
double atan(double x);
double atan2(double y, double x);
double pow(double base, int exp);

#endif
