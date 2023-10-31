#define _USE_MATH_DEFINES

#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
#include <chrono>
#include "bitmap_image.hpp"
#include "1805098_Header.hpp"

using namespace std; 
double fovY, aspectRatio, nearPlane, farPlane;
double cameraHeight=150.0, cameraAngle=1.0;
bool drawAxes=true, drawGrid=true;
double angle=0; 
int numOfLights;
int numOfSpotLights;
int numOfObjects;
vector <Object*> objects;
vector <Light*> lights;
vector <SpotLight*> spotLights;
int numOfSegments, recursionLevel, imageWidth, imageHeight;

int imageIndex = 0;
bitmap_image image;

Point cameraPosition(200,0,10); 

double variable = 2.0; 
// up right and look
Point u(0,0,1);
Point r(-1 / sqrt(variable), 1 / sqrt(variable), 0);
Point l(-1/sqrt(variable), -1/sqrt(variable), 0);

// OpenGL PI 
double PI = 2*acos(0.0);
const double DEGREE_TO_RADIAN = PI/180.0;
double rotationAngle = PI/180.0;

int numberOfSegments=36; 

double windowWidth = 500;
double windowHeight = 500;
double viewAngle = 80;

void rotate3D(Point &vec, Point &axis, double angle)
{
    //vec = vec * cos(angle) + (axis^vec) * sin(angle) ;
    vec = vec * cos(angle); 
    vec = vec + (axis^vec) * sin(angle); 
}
void capture()
{
    cout<<"Capturing Image"<<endl; 

    // initialize bitmap image and set the background color black
    for (int i=0; i<imageWidth; i++)
    {
        for (int j=0; j<imageHeight; j++)
        {
            image.set_pixel(i,j,0,0,0);
        }
    }

    double planeDistance = (windowHeight/2.0)/tan((acos(-1)*viewAngle)/360.0);

    Point topLeft = cameraPosition + l*planeDistance - r*(windowWidth/2.0) + u*(windowHeight/2.0);

    double dv = windowHeight / (imageHeight*1.0);
    double du = windowWidth / (imageWidth*1.0);
    
    int nearestObjectIndex = -1;
    double t, tMin; 


    // Now chooing the middle of the grid cell
    topLeft = topLeft + r*(0.5*du) - u*(0.5*dv);



    for (int i=0; i<imageWidth; i++)
    {
        for (int j = 0; j < imageHeight; j++)
        {
            Point pixel = topLeft + r*(i*du) - u*(j*dv); // this is the pixel point
            
            vector<double> color = vector<double>(3,0);

            // cast ray from eye to pixel
            Ray ray(cameraPosition, pixel - cameraPosition);

            // Find the nearest object
            nearestObjectIndex = -1; 
            tMin = -1; 
            

            for(int k=0; k<objects.size(); k++)
            {
                t = objects[k]->intersect(ray, color, 0);

                if (t>0 && (nearestObjectIndex == -1 || t<tMin))
                {
                    nearestObjectIndex = k; 
                    tMin = t; 
                }
            }

            if (nearestObjectIndex == -1)
            {
                //cout<<"No object found"<<endl;
            }
            // if nearest object is found then shade the pixel 
            else if (nearestObjectIndex !=-1)
            {
                vector <double> dummyColor = vector<double>(3,0);

                double t = objects[nearestObjectIndex]->intersect(ray, dummyColor, 1);

                if (dummyColor[0]<0) dummyColor[0] = 0;
                if (dummyColor[1]<0) dummyColor[1] = 0;
                if (dummyColor[2]<0) dummyColor[2] = 0;

                if (dummyColor[0]>1) dummyColor[0] = 1;
                if (dummyColor[1]>1) dummyColor[1] = 1;
                if (dummyColor[2]>1) dummyColor[2] = 1;

                image.set_pixel(i,j,dummyColor[0]*255,dummyColor[1]*255,dummyColor[2]*255);

            }
        }
    }

    image.save_image("1805098Output_1"+to_string(imageIndex)+".bmp");
    cout<<"Saving Image"<<endl; 
    imageIndex++; 
}

