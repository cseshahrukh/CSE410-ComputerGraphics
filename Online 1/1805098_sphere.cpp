#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
#include <vector>
using namespace std; 



//float triangleSide=1.0f; 
float triangleSide=0.0f; 
float sphereRadius=0.5; 
float radius; //this is var
float bigCentreOfGravity=(1.0f/3);
float cylinderAngle1=70.5; //given in the question 
float phi=109.470; //given in the question
float sphereCenterx=0; 
float sphereCentery=0;
float sphereCenterz=0.5; 
float cylinderRadius=sphereRadius; //they are csir sicsetant  
float scale=0; //this is variable
float d=cylinderRadius*sin(phi/2); 
float cylinderHeight=sqrt(2); //edge of main big triangle 
bool flag=false; 
float rotationCounter=0; 
float rotationCounterx=0; 
float rotationCounterz=0; 
float rotationX=0; 
struct point {
    GLfloat x, y, z;
};

// Global variables
struct point pos;   // position of the eye
struct point l;     // look/forward direction
struct point r;     // right direction
struct point u;     // up direction

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 6; //chokh ei position e ache 
GLfloat centerx = 0, centery = 0, centerz = -2; //look = center - eye 

//upy 1 mane ami upore gele y te change hobe
GLfloat upx = 0, upy = 1, upz = 0; // up (dot) look !=0 and up != look ; mane same o na and perpendicular o na

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);
    glEnd();
}

//Draw a triangle vertices in (1,0,0), (0,1,0), (0,0,1)
void drawTriangle(){
    glPushMatrix(); 
        glTranslated(1.0f/3 - triangleSide/3.0f, 1.0f/3 - triangleSide/3.0f, 1.0f/3 - triangleSide/3.0f);
        glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
            // Front
            //glColor3f(1.0f, 0.0f, 0.0f);     // Red
            glVertex3f( triangleSide, 0.0f, 0.0f);
        
            //glColor3f(0.0f, 1.0f, 0.0f);     // Green
            glVertex3f(0.0f, triangleSide, 0.0f);
        
            //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
            glVertex3f(0.0f, 0.0f, triangleSide);

        glEnd();   // Done drawing the pyramid
    glPopMatrix(); 
}

void drawFourTriangle(){
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    drawTriangle(); 
    
    glColor3f(0.0f, 1.0f, 0.0f);     // Red
    glPushMatrix(); // Create a new scope
        glRotatef(90, 0,1,0);
        drawTriangle(); // Triangle is drawn rotated 90 degrees anti-clockwise (pointing left), 0.2 units to the left of origin
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 0.0f);     // Red
    glPushMatrix(); // Create a new scope
        glRotatef(180, 0,1,0);
        drawTriangle(); // Triangle is drawn rotated 90 degrees anti-clockwise (pointing left), 0.2 units to the left of origin
    glPopMatrix();

    glColor3f(1.0f, 0.0f, 1.0f);     // Red
    glPushMatrix(); // Create a new scope
        glRotatef(270, 0,1,0);
        drawTriangle(); // Triangle is drawn rotated 90 degrees anti-clockwise (pointing left), 0.2 units to the left of origin
    glPopMatrix();

}

void drawEightTriangle()
{
    drawFourTriangle();
    glPushMatrix(); // Create a new scope
        //z er respect e korchi. x er respect e korleo hoto 
        glRotatef(180, 0,0,1);
        drawFourTriangle(); // Triangle is drawn rotated 90 degrees anti-clockwise (pointing left), 0.2 units to the left of origin
    glPopMatrix();
}





