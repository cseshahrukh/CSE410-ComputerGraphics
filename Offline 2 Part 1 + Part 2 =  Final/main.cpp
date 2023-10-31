/*
Author: Md. Shahrukh Islam
ID: 1805098
*/

#include "helpMatrix.h"
#include "bitmap_image.hpp"
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

void stage4()
{
    cout<<"Start stage4"<<endl; 
    ifstream fin("config.txt");
    int screen_width, screen_height;
    fin >> screen_width >> screen_height;

    //box dimension
    double left, right, up, down; 
    left=-1, right=1, up=1, down=-1;

    double zMin=-1, zMax=1;
    fin.close(); 

    //cout<<"Width: "<<width<<" Height: "<<height<<endl;

    fin.open("stage3.txt");
    ofstream fout("z_buffer.txt");

    //pixel dimension 
    double dx=(right-left)/screen_width;
    double dy=(up-down)/screen_height;

    //cout<<"dx: "<<dx<<" dy: "<<dy<<endl;

    // create z buffer with each cell initialized to zMax 
    // Assumming it is far 
    vector<vector <double> > zBuffer(screen_height, vector<double>(screen_width, zMax)); //height is row and width is column

    // create color buffer with each cell initialized to black
    // in the bitmap file width if given first and then height !! Reverse of mine actually. 
    bitmap_image image(screen_width, screen_height);
    for(int i=0; i<screen_width; i++)
    {
        for(int j=0; j<screen_height; j++)
        {
            //each cell with black color
            image.set_pixel(i, j, 0, 0, 0);
        }
    }

    double topY, bottomY, leftX, rightX;
    topY = up-dy/2; // center of top pixel
    bottomY = down+dy/2;
    leftX = left+dx/2;
    rightX = right-dx/2;

    //cout<<"topY: "<<topY<<" bottomY: "<<bottomY<<" leftX: "<<leftX<<" rightX: "<<rightX<<endl;

    for (int triangle=0; triangle<triangleCounter; triangle++)
    {
        //cout<<"Inside trinalge for loop"<<endl; 
        Point p1, p2, p3; 
        fin >> p1 >> p2 >> p3;

        Triangle tri(p1, p2, p3);

        vector<Point> points=tri.getPoints();
        cout<<points[0]<<gap<<points[1]<<gap<<points[2]<<endl;


        double minX=getMin(p1.getX(), p2.getX(), p3.getX());
        double maxX=getMax(p1.getX(), p2.getX(), p3.getX());
        double minY=getMin(p1.getY(), p2.getY(), p3.getY());
        double maxY=getMax(p1.getY(), p2.getY(), p3.getY());

        //cout<<"minX: "<<minX<<" maxX: "<<maxX<<" minY: "<<minY<<" maxY: "<<maxY<<endl;
        // Now it's time for cliping 
        maxY = min(maxY, topY);
        minY = max(minY, bottomY); 
       

        maxX = min(maxX, rightX);
        minX = max(minX, leftX);
        
        //cout<<"minX: "<<minX<<" maxX: "<<maxX<<" minY: "<<minY<<" maxY: "<<maxY<<endl;
        // Y value to start iteration 

        int y_start = round ((topY - maxY) / dy);
        int y_end = round ((topY - minY) / dy);
        //cout<<"y_start: "<<y_start<<" y_end: "<<y_end<<endl;
        // scanline along Y values
        for (int nowY = y_start; nowY <= y_end; nowY++)
        {
            //cout<<"Inside nowY"<<endl; 
            double y_current = topY - nowY * dy; // y value of the scanline; scan is row number 

            int counter=0; 

            double x_left, x_right; // x value of the left and right end of the scanline
            double z_left, z_right; // z value of the left and right end of the scanline

            // In triangle there are 3 points 
            vector<double> xab(2); 
            vector<double> zab(2); 
            

            vector<Point> thisTrianglePoints=tri.getPoints(); 
            for ( int i =0; i<3 ; i++)
            {
                int j = (i+1)%3; // next point
                if (thisTrianglePoints[i].getY() == thisTrianglePoints[j].getY())
                {
                    continue; // horizontal line
                }

                // check if the scanline intersects with the edge/scanline
                else if (y_current >= min (thisTrianglePoints[i].getY(), thisTrianglePoints[j].getY()))
                {
                    if (y_current<= max(thisTrianglePoints[i].getY(), thisTrianglePoints[j].getY()))
                    {

                        // slide formula !!
                        //cout<<"Slide Formula First"<<endl; 
                        xab[counter]=thisTrianglePoints[i].getX() - (thisTrianglePoints[i].getX()-thisTrianglePoints[j].getX())*(thisTrianglePoints[i].getY()-y_current)/(thisTrianglePoints[i].getY()-thisTrianglePoints[j].getY());
                        zab[counter]=thisTrianglePoints[i].getZ() - (thisTrianglePoints[i].getZ()-thisTrianglePoints[j].getZ())*(thisTrianglePoints[i].getY()-y_current)/(thisTrianglePoints[i].getY()-thisTrianglePoints[j].getY());
                        counter++;
                    }
                }
            }

            double saveXab[2], saveZab[2];
            //vector<double> saveXab(2);
            //vector<double> saveZab(2);
            //saveXab=xab;
            for (int i=0; i<2; i++)
            {
                saveXab[i]=xab[i];
                saveZab[i]=zab[i];
            }


            //clip x on both sides
            for (int i=0; i<2; i++)
            {
                // if (xab[i]<minX)
                //     xab[i]=minX;
                xab[i]=max(minX, xab[i]);
                
                // if (xab[i]>maxX)
                //     xab[i]=maxX;
                xab[i]=min(maxX, xab[i]);
            }

            // now this is readjusting 
            zab[0]=zab[1] - (zab[1]-zab[0])*(saveXab[1]-xab[0])/(saveXab[1]-saveXab[0]);
            zab[1]=zab[1] - (zab[1]-zab[0])*(saveXab[1]-xab[1])/(saveXab[1]-saveXab[0]);

            double x_l = xab[0];
            double x_r = xab[1];

            double z_l = zab[0];
            double z_r = zab[1];

            if (xab[0]<xab[1])
            {
                //do nothing 
            }
            else 
            {
                //cout<<"Inside swap"<<endl;
                swap(x_l, x_r);
                swap(z_l, z_r);
                //swap(saveXab[0], saveXab[1]);
                // swap array elements
                std::swap(saveXab[0], saveXab[1]);
            }

            int start=round((x_l-leftX)/dx);
            int endd=round((x_r-leftX)/dx);

            for(int nowX=start; nowX<=endd; nowX++)
            {
                //cout<<"Inside nowX"<<endl;
                double x_current = leftX + nowX*dx;

                // Slide Formula
                double z_current=z_r - (z_r-z_l)*((x_r-x_current)/(x_r-x_l));

                if(z_current<zMin)
                {
                    continue; // behind the near plane
                }

                // missing some 
                else if (z_current<zBuffer[nowY][nowX])
                {
                    zBuffer[nowY][nowX]=z_current;
                    vector<double> color=tri.getColors();
                    image.set_pixel(nowX, nowY, color[0], color[1], color[2]);
                }
            }
            
        }
    

            
    }

    //save to the file 
    for (int i=0; i< screen_height; i++)
    {
        for (int j=0; j<screen_width; j++)
        {
            
            if(zBuffer[i][j]<zMax)
            {
                //cout<<"Should print this"<<gap<<endl; 
                fout << setprecision(6) << fixed << zBuffer[i][j] << tab;
            }
            cout<<endl; 
    
        }
        fout << endl;
    }

    image.save_image("out.bmp");

    fin.close(); 
    fout.close();

    zBuffer.clear();
    image.clear(); 


}
    


int main()
{
    
    stage1();   // stage 1 : Modeling Transformation

    stage2();   //stage 2 : View Transformation

    stage3();   //stage 3 : Projection Transformation

    stage4();   //stage 4 : Buffer Algorithm

    return 0; 
}