/*
Author: Md. Shahrukh Islam
ID: 1805098
*/

#include "helpMatrix.h"
using namespace std;


Point look, eye, up; 

    //fovY is field of view angle in Y direction
    //aspectRatio is the ratio of width to height
    //near is the distance of near plane from the eye
    //far is the distance of far plane from the eye
double fovY, aspectRatio, near, far;

//Need this for me
int triangleCounter=0;


//Model Transformation
void stage1(){
    //cout<<"Hello World!"<<endl;
    //cout<<"This is stage1.cpp"<<endl;

    


    //fin is declared in helpPoint.h as we take input as point from scene.txt
    //ifstream fin("scene.txt");
    ifstream fin("scene.txt"); 
    //fout is declared in helpMatrix.h as we give output as point in stage1.txt
    ofstream fout("stage1.txt");

    fin >> eye >> look >> up; 

    //Normal input
    fin >> fovY >> aspectRatio >> near >> far;

    stack<Matrix> myStack; 
    Matrix identityMatrix(4);
    identityMatrix.setIdentity();

    myStack.push(identityMatrix);

    bool flag=true; 
    //cout<<"Starting the loop"<<endl;
    while(flag)
    {
        string command; 
        fin >> command;

        //We need to create triangle
        if (command=="triangle")
        {
            //cout<<"Inside Triangle Command"<<endl; 
            Point p1, p2, p3; 

            //Taking input from file
            fin >> p1 >> p2 >> p3; 

            //Creating the matrix
            Matrix m(4); 
            
            //Matrix * Point
            p1=myStack.top()*p1;
            p2=myStack.top()*p2;
            p3=myStack.top()*p3;

            //Writing the output in the file
            fout<<p1<<endl<<p2<<endl<<p3<<endl<<endl;

            //Pushing the matrix in the stack
            //myStack.push(m); 

            triangleCounter++; 
        }

        else if (command=="push")
        {
            //need to copy the current state
            Matrix m=myStack.top();
            myStack.push(m);
        }

        else if (command=="pop")
        {
            //if it is not empty then pop
            if(myStack.empty()==false)
            {
                myStack.pop();
            }
            else 
            {
                cout<<"Stack is empty"<<endl;
            }
        }

        else if(command=="translate")
        {
            double tx, ty, tz; 
            fin >> tx >> ty >> tz; 

            Point p(tx, ty, tz);
            //direct fin>>p also works 

            //Creating the matrix
            Matrix m(4); 

            m.setIdentity();
            m.setTranslate(p);

            //Multiplying the matrix with the top of the stack
            Matrix top=myStack.top();
            Matrix res=top*m;
            myStack.pop();
            myStack.push(res);
        }

        else if (command=="scale")
        {
            Point p; 
            fin>>p; 

            //Creating the matrix
            Matrix m(4); 

            m.setIdentity();
            m.setScale(p);

            //Multiplying the matrix with the top of the stack
            Matrix top=myStack.top();
            Matrix res=top*m;
            myStack.pop();
            myStack.push(res);
        }

        else if(command=="rotate")
        {
            double angle, rx, ry, rz; 
            
            fin >> angle >> rx >> ry >> rz; 
            Point p(rx, ry, rz);

            //Creating the matrix
            Matrix m(4); 

            m.setIdentity();
            m.setRotation(angle, p);

            //Multiplying the matrix with the top of the stack
            Matrix top=myStack.top();
            Matrix res=top*m;
            myStack.pop();
            myStack.push(res);
        }

        else if(command=="end")
        {
            break; 
        }
        else 
        {
            cout<<"Invalid Command in input file"<<endl;
        }
    }
    
    fin.close();
    fout.close();
}


//View Transformation
void stage2(){
    ifstream fin("stage1.txt");
    ofstream fout("stage2.txt");
    
    Matrix m(4);
    m.setView(eye, look, up);

    int n=triangleCounter;
    while(n--)
    {
        Point p1, p2, p3; 
        fin >> p1 >> p2 >> p3; 

        p1=m*p1;
        p2=m*p2;
        p3=m*p3;

        fout<<p1<<endl<<p2<<endl<<p3<<endl<<endl;
    }

    fin.close();
    fout.close();

}

void stage3()
{
    //cout<<"Start of stage 3"<<endl;
    ifstream fin("stage2.txt");
    ofstream fout("stage3.txt");

    Matrix m(4);
    m.setProjection(fovY, aspectRatio, near, far);
    
    int n=triangleCounter;
    while(n--)
    {
        Point p1, p2, p3; 
        fin >> p1 >> p2 >> p3; 

        p1=m*p1;
        p2=m*p2;
        p3=m*p3;

        fout<<p1<<endl<<p2<<endl<<p3<<endl<<endl;
    }
    
    fin.close();
    fout.close();
    //cout<<"End of stage 3"<<endl;
}


int main()
{
    
    stage1();   // stage 1 : Modeling Transformation

    stage2();   //stage 2 : View Transformation

    stage3();   //stage 3 : Projection Transformation


    return 0; 
}