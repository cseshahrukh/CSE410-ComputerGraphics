/*
Author: Md. Shahrukh Islam
ID: 1805098
*/

#include <bits/stdc++.h>
using namespace std;
#define gap " "
class Point
{

    private: 
    double x, y, z, n; //n is scale factor

    public:

    //constructors
    Point()
    {
        x = y = z = n = 0.0;
        n = 1.0;
    }
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        n = 1.0;
    }

    Point(double x, double y, double z, double n)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->n = n;
    }

    Point(const Point &p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
        n = p.n;
    }

    //getters
    double getX()
    {
        return x;
    }

    double getY()
    {
        return y;
    }

    double getZ()
    {
        return z;
    }

    double getn()
    {
        return n;
    }

    //setters
    void setX(double x)
    {
        this->x = x;
    }

    void setY(double y)
    {
        this->y = y;
    }

    void setZ(double z)
    {
        this->z = z;
    }

    void setn(double n)
    {
        this->n = n;
    }
    
    //arighmetic operations
    Point operator+(const Point &p)
    {
        Point temp;
        temp.x = x + p.x;
        temp.y = y + p.y;
        temp.z = z + p.z;
        temp.n = n + p.n;
        return temp;
    }

    Point operator-(const Point &p)
    {
        Point temp;
        temp.x = x - p.x;
        temp.y = y - p.y;
        temp.z = z - p.z;
        temp.n = n - p.n;
        return temp;
    }

    //dot product
    double operator*(const Point &p)
    {
        return (x * p.x + y * p.y + z * p.z);
    }

    //scalar multiplication
    Point operator*(const double d)
    {
        Point temp;
        temp.x = x * d;
        temp.y = y * d;
        temp.z = z * d;
        temp.n = n ;
        return temp;
    }

    //scalar division
    Point operator/(const double d)
    {
        Point temp;
        temp.x = x / d;
        temp.y = y / d;
        temp.z = z / d;
        temp.n = n;
        return temp;
    }

    //cross product
    Point operator^(const Point &p)
    {
        Point temp;
        temp.x = y * p.z - z * p.y;
        temp.y = z * p.x - x * p.z;
        temp.z = x * p.y - y * p.x;
        temp.n = 1;
        return temp;
    }

    //length from origin
    double length()
    {
        return sqrt(x * x + y * y + z * z);
    }

    void normalize()
    {
        double len = sqrt(x * x + y * y + z * z);
        x /= len;
        y /= len;
        z /= len;
    }

    void scale()
    {
        //supporse x was 16 and n was 2
        //then x = 16/2 = 8 and n = 2/2 = 1
        //that means n is now ok after operation
        x /= n;
        y /= n;
        z /= n;
        n = 1;

         
    }

    void printPoint()
    {
        cout << x << " " << y << " " << z << " " << n << endl;
    }

    //friend functions
    //different 
    friend ostream &operator<<(ostream &output, const Point &p)
    {
        output << p.x << " " << p.y << " " << p.z;
        return output;
    }

    //Will take point like below and rest is normal
    //input
    friend istream &operator>>(istream &input, Point &p)
    {
        input >> p.x >> p.y >> p.z ;
        return input;
    }

    //output to file
    friend ofstream &operator<<(ofstream &output, const Point &p)
    {
        //In test case 7 decimal points used 
        output << fixed <<setprecision(7) <<p.x << gap << p.y << gap << p.z;
        return output;
    }



};