void display() {

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int nowVal=0; 
    glClearColor(nowVal,nowVal, nowVal, nowVal);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ(), 
    cameraPosition.getX()+l.getX(), cameraPosition.getY()+l.getY(), cameraPosition.getZ()+l.getZ(), 
    u.getX(), u.getY(), u.getZ());

    glMatrixMode(GL_MODELVIEW);

    for (int i=0; i<objects.size(); i++){
        Object *object = objects[i];
        if (object->type=="floor")
        {
            // object->position.x= cameraPosition.x+l.getX();
            // object->position.y= cameraPosition.y+l.getY();
        }
        object->draw();
        //objects[i]->draw();
    }
    for (int i=0; i<lights.size(); i++){
        lights[i]->draw();
    }
    for (int i=0; i<spotLights.size(); i++){
        spotLights[i]->draw();
    }
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void idle() {
    glutPostRedisplay(); // Post a re-paint request to activate display()

}

void mouseListerer(int button, int state, int x, int y) {
    //cout << "Mouse Listener" << endl;
}

void keyboardListener(unsigned char key, int x, int y) {
    //cout << "Keyboard Listener" << endl;
    switch(key){
        case '0':
            capture(); 
            break; 
        case '1':
            //left rotation 
            rotate3D(r, u, rotationAngle); 
            rotate3D(l, u, rotationAngle); 
            break; 

        case '2':
            //right rotation
            rotate3D(r, u, -rotationAngle); 
            rotate3D(l, u, -rotationAngle); 
            break ; 

        case '3': 
            //up rotation
            rotate3D(u, r, rotationAngle); 
            rotate3D(l, r, rotationAngle); 
            break; 

        case '4': 
            //down rotation
            rotate3D(u, r, -rotationAngle); 
            rotate3D(l, r, -rotationAngle);
            break; 

        case '5':
            //tilt clockwise
            rotate3D(r, l, rotationAngle);
            rotate3D(u, l, rotationAngle);
            break; 

        case '6':
            //tilt anti-clockwise
            rotate3D(r, l, -rotationAngle);
            rotate3D(u, l, -rotationAngle);
            break;
        default: 
            break; 

    }
}

int right, left, forward, backward;
#define move 2
void specialKeyboardListener(int key, int x, int y) {
    //cout << "Special Keyboard Listener" << endl;
    
    switch(key)
    {
        case GLUT_KEY_DOWN:
            // cameraPosition = cameraPosition - l*move;
            cameraPosition = cameraPosition - l;
            cameraPosition = cameraPosition - l;
            
         
            objects[0]->position = objects[0]->position - l;
            //objects[0]->position = objects[0]->position - l;
            break;

        case GLUT_KEY_UP:
            cameraPosition = cameraPosition + l;
            cameraPosition = cameraPosition + l;
            objects[0]->position = objects[0]->position + l;
            //objects[0]->position = objects[0]->position + l;
            break;
        
        case GLUT_KEY_RIGHT:
            cameraPosition = cameraPosition + r;
            cameraPosition = cameraPosition + r;
            objects[0]->position = objects[0]->position + r;
            // objects[0]->position = objects[0]->position + r;
            break;
        
        case GLUT_KEY_LEFT:
            cameraPosition = cameraPosition - r;
            cameraPosition = cameraPosition - r;
            objects[0]->position = objects[0]->position - r;
            // objects[0]->position = objects[0]->position - r;
            break;

        case GLUT_KEY_PAGE_UP:
            cameraPosition = cameraPosition + u;
            cameraPosition = cameraPosition + u;

            break;

        case GLUT_KEY_PAGE_DOWN:
            cameraPosition = cameraPosition - u;
            cameraPosition = cameraPosition - u;
            break;

        default : 
            break;
    }
}
void loadData(){
    ifstream input("description.txt");
    input >> nearPlane >> farPlane>> fovY >> aspectRatio;
    cout<<"nearPlane: "<<nearPlane<<" farPlane: "<<farPlane<<" fovY: "<<fovY<<" aspectRatio: "<<aspectRatio<<endl;



    //ifstream input("description.txt");
    input >> recursionLevel;
    input >> imageWidth; 
    imageHeight = imageWidth; 

    cout<<"recursion level: "<<recursionLevel<<endl;
    cout<<"image width: "<<imageWidth<<endl;

    Object *floor = new Floor();
    
    input >> *((Floor *)floor);

    objects.push_back(floor);

    // now take the objects


    input >> numOfObjects;
    string objectType;
    for (int i=0; i<numOfObjects; i++){
        input>>objectType;

        Object *object;

        if (objectType=="sphere")
        {
            object = new Sphere();

            input >> *((Sphere *)object);

            //cout<<object->getColor()[0]<<" "<<object->getColor()[1]<<" "<<object->getColor()[2]<<endl;
            objects.push_back(object);

        }
        else if (objectType=="cube")
        {
            object = new Cube();
            input >> *((Cube *)object);

            // input er moddhe push er kaj ache
        }
        else if (objectType=="pyramid")
        {
            object = new Pyramid();
            input >> *((Pyramid *)object);
        }
        //objects.push_back(object);
    }


    // now take the lights
    
    input >> numOfLights;
    
    for (int i=0; i<numOfLights; i++)
    {
        Light *light = new Light();
        input >> *light;
        lights.push_back(light);
    }

    input >> numOfSpotLights;
    for (int i=0; i<numOfSpotLights; i++)
    {
        SpotLight *spotLight = new SpotLight();
        input >> *spotLight;
        spotLights.push_back(spotLight);
    }
    

    image = bitmap_image(imageWidth, imageHeight);

    glClearColor(0,0,0,0);

    glMatrixMode(GL_PROJECTION); 


    glLoadIdentity();

    gluPerspective(fovY, aspectRatio, nearPlane, farPlane); // set up the projection matrix
    
    cout<<"object size "<<objects.size()<<endl; 


}

