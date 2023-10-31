/*
Author: Md. Shahrukh Islam
ID: 1805098
*/

#include <bits/stdc++.h>
#include "helpPoint.h"
using namespace std;

class Matrix
{
    vector<vector <double> > mat;
    int dim; //dimension of the matrix

    public:

    Matrix()
    {
        mat.resize(4, vector<double>(4, 0));
        dim = 4;
    }

    //n is the dimension of the matrix
    Matrix(int n)
    {
        dim = n;
        mat.resize(n, vector<double>(n, 0));
        //cout<<"Inside Matrix(int n) dim is "<<dim<<endl;
    }

    //print the matrix
    void printMatrix()
    {
        cout<<"Printing matrix"<<endl;
        for(int i=0; i<dim; i++)
        {
            for(int j=0; j<dim; j++)
            {
                cout<<mat[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    //set the matrix
    void setMatrix(vector<vector <double> > m)
    {
        mat = m;
    }

    //get the matrix
    vector<vector <double> > getMatrix()
    {
        return mat;
    }


    //Multiplication of two matrices
    Matrix operator*(Matrix &m)
    {
        Matrix res(dim);
        for(int i=0; i<dim; i++)
        {
            for(int j=0; j<dim; j++)
            {
                for(int k=0; k<dim; k++)
                {
                    res.mat[i][j] += mat[i][k] * m.mat[k][j];
                }
            }
        }
        return res;
    }

    //Multiplication of a matrix with a vector
    Point operator*(Point p)
    {
        //Assumming the vector is point p
        double res[4] = {0};

        //Taking the p point in here to work with
        double b[4] = {p.getX(), p.getY(), p.getZ(), p.getn()};

        //Multiplying the matrix with the vector
        //example point * matrix
        /* 
            4x4 and 4x1 = 4x1
            showing for just one row
            [a b c d] [x]   
                      [y] = [ax + by + cz + d]
                      [z]   
                      [n]   
        
        
        */
        //cout<<"dim is "<<dim<<endl;
        //printMatrix();
        for(int i=0; i<dim; i++)
        {
            //inner loop example given above
            for(int j=0; j<dim; j++)
            {
                res[i] += mat[i][j] * b[j];
            }
        }

        //printMatrix();
        //We don't want to return this matrix
        //We need point 

        Point returnPoint(res[0], res[1], res[2], res[3]);

        returnPoint.setX(res[0]);
        returnPoint.setY(res[1]);
        returnPoint.setZ(res[2]);
        returnPoint.setn(res[3]);

        returnPoint.scale(); //if we have n not equal to 1
        //returnPoint.printPoint();
        return returnPoint; 
    }

    //Set the matrix as identity matrix
    void setIdentity()
    {
        for(int i=0; i<dim; i++)
        {
            for(int j=0; j<dim; j++)
            {
                if(i==j)
                {
                    mat[i][j] = 1;
                }
                else
                {
                    mat[i][j] = 0;
                }
            }
        }
    }

    void setTranslate(Point p)
    {
        setIdentity();
        mat[0][3] = p.getX();
        mat[1][3] = p.getY();
        mat[2][3] = p.getZ();
    }

    void setScale(Point p)
    {
        setIdentity();
        mat[0][0] = p.getX();
        mat[1][1] = p.getY();
        mat[2][2] = p.getZ();
    }

    void setRotation(double angle, Point p)
    {
        setIdentity();

        Point p_normalized(p.getX(), p.getY(), p.getZ());
        p_normalized.normalize();

        Point xaxis(1, 0, 0);
        Point yaxis(0, 1, 0);
        Point zaxis(0, 0, 1);


        double radian = (angle * acos(-1)) / 180.0;
        double cosTheta = cos(radian);
        double sinTheta = sin(radian);

        //Code of Rodrigues' Rotation Formula
        // ^ means cross product and * means dot product in my code 
        Point c1= (xaxis * cosTheta) + (p_normalized * (p_normalized*xaxis) * (1 - cosTheta))+(p_normalized^xaxis * sinTheta);
        Point c2= (yaxis * cosTheta) + (p_normalized * (p_normalized*yaxis) * (1 - cosTheta))+(p_normalized^yaxis * sinTheta);
        Point c3= (zaxis * cosTheta) + (p_normalized * (p_normalized*zaxis) * (1 - cosTheta))+(p_normalized^zaxis * sinTheta);

        //mat is field of the class Matrix
        mat[0][0] = c1.getX();
        mat[1][0] = c1.getY();
        mat[2][0] = c1.getZ();

        mat[0][1] = c2.getX();
        mat[1][1] = c2.getY();
        mat[2][1] = c2.getZ();

        mat[0][2] = c3.getX();
        mat[1][2] = c3.getY();
        mat[2][2] = c3.getZ();

        
    }


    void setView(Point eye, Point look, Point up)
    {
        setIdentity();

        Point l = look - eye;
        l.normalize();

        Point r = l^up; //r is like x axis
        r.normalize();

        Point u = r^l; //u is like y axis
        u.normalize();

        mat[0][0] = r.getX();
        mat[0][1] = r.getY();
        mat[0][2] = r.getZ();

        mat[1][0] = u.getX();
        mat[1][1] = u.getY();
        mat[1][2] = u.getZ();

        mat[2][0] = -l.getX();
        mat[2][1] = -l.getY();
        mat[2][2] = -l.getZ();

        Matrix last;
        Point pointTemp(-eye.getX(), -eye.getY(), -eye.getZ());

        pointTemp.setX(-eye.getX());
        pointTemp.setY(-eye.getY());
        pointTemp.setZ(-eye.getZ());
        pointTemp.setn(1);
        
        last.setTranslate(pointTemp);
        
        //Simple matrix multiplication
        *this = *this * (last);

    }

    //Need this for state 3 
    void setProjection(double fovY, double aspectRatio, double near, double far)
    {
        setIdentity();

        double fovX = fovY * aspectRatio;
        
        //(fovY*2)*PI/180 is similar to fovY*PI/360
        double t = near * tan((fovY * acos(-1)) / 360.0);
        double r = near * tan((fovX * acos(-1)) / 360.0);

        mat[0][0] = near / r;
        mat[1][1] = near / t;
        mat[2][2] = -(far + near) / (far - near);
        mat[2][3] = -(2 * far * near) / (far - near);
        mat[3][2] = -1;
        mat[3][3] = 0;
    }



};