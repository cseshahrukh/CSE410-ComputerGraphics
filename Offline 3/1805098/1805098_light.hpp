/*
Author: Md. Shahrukh Islam
ID: 1805098
*/
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;
#define gap " "
class Point
{

    public:
    double x, y, z, n; //n is scale factor

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
        temp.n = 1 ;
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

    // Negation of a point 
    Point operator -()
    {
        Point temp;
        temp.x = -x;
        temp.y = -y;
        temp.z = -z;
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



//calculate the determinant of a 3x3 matrix; input parameter is 2D array
double determinant(double mat[3][3])
{
    double det = 0;
    for(int i=0; i<3; i++)
    {
        det += (mat[0][i] * (mat[1][(i+1)%3] * mat[2][(i+2)%3] - mat[1][(i+2)%3] * mat[2][(i+1)%3]));
    }
    return det;
}



class Light {
public:
    Point position;
    double fallOffParameter;


    void draw() {
        glPointSize(5);
        glBegin(GL_POINTS);

        // Color is white
        glColor3f(1, 1, 1);
        glVertex3f(position.getX(), position.getY(), position.getZ());
        glEnd();
    }

    // input stream of light
    friend istream &operator>>(istream &input, Light &light) {
        double x, y, z;
        input >> x >> y >> z;
        light.position.setX(x);
        light.position.setY(y);
        light.position.setZ(z);

        input >> light.fallOffParameter;
        return input;
    }
};

class SpotLight{
    public:
    Light pointLight; 
    Point direction;
    double cutOffAngle;

    void draw() {
        // Color is white
        glColor3f(1, 1, 1);

        // Draw the cone
        glPushMatrix();

        // Translate to the base position
        glTranslatef(pointLight.position.getX(), pointLight.position.getY(), pointLight.position.getZ());

        // Calculate the direction vector as a normalized vector
        double dirX = direction.getX() - pointLight.position.getX();
        double dirY = direction.getY() - pointLight.position.getY();
        double dirZ = direction.getZ() - pointLight.position.getZ();
        
        // Calculate the length of the direction vector
        double dirLength = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

        // Normalize the direction vector
        dirX /= dirLength;
        dirY /= dirLength;
        dirZ /= dirLength;

        // Calculate the rotation angles
        double pitch = -asin(dirY);
        double yaw = atan2(dirX, dirZ);

        // Rotate to point in the specified direction
        glRotatef(yaw * 180.0 / M_PI, 0.0, 1.0, 0.0);   // Rotate around the y-axis
        glRotatef(pitch * 180.0 / M_PI, 1.0, 0.0, 0.0); // Rotate around the x-axis

        // Scale the cone to adjust height
        glScalef(0.5, 0.5, 0.5); // Adjust the scaling factors as needed

        // Draw the cone using OpenGL's primitive drawing functions
        glutSolidCone(1.0, dirLength, 32, 32);

        glPopMatrix();
    }

    // input stream of light
    friend istream &operator>>(istream &input, SpotLight &spotLight) {
        double x, y, z;
        input >> x >> y >> z;
        spotLight.pointLight.position.setX(x);
        spotLight.pointLight.position.setY(y);
        spotLight.pointLight.position.setZ(z);

        input >> spotLight.pointLight.fallOffParameter;
        
        input >> x >> y >> z;
        spotLight.direction.setX(x);
        spotLight.direction.setY(y);
        spotLight.direction.setZ(z);
        input >> spotLight.cutOffAngle;
        return input;
    }
};

class Ray {
    public: 
    Point origin, direction;

    Ray(){

    }

    Ray(Point origin, Point direction){
        this->origin = origin;
        direction.normalize();
        this->direction = direction;
    }

    // stream
    friend ostream &operator<<(ostream &output, Ray &ray){
        output<<"Origin: "<<ray.origin<<", Direction: "<<ray.direction;
        cout<<"Inside ray stream"<<endl;
        output << "Origin: "<< ray.origin << ", Direction: "<<ray.direction;
        return output;
    }
}; 