void drawCylinder(double height, double radius, int segments) {
    double tempx = radius, tempy = 0;
    double currx, curry;
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = i * cylinderAngle1*M_PI/180 / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            //mane c er emni kono kaj nai. emni color korte geche ar ki 
            GLfloat c = (2+cos(theta))/3;
            glColor3f(c,c,c);

            //function kintu choturvuj er 
            //porer loop er sathe er kono somporko thakbe na 
            glVertex3f(currx, curry, height/2);
            glVertex3f(currx, curry, -height/2);

            //tempx and tempy te hocche oi previous point ta store kore rakhchi 
            glVertex3f(tempx, tempy, -height/2);
            glVertex3f(tempx, tempy, height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}

void drawUpgradedCylinder(){
    radius=(1-scale)*cylinderRadius; 
    //d te change kora lagbe 
    d=radius /sin(phi*M_PI/(2*180)); 
    
    glPushMatrix();
        glRotated(-45, 0, 1,0); 
        glTranslated(1/sqrt(2)-d,0,0);


        glPushMatrix();
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();

        glRotated(90, 0, 1,0); 
        glTranslated(sqrt(2)/2*triangleSide,0,-sqrt(2)/2*triangleSide);
        glPushMatrix();
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();

        glRotated(90, 0, 1,0); 
        glTranslated(sqrt(2)/2*triangleSide,0,-sqrt(2)/2*triangleSide);
        glPushMatrix();
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();

        glRotated(90, 0, 1,0); 
        glTranslated(sqrt(2)/2*triangleSide,0,-sqrt(2)/2*triangleSide);
        glPushMatrix();
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();

    glPopMatrix();

    //will draw 4 more cylinder
    glPushMatrix();
        glRotated(90, 1,0,0); 
        glPushMatrix();
            glRotated(-45, 0, 1,0); 
            glTranslated(1/sqrt(2)-d,0,0);
        
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();

        glRotated(90, 0,1,0); 
        glPushMatrix();
            glRotated(-45, 0, 1,0); 
            glTranslated(1/sqrt(2)-d,0,0);
        
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();

        glRotated(90, 0,1,0); 
        glPushMatrix();
            glRotated(-45, 0, 1,0); 
            glTranslated(1/sqrt(2)-d,0,0);
        
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);

        glPopMatrix();

        glRotated(90, 0,01,0);
        glPushMatrix();
            glRotated(-45, 0, 1,0); 
            glTranslated(1/sqrt(2)-d,0,0);
        
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();
    glPopMatrix();

    //will draw 4 more cylinder
        glPushMatrix();
        glRotated(90, 0,0,1); 
        glPushMatrix();
            glRotated(-45, 0, 1,0); 
            glTranslated(1/sqrt(2)-d,0,0);
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();

        glRotated(90, 0,1,0); 
        glPushMatrix();
            glRotated(-45, 0, 1,0); 
            glTranslated(1/sqrt(2)-d,0,0);
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();

        glRotated(90, 0,1,0); 
        glPushMatrix();
            glRotated(-45, 0, 1,0); 
            glTranslated(1/sqrt(2)-d,0,0);
        
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();

        glRotated(90, 0,01,0);
        glPushMatrix();
            glRotated(-45, 0, 1,0); 
            glTranslated(1/sqrt(2)-d,0,0);
            glRotated(-cylinderAngle1/2, 0,0,1);
            drawCylinder(triangleSide*sqrt(2), radius, 100);
        glPopMatrix();
    glPopMatrix();
}

//Link of Towhid Sir
// generate vertices for +X face only by intersecting 2 circular planes
// (longitudinal and latitudinal) at the given longitude/latitude angles
std::vector<float> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            //chaging here 
            //float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            float scale = radius / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }

    return vertices;
}

/*this function will take array of points and draw a segment of sphere*/
void drawOneSphere(point pointarray[], int n){
    glBegin(GL_QUADS);
        for(unsigned int i = 0; i < n*n - n; i ++)
        {
            if(i%n==n-1) continue; //last element of a col
            unsigned isave=i; 
        //std::cout<<"isave is "<<isave<<endl;
            glVertex3f(pointarray[isave].x, pointarray[isave].y, pointarray[isave].z);

            isave++;
        //std::cout<<"isave is "<<isave<<endl;
            glVertex3f(pointarray[isave].x, pointarray[isave].y, pointarray[isave].z);

            isave=isave+n;
        //std::cout<<"isave is "<<isave<<endl;
            glVertex3f(pointarray[isave].x, pointarray[isave].y, pointarray[isave].z);

            isave--;
        //std::cout<<"isave is "<<isave<<endl;
            glVertex3f(pointarray[isave].x, pointarray[isave].y, pointarray[isave].z);
        
        }
    glEnd();
}

