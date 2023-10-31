#include <bits/stdc++.h>
using namespace std;

#define tab "\t"
//fuction for get minimum of three numbers
double getMin(double a, double b, double c)
{
    double min = a;
    if (b < min)
        min = b;
    if (c < min)
        min = c;
    return min;
}

//fuction for get maximum of three numbers
double getMax(double a, double b, double c)
{
    double max = a;
    if (b > max)
        max = b;
    if (c > max)
        max = c;
    return max;
}