void outputData(){
    //printing everything that I took input in loaddata
    cout << "nearPlane: " << nearPlane << endl;
    cout << "farPlane: " << farPlane << endl;
    cout << "fovY: " << fovY << endl;
    cout << "aspectRatio: " << aspectRatio << endl;
    
    cout<< "level of recursion: " << recursionLevel << endl;
    cout<< "image width: " << imageWidth << endl;

    cout<<"checkerboard width: "<< ((Floor *)objects[0])->length<<endl;
    cout<<"checkerboard others "<< ((Floor *)objects[0])->coefficients[0]<<" "<<((Floor *)objects[0])->coefficients[1]<<" "<<((Floor *)objects[0])->coefficients[2]<<" "<<((Floor *)objects[0])->coefficients[3]<<" "<<((Floor *)objects[0])->coefficients[4]<<endl;

    cout<<"number of objects: "<< numOfObjects << endl;

    for (int i=1; i<objects.size(); i++)
    {
        cout<<"object "<<i<<endl;
        cout<<"type: "<<objects[i]->type<<endl;
        if (objects[i]->type=="sphere")
        {
            cout<<"sphere center: "<<((Sphere *)objects[i])->center.getX()<<" "<<((Sphere *)objects[i])->center.getY()<<" "<<((Sphere *)objects[i])->center.getZ()<<endl;
            cout<<"sphere radius: "<<((Sphere *)objects[i])->radius<<endl;

            cout<<"sphere color: "<<objects[i]->color[0]<<" "<<objects[i]->color[1]<<" "<<objects[i]->color[2]<<endl;
            cout<<"sphere others: "<<objects[i]->coefficients[0]<<" "<<objects[i]->coefficients[1]<<" "<<objects[i]->coefficients[2]<<" "<<objects[i]->coefficients[3]<<" "<<objects[i]->coefficients[4]<<endl;
        }
        else if (objects[i]->type=="cube")
        {
            cout<<"point A: "<<((Cube *)objects[i])->A.getX()<<" "<<((Cube *)objects[i])->A.getY()<<" "<<((Cube *)objects[i])->A.getZ()<<endl;
            cout<<"cube side: "<<((Cube *)objects[i])->width<<endl;
            cout<<"cube color: "<<objects[i]->color[0]<<" "<<objects[i]->color[1]<<" "<<objects[i]->color[2]<<endl;
            cout<<"cube others: "<<objects[i]->coefficients[0]<<" "<<objects[i]->coefficients[1]<<" "<<objects[i]->coefficients[2]<<" "<<objects[i]->coefficients[3]<<" "<<objects[i]->coefficients[4]<<endl;
        }
        else if (objects[i]->type=="pyramid")
        {
            cout<<"Point A: "<<((Pyramid *)objects[i])->A.getX()<<" "<<((Pyramid *)objects[i])->A.getY()<<" "<<((Pyramid *)objects[i])->A.getZ()<<endl;
            
            cout<<"pyramid width: "<<((Pyramid *)objects[i])->width<<endl;
            cout<<"pyramid height: "<<((Pyramid *)objects[i])->height<<endl;
            cout<<"pyramid color: "<<objects[i]->color[0]<<" "<<objects[i]->color[1]<<" "<<objects[i]->color[2]<<endl;
            cout<<"pyramid others: "<<objects[i]->coefficients[0]<<" "<<objects[i]->coefficients[1]<<" "<<objects[i]->coefficients[2]<<" "<<objects[i]->coefficients[3]<<" "<<objects[i]->coefficients[4]<<endl;
        }
    }

    cout<<"number of lights: "<<numOfLights<<endl;
    for (int i=0; i<lights.size(); i++)
    {
        cout<<"light "<<i<<endl;
        cout<<"light position: "<<lights[i]->position.getX()<<" "<<lights[i]->position.getY()<<" "<<lights[i]->position.getZ()<<endl;
        cout<<"light falloff parameter: "<<lights[i]->fallOffParameter<<endl;
    }

    cout<<"number of spot lights: "<<numOfSpotLights<<endl;
    for (int i=0; i<spotLights.size(); i++)
    {
        cout<<"spot light "<<i<<endl;
        cout<<"spot light position: "<<spotLights[i]->pointLight.position.getX()<<" "<<spotLights[i]->pointLight.position.getY()<<" "<<spotLights[i]->pointLight.position.getZ()<<endl;
        cout<<"spot light falloff parameter: "<<spotLights[i]->pointLight.fallOffParameter<<endl;
        cout<<"spot light direction: "<<spotLights[i]->direction.getX()<<" "<<spotLights[i]->direction.getY()<<" "<<spotLights[i]->direction.getZ()<<endl;
        cout<<"spot light cutoff angle: "<<spotLights[i]->cutOffAngle<<endl;
    }
    
    
}