//6 ta sub sphere draw korbo
void drawSubSphere(){
    radius=(1-scale)*cylinderRadius; //this radius is current 
    //std::cout<<"radius is "<<radius<<endl;
    // generate vertices for +X face only by intersecting 2 circular planes
    // (longitudinal and latitudinal) at the given longitude/latitude angles
    std::vector<float> vertices = buildUnitPositiveX(5);
    //if (vertices.size()%3==0)
        //std::cout<<"Asholei kaj kore"<<endl; 

    point pointarray[vertices.size()/3];

    //std::cout<<"size of floats are "<<vertices.size()<<endl; 
    //std::cout<<"size of pointarray is "<<sizeof(pointarray)/sizeof(point)<<endl; 

    unsigned n=sizeof(pointarray)/sizeof(point);
    n=sqrt(n); 
    //std::cout<<"n is "<<n<<endl; 
    for(unsigned int i = 0; i < vertices.size(); i += 3)
    {
        pointarray[i/3].x=vertices[i]; 
        pointarray[i/3].y=vertices[i+1]; 
        pointarray[i/3].z=vertices[i+2]; 
    }
    glPushMatrix();
        //glRotated(rotationX, 0,0,1);
        //glTranslated(sphereCenterx,sphereCentery,sphereCenterz);
        
        
        glPushMatrix();
            glPushMatrix();
                glTranslated(triangleSide, 0,0); 
                drawOneSphere(pointarray, n);
            glPopMatrix();

            glColor3f(0.15,23,36); 
            glPushMatrix();
                glRotated(90, 0,1,0);
                glTranslated(triangleSide, 0,0);
                drawOneSphere(pointarray, n);
            glPopMatrix();

            

            glColor3f(0.15,0.23,0.36); 
            glPushMatrix();
                glRotated(180, 0,1,0);
                glTranslated(triangleSide, 0,0);
                drawOneSphere(pointarray, n);
            glPopMatrix();

            glColor3f(0.15,0.73,0.36); 
            glPushMatrix();
                glRotated(270, 0,1,0);
                glTranslated(triangleSide, 0,0);
                drawOneSphere(pointarray, n);
            glPopMatrix();

            glColor3f(0.55,0.73,0.36); 
            glPushMatrix();
                glRotated(90, 0,0,1);
                glTranslated(triangleSide, 0,0);
                drawOneSphere(pointarray, n);
            glPopMatrix();

            glColor3f(0.95,0.73,0.66); 
            glPushMatrix();
                glRotated(270, 0,0,1);
                glTranslated(triangleSide, 0,0);
                drawOneSphere(pointarray, n);
            glPopMatrix();

        glPopMatrix();
    glPopMatrix();
}