void printRawData()
{
    // print the objects
    for (int i=0; i<objects.size(); i++)
    {
        cout<<"object "<<i<<endl;
        cout<<"type: "<<objects[i]->type<<endl;
        
        if (objects[i]->type=="triangle")
        {
            //print the vertices
            cout<<"vertices: "<<endl;
            cout<<((Triangle *)objects[i])->a.getX()<<" "<<((Triangle *)objects[i])->a.getY()<<" "<<((Triangle *)objects[i])->a.getZ()<<endl;
            cout<<((Triangle *)objects[i])->b.getX()<<" "<<((Triangle *)objects[i])->b.getY()<<" "<<((Triangle *)objects[i])->b.getZ()<<endl;
            cout<<((Triangle *)objects[i])->c.getX()<<" "<<((Triangle *)objects[i])->c.getY()<<" "<<((Triangle *)objects[i])->c.getZ()<<endl;
        }
        
    }
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
int main(int argc, char** argv) {
    
    
    glutInit(&argc, argv);                 // Initialize GLUT
    glutCreateWindow("Ray Tracing"); // Create a window with the given title
    glutInitWindowSize(800, 800);   // Set the window's initial width & height
    glutInitWindowPosition(0,0); // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Enable depth buffer and double buffering
    
    
    
    // initialization function 
    loadData();

    //outputData();
    //printRawData();
    glEnable(GL_DEPTH_TEST); // Enable depth testing for z-culling
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    //glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutIdleFunc(idle); // Register idle callback handler for window re-paint

    glutMouseFunc(mouseListerer); // Register mouse callback handler for window re-paint
    glutKeyboardFunc(keyboardListener); // Register keyboard callback handler for window re-paint
    glutSpecialFunc(specialKeyboardListener); // Register special callback handler for window re-paint
    glutMainLoop();           // Enter the infinitely event-processing loop

    objects.clear();
    lights.clear();
    spotLights.clear();
    
    return 0;
}