/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    //eitao 3d er jonno 
    // control viewing (or camera)
    //camera koi; camera koi takay ache; 

    if(flag==true)
    {
            gluLookAt(eyex,eyey,eyez,
              centerx,centery,centerz,
              upx,upy,upz);
    }

    else {
         gluLookAt(pos.x,pos.y,pos.z,
              pos.x+l.x,pos.y+l.y,pos.z+l.z,
              u.x,u.y,u.z);
    }
    
    //glRotatef(rotationCounter*15, 0,1,0); 
    //glRotatef(rotationCounterx*15, 1,0,0); 
    //glRotatef(rotationCounterz*15, 0,0,1); 
    //drawFourTriangle(); 
    //drawEightTriangle(); 
    //drawSphere(sphereRadius, 100, 100); 


    drawAxes(); 
    glColor3f(240,56,0);
    //drawUpgradedCylinder();
    glPushMatrix();
        glTranslated(sphereCenterx,sphereCentery,sphereCenterz);
        glRotated(rotationX, 0,0,1);
        drawSubSphere();
    glPopMatrix();    
        
    
    
    //Ekta buffer samne ashbe ar arekta pichone jabe 
    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    //Mane 3D korchi //near er kacher jinis dekhte parbo na. far er durer jinis dekhte parbo na
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    //float v = 0.1;
    switch (key) {
    // Control eye (location of the eye)
    // control eyex
    case '1':
        eyex -= v;
        break;
    case '2':
        eyex += v;
        break;
    // control eyey
    case '3':
        eyey += v;
        break;
    case '4':
        eyey -= v;
        break;
    // control eyez
    case '5':
        centerz -= v;
        break;
    case '6':
        centerz += v;
        break;

    // Control center (location where the eye is looking at)
    // control centerx
    case 'q':
        centerx += v;
        break;
    case 'w':
        centerx -= v;
        break;
    // control centery
    case 'e':
        centery += v;
        break;
    case 'r':
        centery -= v;
        break;
    // control centerz
    case 't':
        centerz += v;
        break;
    case 'y':
        centerz -= v;
        break;

    // Control what is shown
    case 'a':
        eyex += v * (-upy*lz);
        eyez += v * (lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case 'd':
        eyex += v * (upy*lz);
        eyez += v * (-lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    
    case ',':
        //size will not be less than zero
        //triangleSide=fmax(0.0,triangleSide-(1.0f/16)); //as 16 times comma will make it 0 
        //sphereRadius=fmin(1/sqrt(3), sphereRadius+(1.0/sqrt(3)/16.0)); 
        //scale=fmax(0, scale-1.0f/16.0); 
        
        break;
    case '.':
         triangleSide=fmin(1.0,triangleSide+(1.0f/16)); //as 16 times comma will make it 0 
         //sphereRadius=fmax(1/sqrt(3), sphereRadius-(1.0/sqrt(3)/16.0)); 
         scale=fmin(1.0, scale+1.0f/16.0);
         break; 
    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    switch (key) {
    case GLUT_KEY_LEFT:
        eyex-=v;
        break;
    case GLUT_KEY_RIGHT:
        eyex+=v;
        break;
    case GLUT_KEY_UP:
        eyez -= v;
        break;
    case GLUT_KEY_DOWN:
        eyez += v;
        break;
    case GLUT_KEY_PAGE_UP:
        eyey += v;
        break;
    case GLUT_KEY_PAGE_DOWN:
        eyey -= v;
        break;
    default:
        return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for normal-key event */
void keyboardListener2(unsigned char key, int x, int y) {
    double v = 0.25;
    double rate = 0.01;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    //float v = 0.1;
    switch (key) {
        case '1':
			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);
			break;

        case '2':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);
			break;

        case '3':
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-l.x*sin(rate);
			u.y = u.y*cos(rate)-l.y*sin(rate);
			u.z = u.z*cos(rate)-l.z*sin(rate);
			break;

        case '4':
			l.x = l.x*cos(-rate)+u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-l.x*sin(-rate);
			u.y = u.y*cos(-rate)-l.y*sin(-rate);
			u.z = u.z*cos(-rate)-l.z*sin(-rate);
			break;

        case '5':
			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);
			break;

        case '6':
			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);
			break;

    // Control center (location where the eye is looking at)
    // control centerx
        case 'q':
            centerx += v;
            break;
        case 'w':
            centerx -= v;
            break;
    // control centery
        case 'e':
            centery += v;
            break;
        case 'r':
            centery -= v;
            break;
    // control centerz
        case 't':
            centerz += v;
            break;
        case 'y':
            centerz -= v;
            break;

    // Control what is shown
    case 'a':
        //rotationCounter++; 
        sphereCenterx+=0.1; 
        rotationX-=0.1*180/(radius*3.1416); 
        //sphereCentery+=0.1;
        break;
    //case 'd' is opposite to 'a'
    case 'd':
        //rotationCounter--; 
        sphereCenterx-=0.1; 
        rotationX+=0.1*180/(radius*3.1416); 
        //sphereCentery-=0.1;
        break;

    case 'x':
        rotationCounterx++; 
        break;

    case 'c':
        
        rotationCounterx--; 
        break;

    case 'z':
        rotationCounterz++; 
        break;
        
    case 'v': 
        rotationCounterz--; 
        break;



    case ',':
        //size will not be less than zero
        triangleSide=fmax(0.0,triangleSide-(1.0f/16)); //as 16 times comma will make it 0
        triangleSide=0;  
        scale=fmax(0, scale-1.0f/16.0); 
        scale=0; 
        break;

    case '.':
         triangleSide=fmin(1.0,triangleSide+(1.0f/16)); //as 16 times comma will make it 0 
         //sphereRadius=fmax(1/sqrt(3), sphereRadius-(1.0/sqrt(3)/16.0)); 
         scale=fmin(1.0, scale+1.0f/16.0);
         break; 
    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener2(int key, int x,int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    switch (key) {
        case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;
    default:
        return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {


    //center at pos+l
    pos.x=3;pos.y=3;pos.z=3;

    l.x=-1;l.y=-1;l.z=-2;



    u.x=0;u.y=1;u.z=0;
    r.x=1;r.y=0;r.z=0;


    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(640, 640);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    
    if(flag==true)
    {
        glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
        glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event

    }
    else 
    {
        glutKeyboardFunc(keyboardListener2);         // Register callback handler for normal-key event
        glutSpecialFunc(specialKeyListener2);        // Register callback handler for special-key event
    }
    
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}
